// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken
#pragma once

#include <Classes/Render/Math/Vector3D.h>
#include <memory_resource>

namespace Render
{
    // ReSharper disable once CppInconsistentNaming
    struct VBOTag
    {
        using Type = double;
        using BuffType = Math::Vector3D<double>;
    };

    // ReSharper disable once CppInconsistentNaming
    struct IBOTag
    {
        using Type = unsigned;
        using BuffType = unsigned;
    };

    // ReSharper disable once CppInconsistentNaming
    struct VAOTag
    {
        using Type = void;
        using BuffType = void;
    };

    struct UniformBlockTag
    {
        struct Self
        {
        };

        using Type = Self;
        using BuffType = void*;
    };

    template <class T>
    struct Buffer
    {
    };

    template <>
    struct alignas(16) Buffer<void*>
    {
        using Tag = UniformBlockTag;

        Tag::BuffType* Buff;
        char* MemBlockBuff;
        char** UniformsNames;
        int* UniformsOffset;
        char* BlockName;
        unsigned* BuffSizes;
        int* BuffSizeof;
        unsigned* UniformsNamesSize;
        unsigned* UniformsIndexes;
        int UniformsSize;
        unsigned BuffSize;
        unsigned BlockNameSize;
        int MemBlockSize;
        unsigned Type;
        unsigned RenderType;
        unsigned Id;
        unsigned BlockIndex;
        unsigned ShaderProgramHandle;
        unsigned Location;
    };

    template <>
    struct alignas(sizeof(unsigned*)) Buffer<unsigned>
    {
        using Tag = IBOTag;

        Tag::BuffType* Buff;
        size_t BaseSizeOf;
        size_t SizeOf;
        unsigned Type;
        int Size;
        unsigned RenderType;
        unsigned Mode;
        unsigned BuffType;
        unsigned Id;
    };

    template <>
    struct alignas(sizeof(double*)) Buffer<double>
    {
        using Tag = VBOTag;

        Tag::BuffType* Buff;
        size_t BaseSizeOf;
        size_t SizeOf;
        unsigned Type;
        int Size;
        unsigned RenderType;
        unsigned Mode;
        unsigned BuffType;
        unsigned Id;
    };

    template <>
    struct alignas(sizeof(void*)) Buffer<void>
    {
        using Tag = VAOTag;

        void* OffsetPtr;
        unsigned BuffType;
        unsigned Location;
        int ComponentsCount;
        int Stride;
        unsigned Id;
        char PaddingBytes[sizeof(unsigned) - sizeof(unsigned char)];
        unsigned char Normalized;
    };

    __forceinline void CreateBuffer(Buffer<double>*) noexcept;
    __forceinline void CreateBuffer(Buffer<unsigned>*) noexcept;
    __forceinline void CreateBuffer(Buffer<void>*) noexcept;
    __forceinline void CreateBuffer(Buffer<void*>*) noexcept;

    __forceinline void BindBuffer(const Buffer<unsigned>*) noexcept;
    __forceinline void BindBuffer(const Buffer<double>*) noexcept;
    __forceinline void BindBuffer(const Buffer<void>*) noexcept;
    __forceinline void BindBuffer(const Buffer<void*>*) noexcept;

    __forceinline void UnBindBuffer(const Buffer<double>*) noexcept;
    __forceinline void UnBindBuffer(const Buffer<unsigned>*) noexcept;
    __forceinline void UnBindBuffer(const Buffer<void>*) noexcept;
    __forceinline void UnBindBuffer(const Buffer<void*>*) noexcept;

    __forceinline void DrawBuffer(const Buffer<unsigned>*) noexcept;
    __forceinline void DrawBuffer(const Buffer<double>*) noexcept;
    __forceinline void DrawBuffer(const Buffer<void>*) noexcept;
    __forceinline void DrawBuffer(const Buffer<void*>*) noexcept;

    __forceinline void DeleteBuffer(Buffer<unsigned>*) noexcept;
    __forceinline void DeleteBuffer(Buffer<double>*) noexcept;
    __forceinline void DeleteBuffer(Buffer<void>*) noexcept;
    __forceinline void DeleteBuffer(Buffer<void*>*) noexcept;


    template <class T>
    // ReSharper disable once CppClassCanBeFinal
    struct BufferTraits // NOLINT(cppcoreguidelines-special-member-functions)
    {
        using Tag = typename Buffer<T>::Tag;
        using Type = typename Tag::Type;
        using BuffType = typename Tag::BuffType;
        using BufferType = Buffer<T>;

        using CreateBufferFunc = void(*)(Buffer<T>*);
        using BindBufferFunc = void(*)(const Buffer<T>*);
        using UnbindBufferFunc = void(*)(const Buffer<T>*);
        using DrawBufferFunc = void(*)(const Buffer<T>*);
        using DeleteBufferFunc = void(*)(Buffer<T>*);

