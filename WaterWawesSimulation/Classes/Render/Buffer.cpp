#include "Buffer.h"

#include <Classes/Render/glad.h>

namespace Render
{
    void CreateBuffer(Buffer<double>* buf) noexcept
    {
        if (!buf)
        {
            return;
        }

        buf->Buff = nullptr;
        buf->Id = 0;
        buf->RenderType = GL_STATIC_DRAW;
        buf->Size = 0;
        buf->BaseSizeOf = sizeof(VBOTag::Type);
        buf->SizeOf = sizeof(Math::Vector3D<VBOTag::Type>);
        buf->Type = GL_DOUBLE;
        buf->BuffType = GL_VERTEX_ARRAY;
        buf->Mode = GL_TRIANGLES;

        glGenBuffers(1, &buf->Id);
        glBindBuffer(GL_VERTEX_ARRAY, buf->Id);
    }

    void CreateBuffer(Buffer<unsigned>* buf) noexcept
    {
        if (!buf)
        {
            return;
        }

        buf->Buff = nullptr;
        buf->Id = 0;
        buf->RenderType = GL_STATIC_DRAW;
        buf->Size = 0;
        buf->BaseSizeOf = sizeof(IBOTag::Type);
        buf->SizeOf = sizeof(IBOTag::Type*);
        buf->Type = GL_UNSIGNED_INT;
        buf->BuffType = GL_ELEMENT_ARRAY_BUFFER;
        buf->Mode = GL_TRIANGLES;

        glGenBuffers(1, &buf->Id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->Id);
    }


    void CreateBuffer(Buffer<void>* buf) noexcept
    {
        if (!buf)
        {
            return;
        }

        buf->Id = 0;
        buf->BuffType = GL_DOUBLE;
        buf->Normalized = GL_FALSE;
        buf->Stride = sizeof(Math::Vector3D<VBOTag::Type>);
        buf->OffsetPtr = (void*)offsetof(Math::Vector3D<VBOTag::Type>, Vec);
        buf->Location = 0;
        buf->ComponentsCount = Math::Vector3D<void>::Size();

        glGenVertexArrays(1, &buf->Id);
        glBindVertexArray(buf->Id);
    }


    void CreateBuffer(Buffer<void*>* buf) noexcept
    {
        if (!buf)
        {
            return;
        }

        buf->Id = 0;
        buf->RenderType = GL_STATIC_DRAW;
        buf->Type = GL_UNIFORM_BLOCK;

        buf->BlockIndex = glGetUniformBlockIndex(buf->ShaderProgramHandle, buf->BlockName);
        glGetActiveUniformBlockiv(buf->ShaderProgramHandle, buf->BlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE,
                                  &buf->MemBlockSize);
        buf->MemBlockBuff = new(std::nothrow)char[buf->MemBlockSize];

        glGetUniformIndices(buf->ShaderProgramHandle, buf->UniformsSize, buf->UniformsNames, buf->UniformsIndexes);
        glGetActiveUniformsiv(buf->ShaderProgramHandle, buf->UniformsSize, buf->UniformsIndexes, GL_UNIFORM_OFFSET,
                              buf->UniformsOffset);

        for (int i = 0; i < buf->UniformsSize; ++i)
        {
            _memccpy(buf->MemBlockBuff + buf->UniformsOffset[i], buf->Buff[i], buf->BuffSizeof[i],
                     buf->BuffSizes[i]);
        }

        glGenBuffers(1, &buf->Id);
        glBindBuffer(buf->Type, buf->Id);
    }


    void BindBuffer(const Buffer<double>* buf) noexcept
    {
        glBindBuffer(buf->BuffType, buf->Id);
        glBufferData(buf->BuffType, static_cast<GLsizeiptr>(buf->Size * buf->SizeOf), buf->Buff, buf->RenderType);
    }


    void BindBuffer(const Buffer<unsigned>* buf) noexcept
    {
        glBindBuffer(buf->BuffType, buf->Id);
        glBufferData(buf->BuffType, static_cast<GLsizeiptr>(buf->Size * buf->BaseSizeOf), buf->Buff, buf->RenderType);
    }


