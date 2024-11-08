#pragma once

#if !defined(PREFE_GLM)

namespace Render
{
    namespace Math
    {
        template <class T>
        struct Vector3D final
        {
            static constexpr unsigned Size() noexcept
            {
                return 4;
            }
        };

        template <>
        struct Vector3D<double> final
        {
            Vector3D() noexcept;
            explicit Vector3D(double x, double y, double z, double w = 1) noexcept;

            Vector3D(const Vector3D&) noexcept;
            Vector3D& operator=(const Vector3D&) noexcept;

            Vector3D(Vector3D&&) noexcept;
            Vector3D& operator=(Vector3D&&) noexcept;

            ~Vector3D() = default;

            Vector3D operator+(const Vector3D&) const noexcept;
            Vector3D& operator+=(const Vector3D&) noexcept;

            Vector3D operator-(const Vector3D&) const noexcept;
            Vector3D& operator-=(const Vector3D&) noexcept;

            Vector3D operator*(const Vector3D&) const noexcept;
            Vector3D& operator*=(const Vector3D&) noexcept;

            Vector3D operator+(double) const noexcept;
            Vector3D& operator+=(double) noexcept;

            Vector3D operator-(double) const noexcept;
            Vector3D& operator-=(double) noexcept;

            Vector3D operator*(double) const noexcept;
            Vector3D& operator*=(double) noexcept;

            Vector3D operator/(double) const noexcept;
            Vector3D& operator/=(double) noexcept;

            bool operator==(const Vector3D&) const noexcept;
            bool operator!=(const Vector3D&) const noexcept;


            Vector3D& Normalize() noexcept;

            static Vector3D Cross(const Vector3D&, const Vector3D&) noexcept;
            static double Dot(const Vector3D&, const Vector3D&) noexcept;

            static Vector3D NormalizeW(const Vector3D&) noexcept;

            static Vector3D Once() noexcept;

            [[nodiscard]] double* AsArray() noexcept;

            [[nodiscard]] const double* AsArray() const noexcept;

            friend Vector3D operator+(double, const Vector3D&) noexcept;

            friend Vector3D operator-(double, const Vector3D&) noexcept;

            friend Vector3D operator*(double, const Vector3D&) noexcept;

            friend Vector3D operator/(double, const Vector3D&) noexcept;

            [[nodiscard]] double Length() const noexcept;

            union
            {
                alignas(sizeof(double) * 4) double Vec[4];

                struct
                {
                    double X;
                    double Y;
                    double Z;
                    double W;
                };
            };
        };

        template <>
        struct Vector3D<unsigned> final
        {
            Vector3D() noexcept;
            explicit Vector3D(unsigned x, unsigned y, unsigned z, unsigned w = 1) noexcept;

            Vector3D(const Vector3D&) noexcept;
            Vector3D& operator=(const Vector3D&) noexcept;

            Vector3D(Vector3D&&) noexcept;
            Vector3D& operator=(Vector3D&&) noexcept;

            ~Vector3D() = default;

            Vector3D operator+(const Vector3D&) const noexcept;
            Vector3D& operator+=(const Vector3D&) noexcept;

            Vector3D operator-(const Vector3D&) const noexcept;
            Vector3D& operator-=(const Vector3D&) noexcept;

            Vector3D operator*(const Vector3D&) const noexcept;
            Vector3D& operator*=(const Vector3D&) noexcept;

            Vector3D operator+(unsigned) const noexcept;
            Vector3D& operator+=(unsigned) noexcept;

            Vector3D operator-(unsigned) const noexcept;
            Vector3D& operator-=(unsigned) noexcept;

            Vector3D operator*(unsigned) const noexcept;
            Vector3D& operator*=(unsigned) noexcept;

            Vector3D operator/(unsigned) const noexcept;
            Vector3D& operator/=(unsigned) noexcept;

            bool operator==(const Vector3D&) const noexcept;
            bool operator!=(const Vector3D&) const noexcept;


            Vector3D& Normalize() noexcept;

            static Vector3D Cross(const Vector3D&, const Vector3D&) noexcept;
            static double Dot(const Vector3D&, const Vector3D&) noexcept;

            static Vector3D NormalizeW(const Vector3D&) noexcept;

            static Vector3D Once() noexcept;

            [[nodiscard]] unsigned* AsArray() noexcept;

            [[nodiscard]] const unsigned* AsArray() const noexcept;

            friend Vector3D operator+(unsigned, const Vector3D&) noexcept;

            friend Vector3D operator-(unsigned, const Vector3D&) noexcept;

            friend Vector3D operator*(unsigned, const Vector3D&) noexcept;

            friend Vector3D operator/(unsigned, const Vector3D&) noexcept;

            [[nodiscard]] double Length() const noexcept;

            union
            {
                unsigned X, Y, Z, W;

                alignas(sizeof(unsigned)) unsigned Vec[4];
            };
        };
    }
}

#else

#include <glm/glm.hpp>

namespace Render
{
	namespace Math
	{
		template<class T>
		using Vector3D = glm::vec4;
	}
}
#endif