        BufferTraits()
        {
            Create = CreateBuffer;
            Bind = BindBuffer;
            Unbind = UnBindBuffer;
            Draw = DrawBuffer;
            Delete = DeleteBuffer;
        }

        virtual ~BufferTraits() = default;

        CreateBufferFunc Create;
        BindBufferFunc Bind;
        UnbindBufferFunc Unbind;
        DrawBufferFunc Draw;
        DeleteBufferFunc Delete;
    };

    class ObjectBuffer final
    {
    public:
        ObjectBuffer();

        ObjectBuffer(const ObjectBuffer&);
        ObjectBuffer& operator=(const ObjectBuffer&);

        ObjectBuffer(ObjectBuffer&&) noexcept;
        ObjectBuffer& operator=(ObjectBuffer&&) noexcept;

        ~ObjectBuffer();

        template <class T>
        void AllocateBuffer(const int size, void* mem = nullptr) noexcept
        {
            using Basic = std::conditional_t<std::is_same_v<T, VAOTag> || std::is_same_v<T, IBOTag> || std::is_same_v<
                                                 T, VBOTag>, typename T::Type, T>;
            using TBufferTraits = BufferTraits<Basic>;
            using Tag = typename TBufferTraits::Tag;
            using AllocType = decltype(alloc_);
            using BaseBuffType = typename TBufferTraits::BuffType;
            using BufferType = typename TBufferTraits::BufferType;

            BufferType buffer = GetBuffer<Tag>();

            if (buffer.Size >= size)
            {
                return;
            }

            using ReboundAllocator = typename std::allocator_traits<AllocType>::rebind_alloc<
                std::pmr::polymorphic_allocator<BaseBuffType>>::value_type;

            ReboundAllocator alloc = alloc_;

            void* ptr = alloc.allocate(size);
            if (ptr == nullptr)
            {
                return;
            }

            alloc.deallocate(buffer.Buff, buffer.Size);

            buffer.Buff = static_cast<BaseBuffType*>(ptr);
            buffer.Size = size;
            if (mem)
            {
                _memccpy(buffer.Buff, mem, sizeof(BaseBuffType), size);
            }
        }

        template <class T>
        void CreateBuffer(BufferTraits<T>* traits) noexcept
        {
            using Basic = std::conditional_t<std::is_same_v<T, VAOTag> || std::is_same_v<T, IBOTag> || std::is_same_v<
                                                 T, VBOTag>, typename T::Type, T>;
            traits->Create(std::addressof(GetBuffer<typename BufferTraits<Basic>::Tag>()));
        }

        template <class T>
        void BindBuffer(BufferTraits<T>* traits) noexcept
        {
            using Basic = std::conditional_t<std::is_same_v<T, VAOTag> || std::is_same_v<T, IBOTag> || std::is_same_v<
                                                 T, VBOTag>, typename T::Type, T>;
            traits->Bind(std::addressof(GetBuffer<typename BufferTraits<Basic>::Tag>()));
        }

        template <class T>
        void DrawBuffer(BufferTraits<T>* traits) noexcept
        {
            using Basic = std::conditional_t<std::is_same_v<T, VAOTag> || std::is_same_v<T, IBOTag> || std::is_same_v<
                                                 T, VBOTag>, typename T::Type, T>;
            traits->Draw(std::addressof(GetBuffer<typename BufferTraits<Basic>::Tag>()));
        }

        template <class T>
        void UnbindBuffer(BufferTraits<T>* traits) noexcept
        {
            using Basic = std::conditional_t<std::is_same_v<T, VAOTag> || std::is_same_v<T, IBOTag> || std::is_same_v<
                                                 T, VBOTag>, typename T::Type, T>;
            traits->Unbind(std::addressof(GetBuffer<typename BufferTraits<Basic>::Tag>()));
        }

        template <class Tag>
        Buffer<typename Tag::Type>& GetBuffer() & noexcept
        {
            static_assert(std::is_same_v<Tag, VAOTag> || std::is_same_v<Tag, IBOTag> || std::is_same_v<Tag, VBOTag>);
            if constexpr (std::is_same_v<Tag, VAOTag>)
            {
                return vao_;
            }
            if constexpr (std::is_same_v<Tag, IBOTag>)
            {
                return ibo_;
            }
            return vbo_;
        }

        template <class Tag>
        const Buffer<typename Tag::Type>& GetBuffer() const & noexcept
        {
            static_assert(std::is_same_v<Tag, VAOTag> || std::is_same_v<Tag, IBOTag> || std::is_same_v<Tag, VBOTag>);
            if constexpr (std::is_same_v<Tag, VAOTag>)
            {
                return vao_;
            }
            if constexpr (std::is_same_v<Tag, IBOTag>)
            {
                return ibo_;
            }
            return vbo_;
        }

    private:
        void Init() noexcept;

    protected:
        std::pmr::memory_resource* resource_;
        std::pmr::polymorphic_allocator<> alloc_;
        Buffer<void> vao_;
        Buffer<unsigned> ibo_;
        Buffer<double> vbo_;
    };
}
