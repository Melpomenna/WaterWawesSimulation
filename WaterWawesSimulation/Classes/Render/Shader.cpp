#include "Shader.h"

#include <new>
#include <cstring>
#include <fstream>
#include <iostream>
#include <Classes/Render/glad.h>

namespace Render
{
    void CreateProgram(ShaderProgram* program, const ShaderParams* params) noexcept
    {
        const unsigned short ShadersSize[] = {params->FragSize, params->VertSize, params->GeomSize, params->TessSize};
        const char* shadersNames[] = {
            params->FragmentShaderFile, params->VertexShaderFile, params->GeometryShaderFile,
            params->TesselationShaderFile
        };

        program->Id = glCreateProgram();

        constexpr unsigned shadersSize = static_cast<unsigned>(ShaderType::End);
        for (unsigned i = 0; i < shadersSize; ++i)
        {
            if (shadersNames[i] == nullptr)
            {
                continue;
            }

            Shader* shader = &program->Shaders[static_cast<int>(ShaderType::Index0) + i];

            shader->ShaderNameSize = ShadersSize[i];
            shader->ShaderName = new(std::nothrow) char[shader->ShaderNameSize];
            _memccpy(shader->ShaderName, shadersNames[i], sizeof(char),
                     shader->ShaderNameSize);
            CreateShader(shader);

            glAttachShader(program->Id, shader->Id);
        }

        glLinkProgram(program->Id);
        GLint status;

        glGetProgramiv(program->Id, GL_LINK_STATUS, &status);

        if (status != GL_TRUE)
        {
            GLint length;
            glGetProgramiv(program->Id, GL_INFO_LOG_LENGTH, &length);
            char* info = new(std::nothrow) char[length];

            if (!info)
            {
                return;
            }

            glGetProgramInfoLog(program->Id, length, &length, info);

            std::cerr << info << "\n";

            delete[] info;
            DeleteProgram(program);
            return;
        }
        glValidateProgram(program->Id);
        glGetProgramiv(program->Id, GL_VALIDATE_STATUS, &status);

        if (status != GL_TRUE)
        {
            GLint length;
            glGetProgramiv(program->Id, GL_INFO_LOG_LENGTH, &length);
            char* info = new(std::nothrow) char[length];

            if (!info)
            {
                return;
            }

            glGetProgramInfoLog(program->Id, length, &length, info);

            std::cerr << info << "\n";

            delete[] info;
            DeleteProgram(program);
            return;
        }


        for (unsigned i = 0; i < shadersSize; ++i)
        {
            if (shadersNames[i] != nullptr)
            {
                glDetachShader(program->Id, program->Shaders[i].Id);
            }
        }

        {
            GLint numAttributes;
            glGetProgramInterfaceiv(program->Id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttributes);
            constexpr GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};

            program->InAttributes = new(std::nothrow) Attribute[numAttributes];

            if (program->InAttributes == nullptr)
            {
                return;
            }

            program->InAttributesCount = numAttributes;

            for (GLint i = 0; i < numAttributes; ++i)
            {
                GLint result[3];
                glGetProgramResourceiv(program->Id, GL_PROGRAM_INPUT, i, 3, properties, 3, nullptr, result);
                const GLint nameBuffSize = result[0] + 1;
                char* name = new(std::nothrow) char[nameBuffSize];
                glGetProgramResourceName(program->Id, GL_PROGRAM_INPUT, i, nameBuffSize, nullptr, name);
                name[nameBuffSize - 1] = '\0';
                program->InAttributes[i].Name = name;
                program->InAttributes[i].Location = result[2];
                program->InAttributes[i].Type = result[1];
                program->InAttributes[i].NameSize = nameBuffSize;
            }
        }

        {
            GLint numAttributes;
            glGetProgramInterfaceiv(program->Id, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &numAttributes);
            constexpr GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};

            program->OutAttributes = new(std::nothrow) Attribute[numAttributes];

            if (program->OutAttributes == nullptr)
            {
                return;
            }

            program->OutAttributesCount = numAttributes;

