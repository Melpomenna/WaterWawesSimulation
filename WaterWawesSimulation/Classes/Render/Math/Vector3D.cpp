#include "Vector3D.h"

#include <cmath>

#include <immintrin.h>

namespace Render
{
	namespace Math
	{
		Vector3D::Vector3D() noexcept : Vec{0, 0, 0, 1}
		{
		}

		Vector3D::Vector3D(const double x, const double y, const double z, const double w) noexcept : Vec{
			x, y, z, w
		}
		{
		}

		Vector3D::Vector3D(const Vector3D& other) noexcept
		{
			const auto ov = _mm256_loadu_pd(other.Vec);
			_mm256_stream_pd(Vec, ov);
		}

		Vector3D& Vector3D::operator=(const Vector3D& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			const auto ov = _mm256_loadu_pd(other.Vec);
			_mm256_stream_pd(Vec, ov);

			return *this;
		}

		Vector3D::Vector3D(Vector3D&& other) noexcept
		{
			const auto ov = _mm256_loadu_pd(other.Vec);
			_mm256_stream_pd(Vec, ov);
			_mm256_stream_pd(other.Vec, _mm256_set1_pd(0));
		}

		Vector3D& Vector3D::operator=(Vector3D&& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			const auto ov = _mm256_loadu_pd(other.Vec);
			_mm256_stream_pd(Vec, ov);
			_mm256_stream_pd(other.Vec, _mm256_set1_pd(0));

			return *this;
		}

		Vector3D Vector3D::operator+(const Vector3D& other) const noexcept
		{
			const auto ov = _mm256_div_pd(_mm256_loadu_pd(other.Vec), _mm256_set1_pd(other.W));
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			Vector3D res;
			auto rv = _mm256_loadu_pd(res.Vec);
			constexpr __mmask8 mask = 0b0111;
			rv = _mm256_mask_add_pd(rv, mask, ov, sv);
			_mm256_stream_pd(res.Vec, rv);

			return res;
		}

		Vector3D& Vector3D::operator+=(const Vector3D& other) noexcept
		{
			const auto ov = _mm256_div_pd(_mm256_loadu_pd(other.Vec), _mm256_set1_pd(other.W));
			auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			constexpr __mmask8 mask = 0b0111;
			sv = _mm256_mask_add_pd(sv, mask, ov, sv);
			_mm256_stream_pd(Vec, sv);
			W = 1;

			return *this;
		}

		Vector3D Vector3D::operator-(const Vector3D& other) const noexcept
		{
			const auto ov = _mm256_div_pd(_mm256_loadu_pd(other.Vec), _mm256_set1_pd(other.W));
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			Vector3D res;
			auto rv = _mm256_loadu_pd(res.Vec);
			constexpr __mmask8 mask = 0b0111;
			rv = _mm256_mask_sub_pd(rv, mask, sv, ov);
			_mm256_stream_pd(res.Vec, rv);

			return res;
		}

		Vector3D& Vector3D::operator-=(const Vector3D& other) noexcept
		{
			const auto ov = _mm256_div_pd(_mm256_loadu_pd(other.Vec), _mm256_set1_pd(other.W));
			auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			constexpr __mmask8 mask = 0b0111;
			sv = _mm256_mask_sub_pd(sv, mask, sv, ov);
			_mm256_stream_pd(Vec, sv);
			W = 1;

			return *this;
		}

		Vector3D Vector3D::operator*(const Vector3D& other) const noexcept
		{
			constexpr __mmask8 mask = 0b0111;
			const auto v1 = _mm256_div_pd(_mm256_set_pd(X, Z, X, W), _mm256_set1_pd(W));
			const auto v2 = _mm256_div_pd(_mm256_set_pd(Z, X, Y, W), _mm256_set1_pd(W));

			const auto ov1 = _mm256_div_pd(_mm256_set_pd(other.Z, other.X, other.Y, other.W), _mm256_set1_pd(other.W));
			const auto ov2 = _mm256_div_pd(_mm256_set_pd(other.Y, other.Z, other.X, other.W), _mm256_set1_pd(other.W));

			Vector3D res;
			_mm256_stream_pd(res.Vec, _mm256_mask_sub_pd(_mm256_loadu_pd(res.Vec), mask,
			                                             _mm256_mul_pd(v1, ov1),
			                                             _mm256_mul_pd(v2, ov2)));


			return res;
		}