    void BindBuffer(const Buffer<void>* buf) noexcept
    {
        glBindVertexArray(buf->Id);

        glEnableVertexAttribArray(buf->Location);
        glVertexAttribPointer(buf->Location, buf->ComponentsCount, buf->BuffType, buf->Normalized, buf->Stride,
                              buf->OffsetPtr);
    }


    void BindBuffer(const Buffer<void*>* buf) noexcept
    {
        glBindBuffer(buf->Type, buf->Id);
        glBufferData(buf->Type, buf->MemBlockSize, buf->MemBlockBuff, buf->RenderType);
        glBindBufferBase(buf->Type, buf->Location, buf->Id);
    }

    void UnBindBuffer(const Buffer<void*>*) noexcept
    {
    }


    void UnBindBuffer(const Buffer<double>*) noexcept
    {
    }


    void UnBindBuffer(const Buffer<unsigned>*) noexcept
    {
    }


    void UnBindBuffer(const Buffer<void>*) noexcept
    {
        glBindVertexArray(0);
    }


    void DrawBuffer(const Buffer<unsigned>* buf) noexcept
    {
        glDrawElements(buf->Mode, buf->Size, buf->Type, nullptr);
    }

    void DrawBuffer(const Buffer<void*>*) noexcept
    {
    }

    void DrawBuffer(const Buffer<double>*) noexcept
    {
    }

    void DrawBuffer(const Buffer<void>*) noexcept
    {
    }

    void DeleteBuffer(Buffer<double>* buf) noexcept
    {
        delete[] buf->Buff;
        buf->Buff = nullptr;
        buf->Id = 0;
        buf->Mode = 0;
        buf->Size = 0;
        buf->Type = 0;
        buf->BuffType = 0;
        buf->RenderType = 0;
        buf->SizeOf = 0;
        buf->BaseSizeOf = 0;
    }

    void DeleteBuffer(Buffer<unsigned>* buf) noexcept
    {
        delete[] buf->Buff;
        buf->Buff = nullptr;
        buf->Id = 0;
        buf->Mode = 0;
        buf->Size = 0;
        buf->Type = 0;
        buf->BuffType = 0;
        buf->RenderType = 0;
        buf->SizeOf = 0;
        buf->BaseSizeOf = 0;
    }


    void DeleteBuffer(Buffer<void>* buf) noexcept
    {
        buf->Id = 0;
        buf->Location = 0;
        buf->Normalized = 0;
        buf->Stride = 0;
        buf->BuffType = 0;
        buf->ComponentsCount = 0;
        buf->OffsetPtr = nullptr;
    }

    void DeleteBuffer(Buffer<void*>* buf) noexcept
    {
        delete[] buf->UniformsNamesSize;
        delete[] buf->MemBlockBuff;
        delete[] buf->UniformsOffset;
        delete[] buf->UniformsIndexes;
        delete[] buf->BlockName;
        delete[] buf->BuffSizes;
        delete[] buf->BuffSizeof;
        for (int i = 0; i < buf->UniformsSize; ++i)
        {
            delete[] buf->UniformsNames[i];
            delete[] static_cast<double*>(buf->Buff[i]);
        }

        delete[] buf->Buff;

        delete[] buf->UniformsNames;
        buf->Id = 0;
        buf->Location = 0;
        buf->Type = 0;
        buf->Buff = nullptr;
        buf->BlockIndex = 0;
        buf->BuffSize = 0;
        buf->BlockName = nullptr;
        buf->BuffSizeof = nullptr;
        buf->BuffSizes = nullptr;
        buf->RenderType = 0;
        buf->UniformsIndexes = nullptr;
        buf->UniformsNames = nullptr;
        buf->UniformsOffset = nullptr;
        buf->UniformsSize = 0;
        buf->BlockNameSize = 0;
        buf->MemBlockBuff = nullptr;
        buf->MemBlockSize = 0;
        buf->ShaderProgramHandle = 0;
        buf->UniformsNamesSize = nullptr;
    }


    ObjectBuffer::ObjectBuffer() : resource_(new(std::nothrow) std::pmr::unsynchronized_pool_resource()),
                                   alloc_(resource_)
    {
        Init();
    }