            for (GLint i = 0; i < numAttributes; ++i)
            {
                GLint result[3];
                glGetProgramResourceiv(program->Id, GL_PROGRAM_OUTPUT, i, 3, properties, 3, nullptr, result);
                const GLint nameBuffSize = result[0] + 1;
                char* name = new(std::nothrow) char[nameBuffSize];
                glGetProgramResourceName(program->Id, GL_PROGRAM_OUTPUT, i, nameBuffSize, nullptr, name);
                name[nameBuffSize - 1] = '\0';
                program->OutAttributes[i].Name = name;
                program->OutAttributes[i].Location = result[2];
                program->OutAttributes[i].Type = result[1];
                program->OutAttributes[i].NameSize = nameBuffSize;
            }
        }

        {
            GLint numUniforms;
            glGetProgramInterfaceiv(program->Id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
            constexpr GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};

            program->Uniforms = new(std::nothrow) Uniform[numUniforms];
            program->UniformsCount = numUniforms;
            for (GLint i = 0; i < numUniforms; ++i)
            {
                GLint result[4];
                glGetProgramResourceiv(program->Id, GL_UNIFORM, i, 4, properties, 4, nullptr, result);
                const GLint nameBuffSize = result[0] + 1;
                char* name = new(std::nothrow) char[nameBuffSize];
                glGetProgramResourceName(program->Id, GL_UNIFORM, i, nameBuffSize, nullptr, name);
                name[nameBuffSize - 1] = '\0';
                program->Uniforms[i].Name = name;
                program->Uniforms[i].Location = result[2];
                program->Uniforms[i].Type = result[1];
                program->Uniforms[i].NameSize = nameBuffSize;
                program->Uniforms[i].BlockIndex = result[3];
            }
        }
    }

    void UseProgram(const ShaderProgram* program) noexcept
    {
        glUseProgram(program->Id);
    }


    void DeleteProgram(ShaderProgram* program) noexcept
    {
        glDeleteProgram(program->Id);

        program->Id = 0;

        for (unsigned i = 0; i < sizeof(program->Shaders) / sizeof(Shader); ++i)
        {
            DeleteShader(program->Shaders + i);
        }

        for (unsigned i = 0; i < program->InAttributesCount; ++i)
        {
            delete[] program->InAttributes[i].Name;
        }

        for (unsigned i = 0; i < program->UniformsCount; ++i)
        {
            delete[] program->Uniforms[i].Name;
        }

        for (unsigned i = 0; i < program->OutAttributesCount; ++i)
        {
            delete[] program->OutAttributes[i].Name;
        }

        delete[] program->InAttributes;
        delete[] program->Uniforms;
        delete[] program->OutAttributes;

        program->InAttributes = nullptr;
        program->Uniforms = nullptr;
        program->OutAttributes = nullptr;
        program->InAttributesCount = 0;
        program->OutAttributesCount = 0;
        program->UniformsCount = 0;
    }

    void CreateShader(Shader* shader) noexcept
    {
        shader->Id = glCreateShader(shader->GlType);

        std::ifstream stream{shader->ShaderName};

        if (!stream.is_open())
        {
            return;
        }

        shader->SourceSize = stream.seekg(0, std::ios::end).tellg();
        stream.seekg(0, std::ios::beg);

        shader->Source = new(std::nothrow) char[shader->SourceSize]();

        stream.read(shader->Source, shader->SourceSize);

        glShaderSource(shader->Id, 1, nullptr, nullptr);
        glCompileShader(shader->Id);


        GLint status;
        glGetShaderiv(shader->Id, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE)
        {
            GLint length;
            glGetShaderiv(shader->Id, GL_INFO_LOG_LENGTH, &length);
            char* info = new(std::nothrow) char[length];

            if (!info)
            {
                return;
            }

            glGetShaderInfoLog(shader->Id, length, &length, info);

            std::cerr << info << "\n";

            delete[] info;

            DeleteShader(shader);
        }
    }

    void DeleteShader(Shader* shader)
    {
        delete[] shader->ShaderName;
        shader->ShaderName = nullptr;
        shader->ShaderNameSize = 0;
        shader->Id = 0;
        delete[] shader->Source;
        shader->Source = nullptr;
        shader->SourceSize = 0;
        shader->GlType = 0;
    }
}
