#pragma once

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
			Vector3D operator*(const Vector3D&) const noexcept;

			Matrix& operator*=(const Matrix&) noexcept;

			Matrix operator+(double) const noexcept;
			Matrix& operator+=(double) noexcept;

			Matrix operator-(double) const noexcept;
			Matrix& operator-=(double) noexcept;

			Matrix operator*(double) const noexcept;
			Matrix& operator*=(double) noexcept;

			Matrix operator/(double) const noexcept;
			Matrix& operator/=(double) noexcept;

			static Vector3D Scale(const Vector3D&, const Vector3D&) noexcept;
			static Vector3D Translate(const Vector3D&, const Vector3D&) noexcept;
			static Vector3D Rotate(double, const Vector3D&, const Vector3D&) noexcept;

			static Matrix LookAt(const Vector3D&, const Vector3D&, const Vector3D&) noexcept;
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

			alignas(sizeof(double)) double Mat[16];

		private:
			[[nodiscard]] __m256d AsSimd256(size_t) const noexcept;
			[[nodiscard]] __m512d AsSimd512(size_t) const noexcept;
		};
	}
}
