#pragma once

#include <cstdint>
#include <Classes/Render/glad.h>
#include <utility>

#define DEFAULT_VERTEX_SHADER "./Classes/Render/Shaders/Vertex.vert"
#define DEFAULT_FRAGMENT_SHADER "./Classes/Render/Shaders/Fragment.frag"

namespace Render
{
    enum class ShaderType : uint_fast16_t // NOLINT(performance-enum-size)
    {
        FragmentShader = GL_FRAGMENT_SHADER,
        VertexShader = GL_VERTEX_SHADER,
        GeometryShader = GL_GEOMETRY_SHADER,
        TesselationShader = GL_COMPUTE_SHADER,
        /**
         * @brief Fragment Shader Index
         */
        Index0 = 0,

        /**
         * @brief Vertex Shader Index
         */
        Index1,
        /**
         * @brief Geometry Shader Index
         */
        Index2,
        /**
         * @brief Tesselation Shader Index
         */
        Index3,
        End,
    };

    struct Shader
    {
        char* Source;
        char* ShaderName;

        union
        {
            unsigned GlType;
            ShaderType Type;
        };

        long long SourceSize;
        unsigned Id;
        unsigned ShaderNameSize;
    };

    struct alignas(32) Attribute
    {
        char* Name;
        unsigned NameSize;
        unsigned Location;
        char PaddingBytes[sizeof(char*) + sizeof(unsigned)];
        unsigned Type;
    };

    struct alignas(32) Uniform
    {
        char* Name;
        unsigned NameSize;
        unsigned Location;
        unsigned Type;
        char PaddingBytes[sizeof(char*)];
        int BlockIndex;
    };

    struct ShaderProgram
    {
        Shader Shaders[static_cast<int>(ShaderType::End)];
        Attribute* InAttributes;
        Attribute* OutAttributes;
        Uniform* Uniforms;
        unsigned InAttributesCount;
        unsigned OutAttributesCount;
        unsigned Id;
        unsigned UniformsCount;
    };

    struct alignas(sizeof(void*)) ShaderParams
    {
        const char* FragmentShaderFile;
        const char* VertexShaderFile;
        const char* GeometryShaderFile;
        const char* TesselationShaderFile;
        unsigned short FragSize;
        unsigned short VertSize;
        unsigned short GeomSize;
        unsigned short TessSize;
    };

    void CreateProgram(ShaderProgram*, const ShaderParams*) noexcept;
    void DeleteProgram(ShaderProgram*) noexcept;
    void UseProgram(const ShaderProgram*) noexcept;

    void CreateShader(Shader*) noexcept;

    void DeleteShader(Shader*);

    // ReSharper disable once CppClassCanBeFinal
    struct ShaderProgramTraits // NOLINT(cppcoreguidelines-special-member-functions)
    {
        using CreateShaderFunc = void(*)(ShaderProgram*, const ShaderParams*);
        using UseShaderFunc = void(*)(const ShaderProgram*);
        using DeleteShaderFunc = void(*)(ShaderProgram*);

        ShaderProgramTraits()
        {
            Create = CreateProgram;
            UseFunc = UseProgram;
            Delete = DeleteProgram;
        }

        virtual ~ShaderProgramTraits() = default;

        CreateShaderFunc Create;
        UseShaderFunc UseFunc;
        DeleteShaderFunc Delete;
    };
}