    void ObjectBuffer::Init() noexcept
    {
        vao_.Id = 0;
        vao_.BuffType = 0;
        vao_.ComponentsCount = 0;
        vao_.Location = 0;
        vao_.Normalized = 0;
        vao_.OffsetPtr = nullptr;
        vao_.Stride = 0;

        ibo_.Id = 0;
        ibo_.BuffType = 0;
        ibo_.BaseSizeOf = 0;
        ibo_.Buff = nullptr;
        ibo_.Mode = 0;
        ibo_.RenderType = 0;
        ibo_.Size = 0;
        ibo_.SizeOf = 0;
        ibo_.Type = 0;

        vbo_.Id = 0;
        vbo_.Mode = 0;
        vbo_.RenderType = 0;
        vbo_.Size = 0;
        vbo_.SizeOf = 0;
        vbo_.Type = 0;
        vbo_.BaseSizeOf = 0;
        vbo_.BuffType = 0;
        vbo_.Buff = nullptr;
    }


    ObjectBuffer::ObjectBuffer(ObjectBuffer&& other) noexcept : ObjectBuffer()
    {
        vao_.Id = other.vao_.Id;
        vao_.BuffType = other.vao_.BuffType;
        vao_.ComponentsCount = other.vao_.ComponentsCount;
        vao_.Location = other.vao_.Location;
        vao_.Normalized = other.vao_.Normalized;
        vao_.OffsetPtr = other.vao_.OffsetPtr;
        vao_.Stride = other.vao_.Stride;

        ibo_.Id = other.ibo_.Id;
        ibo_.BuffType = other.ibo_.BuffType;
        ibo_.BaseSizeOf = other.ibo_.BaseSizeOf;
        ibo_.Mode = other.ibo_.Mode;
        ibo_.RenderType = other.ibo_.Mode;
        ibo_.SizeOf = other.ibo_.SizeOf;
        ibo_.Type = other.ibo_.Type;

        vbo_.Id = other.vbo_.Id;
        vbo_.Mode = other.vbo_.Mode;
        vbo_.RenderType = other.vbo_.RenderType;
        vbo_.SizeOf = other.vbo_.SizeOf;
        vbo_.Type = other.vbo_.Type;
        vbo_.BaseSizeOf = other.vbo_.BaseSizeOf;
        vbo_.BuffType = other.vbo_.BuffType;

        other.Init();
    }

    ObjectBuffer::ObjectBuffer(const ObjectBuffer& other) : ObjectBuffer()
    {
        vao_.Id = other.vao_.Id;
        vao_.BuffType = other.vao_.BuffType;
        vao_.ComponentsCount = other.vao_.ComponentsCount;
        vao_.Location = other.vao_.Location;
        vao_.Normalized = other.vao_.Normalized;
        vao_.OffsetPtr = other.vao_.OffsetPtr;
        vao_.Stride = other.vao_.Stride;

        ibo_.Id = other.ibo_.Id;
        ibo_.BuffType = other.ibo_.BuffType;
        ibo_.BaseSizeOf = other.ibo_.BaseSizeOf;
        ibo_.Mode = other.ibo_.Mode;
        ibo_.RenderType = other.ibo_.Mode;
        ibo_.SizeOf = other.ibo_.SizeOf;
        ibo_.Type = other.ibo_.Type;

        vbo_.Id = other.vbo_.Id;
        vbo_.Mode = other.vbo_.Mode;
        vbo_.RenderType = other.vbo_.RenderType;
        vbo_.SizeOf = other.vbo_.SizeOf;
        vbo_.Type = other.vbo_.Type;
        vbo_.BaseSizeOf = other.vbo_.BaseSizeOf;
        vbo_.BuffType = other.vbo_.BuffType;
    }