		Vector3D& Vector3D::operator*=(const Vector3D& other) noexcept
		{
			constexpr __mmask8 mask = 0b0111;
			const auto v1 = _mm256_div_pd(_mm256_set_pd(X, Z, X, W), _mm256_set1_pd(W));
			const auto v2 = _mm256_div_pd(_mm256_set_pd(Z, X, Y, W), _mm256_set1_pd(W));

			const auto ov1 = _mm256_div_pd(_mm256_set_pd(other.Z, other.X, other.Y, other.W), _mm256_set1_pd(other.W));
			const auto ov2 = _mm256_div_pd(_mm256_set_pd(other.Y, other.Z, other.X, other.W), _mm256_set1_pd(other.W));

			_mm256_stream_pd(Vec, _mm256_mask_sub_pd(_mm256_loadu_pd(Vec), mask,
			                                         _mm256_mul_pd(v1, ov1),
			                                         _mm256_mul_pd(v2, ov2)));
			W = 1;


			return *this;
		}

		Vector3D Vector3D::operator+(const double value) const noexcept
		{
			const auto ov = _mm256_set1_pd(value);
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			Vector3D res;
			const auto rv = _mm256_loadu_pd(res.Vec);
			constexpr __mmask8 mask = 0b0111;
			_mm256_stream_pd(res.Vec, _mm256_mask_add_pd(rv, mask, ov, sv));

			return res;
		}

		Vector3D& Vector3D::operator+=(const double value) noexcept
		{
			const auto ov = _mm256_set1_pd(value);
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));

			const auto rv = _mm256_loadu_pd(Vec);
			constexpr __mmask8 mask = 0b0111;
			_mm256_stream_pd(Vec, _mm256_mask_add_pd(rv, mask, ov, sv));
			W = 1;

