#pragma once

namespace Render
{
	namespace Math
	{
		struct Vector3D final
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
				double X, Y, Z, W;

				alignas(sizeof(double)) double Vec[4];
			};
		};
	}
}