    ObjectBuffer& ObjectBuffer::operator=(ObjectBuffer&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        vao_.Id = other.vao_.Id;
        vao_.BuffType = other.vao_.BuffType;
        vao_.ComponentsCount = other.vao_.ComponentsCount;
        vao_.Location = other.vao_.Location;
        vao_.Normalized = other.vao_.Normalized;
        vao_.OffsetPtr = other.vao_.OffsetPtr;
        vao_.Stride = other.vao_.Stride;

        ibo_.Id = other.ibo_.Id;
        ibo_.BuffType = other.ibo_.BuffType;
        ibo_.BaseSizeOf = other.ibo_.BaseSizeOf;
        ibo_.Mode = other.ibo_.Mode;
        ibo_.RenderType = other.ibo_.Mode;
        ibo_.SizeOf = other.ibo_.SizeOf;
        ibo_.Type = other.ibo_.Type;


        vbo_.Id = other.vbo_.Id;
        vbo_.Mode = other.vbo_.Mode;
        vbo_.RenderType = other.vbo_.RenderType;
        vbo_.SizeOf = other.vbo_.SizeOf;
        vbo_.Type = other.vbo_.Type;
        vbo_.BaseSizeOf = other.vbo_.BaseSizeOf;
        vbo_.BuffType = other.vbo_.BuffType;
        other.Init();

        return *this;
    }


    ObjectBuffer& ObjectBuffer::operator=(const ObjectBuffer& other)
    {
        if (this == &other)
        {
            return *this;
        }

        vao_.Id = other.vao_.Id;
        vao_.BuffType = other.vao_.BuffType;
        vao_.ComponentsCount = other.vao_.ComponentsCount;
        vao_.Location = other.vao_.Location;
        vao_.Normalized = other.vao_.Normalized;
        vao_.OffsetPtr = other.vao_.OffsetPtr;
        vao_.Stride = other.vao_.Stride;

        ibo_.Id = other.ibo_.Id;
        ibo_.BuffType = other.ibo_.BuffType;
        ibo_.BaseSizeOf = other.ibo_.BaseSizeOf;
        ibo_.Mode = other.ibo_.Mode;
        ibo_.RenderType = other.ibo_.Mode;
        ibo_.SizeOf = other.ibo_.SizeOf;
        ibo_.Type = other.ibo_.Type;

        vbo_.Id = other.vbo_.Id;
        vbo_.Mode = other.vbo_.Mode;
        vbo_.RenderType = other.vbo_.RenderType;
        vbo_.SizeOf = other.vbo_.SizeOf;
        vbo_.Type = other.vbo_.Type;
        vbo_.BaseSizeOf = other.vbo_.BaseSizeOf;
        vbo_.BuffType = other.vbo_.BuffType;

        return *this;
    }


    ObjectBuffer::~ObjectBuffer()
    {
        using ReboundAllocatorIbo = std::allocator_traits<decltype(alloc_)>::rebind_alloc<
            std::pmr::polymorphic_allocator<BufferTraits<unsigned>::BuffType>>::value_type;
        using ReboundAllocatorVbo = std::allocator_traits<decltype(alloc_)>::rebind_alloc<
            std::pmr::polymorphic_allocator<BufferTraits<double>::BuffType>>::value_type;

        ReboundAllocatorIbo iboAlloc = alloc_;

        ReboundAllocatorVbo vboAlloc = alloc_;

        iboAlloc.deallocate(ibo_.Buff, ibo_.Size);
        vboAlloc.deallocate(vbo_.Buff, vbo_.Size);

        delete resource_;
        resource_ = nullptr;

        vao_.Id = 0;
        vao_.BuffType = 0;
        vao_.ComponentsCount = 0;
        vao_.Location = 0;
        vao_.Normalized = 0;
        vao_.OffsetPtr = nullptr;
        vao_.Stride = 0;

        ibo_.Id = 0;
        ibo_.BuffType = 0;
        ibo_.BaseSizeOf = 0;
        ibo_.Buff = nullptr;
        ibo_.Mode = 0;
        ibo_.RenderType = 0;
        ibo_.Size = 0;
        ibo_.SizeOf = 0;
        ibo_.Type = 0;

        vbo_.Id = 0;
        vbo_.Mode = 0;
        vbo_.RenderType = 0;
        vbo_.Size = 0;
        vbo_.SizeOf = 0;
        vbo_.Type = 0;
        vbo_.BaseSizeOf = 0;
        vbo_.BuffType = 0;
        vbo_.Buff = nullptr;
    }
}