			return *this;
		}

		Vector3D Vector3D::operator-(const double value) const noexcept
		{
			const auto ov = _mm256_set1_pd(value);
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			Vector3D res;
			auto rv = _mm256_loadu_pd(res.Vec);
			constexpr __mmask8 mask = 0b0111;
			rv = _mm256_mask_sub_pd(rv, mask, sv, ov);
			_mm256_stream_pd(res.Vec, rv);

			return res;
		}

		Vector3D& Vector3D::operator-=(const double value) noexcept
		{
			const auto ov = _mm256_set1_pd(value);
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));

			auto rv = _mm256_loadu_pd(Vec);
			constexpr __mmask8 mask = 0b0111;
			rv = _mm256_mask_sub_pd(rv, mask, sv, ov);
			_mm256_stream_pd(Vec, rv);
			W = 1;

			return *this;
		}

		Vector3D Vector3D::operator*(const double value) const noexcept
		{
			const auto ov = _mm256_set1_pd(value);
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			Vector3D res;
			auto rv = _mm256_loadu_pd(res.Vec);
			constexpr __mmask8 mask = 0b0111;
			rv = _mm256_mask_mul_pd(rv, mask, ov, sv);
			_mm256_stream_pd(res.Vec, rv);

			return res;
		}

		Vector3D& Vector3D::operator*=(const double value) noexcept
		{
			const auto ov = _mm256_set1_pd(value);
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));

			auto rv = _mm256_loadu_pd(Vec);
			constexpr __mmask8 mask = 0b0111;
			rv = _mm256_mask_mul_pd(rv, mask, ov, sv);
			_mm256_stream_pd(Vec, rv);
			W = 1;

			return *this;
		}

		Vector3D Vector3D::operator/(const double value) const noexcept
		{
			const auto ov = _mm256_set1_pd(value);
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			Vector3D res;
			auto rv = _mm256_loadu_pd(res.Vec);
			constexpr __mmask8 mask = 0b0111;
			rv = _mm256_mask_div_pd(rv, mask, ov, sv);
			_mm256_stream_pd(res.Vec, rv);

			return res;
		}

		Vector3D& Vector3D::operator/=(const double value) noexcept
		{
			const auto ov = _mm256_set1_pd(value);
			const auto sv = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));

			auto rv = _mm256_loadu_pd(Vec);
			constexpr __mmask8 mask = 0b0111;
			rv = _mm256_mask_div_pd(rv, mask, ov, sv);
			_mm256_stream_pd(Vec, rv);
			W = 1;

			return *this;
		}

		bool Vector3D::operator==(const Vector3D& other) const noexcept
		{
			return std::abs(X - other.X + Y - other.Y + Z - other.Z + W - other.W) <= 1E-6;
		}

		bool Vector3D::operator!=(const Vector3D& other) const noexcept
		{
			return !(*this == other);
		}


		Vector3D operator+(const double value, const Vector3D& vec) noexcept
		{
			return vec + value;
		}

		Vector3D operator-(const double value, const Vector3D& vec) noexcept
		{
			return vec - value;
		}

		Vector3D operator*(const double value, const Vector3D& vec) noexcept
		{
			return vec * value;
		}

		Vector3D operator/(const double value, const Vector3D& vec) noexcept
		{
			return vec / value;
		}

		double Vector3D::Length() const noexcept
		{
			alignas(sizeof(double)) double out[4] = {0, 0, 0, 0};

			const auto kVX = _mm256_mul_pd(_mm256_set1_pd(X), _mm256_set1_pd(X));
			const auto kVY = _mm256_mul_pd(_mm256_set1_pd(Y), _mm256_set1_pd(Y));
			const auto kVZ = _mm256_mul_pd(_mm256_set1_pd(Z), _mm256_set1_pd(Z));

			_mm256_storeu_pd(
				out, _mm256_sqrt_pd(_mm256_div_pd(_mm256_add_pd(_mm256_add_pd(kVX, kVY), kVZ), _mm256_set1_pd(W))));

			return out[0];
		}

		const double* Vector3D::AsArray() const noexcept
		{
			return Vec;
		}

		double* Vector3D::AsArray() noexcept
		{
			return Vec;
		}

		Vector3D Vector3D::Once() noexcept
		{
			return Vector3D{1, 1, 1};
		}

		Vector3D& Vector3D::Normalize() noexcept
		{
			const auto v = _mm256_div_pd(_mm256_loadu_pd(Vec), _mm256_set1_pd(W));
			_mm256_stream_pd(Vec, _mm256_mask_div_pd(v, 0b0111, v, _mm256_set1_pd(Length())));
			W = 1;
			return *this;
		}

		Vector3D Vector3D::Cross(const Vector3D& x, const Vector3D& y) noexcept
		{
			const auto vx1 = _mm256_div_pd(_mm256_set_pd(x.Y, x.Z, x.X, x.W), _mm256_set1_pd(x.W));
			const auto vy1 = _mm256_div_pd(_mm256_set_pd(y.Y, y.Z, y.X, y.W), _mm256_set1_pd(y.W));

			const auto vx2 = _mm256_div_pd(_mm256_set_pd(x.Z, x.X, x.Y, x.W), _mm256_set1_pd(x.W));
			const auto vy2 = _mm256_div_pd(_mm256_set_pd(y.Z, y.X, y.Y, y.W), _mm256_set1_pd(y.W));

			Vector3D res;

			_mm256_stream_pd(res.Vec, _mm256_mask_sub_pd(_mm256_loadu_pd(res.Vec), 0b0111, _mm256_mul_pd(vx1, vy2),
			                                             _mm256_mul_pd(vx2, vy1)));

			return res;
		}

		double Vector3D::Dot(const Vector3D& x, const Vector3D& y) noexcept
		{
			const auto tmp = x * y;
			return tmp.X + tmp.Y + tmp.Z;
		}

		Vector3D Vector3D::NormalizeW(const Vector3D& vec) noexcept
		{
			Vector3D res;

			_mm256_stream_pd(res.Vec, _mm256_div_pd(_mm256_loadu_pd(vec.Vec), _mm256_set1_pd(vec.W)));

			return res;
		}
	}
}
