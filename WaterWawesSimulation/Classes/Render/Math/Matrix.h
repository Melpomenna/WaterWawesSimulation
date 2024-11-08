#pragma once

#if !defined(PREFE_GLM)

#include <immintrin.h>
#include <Classes/Render/Math/Vector3D.h>

namespace Render
{
    namespace Math
    {
        struct Matrix final
        {
            Matrix() noexcept;
            explicit Matrix(double);
            explicit Matrix(double, double, double, double,
                            double, double, double, double,
                            double, double, double, double,
                            double, double, double, double) noexcept;

            Matrix(const Matrix&) noexcept;
            Matrix& operator=(const Matrix&) noexcept;

            ~Matrix() = default;

            Matrix(Matrix&&) noexcept;
            Matrix& operator=(Matrix&&) noexcept;

            Matrix operator+(const Matrix&) const noexcept;
            Matrix& operator+=(const Matrix&) noexcept;

            Matrix operator-(const Matrix&) const noexcept;
            Matrix& operator-=(const Matrix&) noexcept;

            Matrix operator*(const Matrix&) const noexcept;
            Vector3D<double> operator*(const Vector3D<double>&) const noexcept;

            Matrix& operator*=(const Matrix&) noexcept;

            Matrix operator+(double) const noexcept;
            Matrix& operator+=(double) noexcept;

            Matrix operator-(double) const noexcept;
            Matrix& operator-=(double) noexcept;

            Matrix operator*(double) const noexcept;
            Matrix& operator*=(double) noexcept;

            Matrix operator/(double) const noexcept;
            Matrix& operator/=(double) noexcept;

            static Vector3D<double> Scale(const Vector3D<double>&, const Vector3D<double>&) noexcept;
            static Vector3D<double> Translate(const Vector3D<double>&, const Vector3D<double>&) noexcept;
            static Vector3D<double> Rotate(double, const Vector3D<double>&, const Vector3D<double>&) noexcept;

            static Matrix LookAt(const Vector3D<double>&, const Vector3D<double>&, const Vector3D<double>&) noexcept;
            static Matrix Identity() noexcept;
            static Matrix Identity(double) noexcept;
            static Matrix Ortho(double, double, double, double, double, double) noexcept;
            static Matrix Frustum(double, double, double, double, double, double) noexcept;
            static Matrix Perspective(double, double, double, double) noexcept;
            static Matrix Once() noexcept;
            static Matrix Once(double) noexcept;

            [[nodiscard]] double* AsArray() noexcept;

            [[nodiscard]] const double* AsArray() const noexcept;

            friend Matrix operator+(double, const Matrix&) noexcept;

            friend Matrix operator-(double, const Matrix&) noexcept;

            friend Matrix operator*(double, const Matrix&) noexcept;

            friend Matrix operator/(double, const Matrix&) noexcept;

            double& operator()(size_t, size_t) & noexcept;
            const double& operator()(size_t, size_t) const & noexcept;

            double* operator[](size_t) & noexcept;
            const double* operator[](size_t) const & noexcept;

            alignas(16) double Mat[16];

            [[nodiscard]] size_t Size() const noexcept;

        private:
            [[nodiscard]] __m256d AsSimd256(size_t) const noexcept;
        };
    }
}

#else
#include <glm/glm.hpp>

namespace Render
{
	namespace Math
	{
		using Matrix = glm::mat4x4;
	}
}
#endif
