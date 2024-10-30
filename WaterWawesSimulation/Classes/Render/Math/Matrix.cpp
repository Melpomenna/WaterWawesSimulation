#include "Matrix.h"

#include <cmath>

namespace Render
{
	namespace Math
	{
		Matrix::Matrix() noexcept
		{
			const __m512d zero = _mm512_setzero_pd();

			_mm512_stream_pd(Mat, zero);
			_mm512_stream_pd(Mat + 8, zero);
		}

		Matrix::Matrix(const double val)
		{
			const __m512d vec = _mm512_set1_pd(val);

			_mm512_stream_pd(Mat, vec);
			_mm512_stream_pd(Mat + 8, vec);
		}


		Matrix::Matrix(const double x0, const double x1, const double x2, const double x3,
		               const double x4, const double x5, const double x6, const double x7,
		               const double x8, const double x9, const double x10, const double x11,
		               const double x12, const double x13, const double x14, const double x15) noexcept
		{
			_mm512_stream_pd(Mat, _mm512_set_pd(x0, x1, x2, x3, x4, x5, x6, x7));
			_mm512_stream_pd(Mat + 8, _mm512_set_pd(x8, x9, x10, x11, x12, x13, x14, x15));
		}

		Matrix::Matrix(const Matrix& other) noexcept
		{
			const auto or1 = other.AsSimd512(0);
			const auto or2 = other.AsSimd512(1);

			_mm512_stream_pd(Mat, or1);
			_mm512_stream_pd(Mat + 8, or2);
		}

		Matrix& Matrix::operator=(const Matrix& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			const auto or1 = other.AsSimd512(0);
			const auto or2 = other.AsSimd512(1);

			_mm512_stream_pd(Mat, or1);
			_mm512_stream_pd(Mat + 8, or2);

			return *this;
		}

		Matrix::Matrix(Matrix&& other) noexcept
		{
			const auto or1 = other.AsSimd512(0);
			const auto or2 = other.AsSimd512(1);

			_mm512_stream_pd(Mat, or1);
			_mm512_stream_pd(Mat + 8, or2);

			_mm512_stream_pd(other.Mat, _mm512_setzero_pd());
			_mm512_stream_pd(other.Mat + 8, _mm512_setzero_pd());
		}

		Matrix& Matrix::operator=(Matrix&& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			const auto or1 = other.AsSimd512(0);
			const auto or2 = other.AsSimd512(1);

			_mm512_stream_pd(Mat, or1);
			_mm512_stream_pd(Mat + 8, or2);

			_mm512_stream_pd(other.Mat, _mm512_setzero_pd());
			_mm512_stream_pd(other.Mat + 8, _mm512_setzero_pd());

			return *this;
		}

		Matrix Matrix::operator+(const Matrix& other) const noexcept
		{
			Matrix self{*this};

			self += other;

			return self;
		}

		Matrix& Matrix::operator+=(const Matrix& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			const auto sr1 = AsSimd512(0);
			const auto sr2 = AsSimd512(1);

			const auto or1 = other.AsSimd512(0);
			const auto or2 = other.AsSimd512(1);

			const auto rr1 = _mm512_add_pd(sr1, or1);
			const auto rr2 = _mm512_add_pd(sr2, or2);

			_mm512_stream_pd(Mat, rr1);
			_mm512_stream_pd(Mat + 8, rr2);

			return *this;
		}

		Matrix Matrix::operator-(const Matrix& other) const noexcept
		{
			Matrix self{*this};

			self -= other;

			return self;
		}

		Matrix& Matrix::operator-=(const Matrix& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			const auto sr1 = AsSimd512(0);
			const auto sr2 = AsSimd512(1);

			const auto or1 = other.AsSimd512(0);
			const auto or2 = other.AsSimd512(1);

			const auto rr1 = _mm512_sub_pd(sr1, or1);
			const auto rr2 = _mm512_sub_pd(sr2, or2);

			_mm512_stream_pd(Mat, rr1);
			_mm512_stream_pd(Mat + 8, rr2);

			return *this;
		}

		Matrix Matrix::operator*(const Matrix& other) const noexcept
		{
			Matrix self{*this};

			self *= other;

			return self;
		}

		Matrix& Matrix::operator*=(const Matrix& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			Matrix result{};

			const auto ov1 = _mm256_loadu_pd(other.Mat);
			const auto ov2 = _mm256_loadu_pd(other.Mat + 4);
			const auto ov3 = _mm256_loadu_pd(other.Mat + 8);
			const auto ov4 = _mm256_loadu_pd(other.Mat + 12);

			// 1
			const auto rm00 = _mm256_mul_pd(_mm256_set1_pd(Mat[0]), ov1);
			const auto rm01 = _mm256_mul_pd(_mm256_set1_pd(Mat[1]), ov2);
			const auto rm02 = _mm256_mul_pd(_mm256_set1_pd(Mat[2]), ov3);
			const auto rm03 = _mm256_mul_pd(_mm256_set1_pd(Mat[3]), ov4);

			const auto rr1 = _mm256_add_pd(_mm256_add_pd(rm00, rm01), _mm256_add_pd(rm02, rm03));

			_mm256_stream_pd(result.Mat, rr1);


			// 2
			const auto rm10 = _mm256_mul_pd(_mm256_set1_pd(Mat[4]), ov1);
			const auto rm11 = _mm256_mul_pd(_mm256_set1_pd(Mat[5]), ov2);
			const auto rm12 = _mm256_mul_pd(_mm256_set1_pd(Mat[6]), ov3);
			const auto rm13 = _mm256_mul_pd(_mm256_set1_pd(Mat[7]), ov4);

			const auto rr2 = _mm256_add_pd(_mm256_add_pd(rm10, rm11), _mm256_add_pd(rm12, rm13));

			_mm256_stream_pd(result.Mat + 4, rr2);

			// 3
			const auto rm20 = _mm256_mul_pd(_mm256_set1_pd(Mat[8]), ov1);
			const auto rm21 = _mm256_mul_pd(_mm256_set1_pd(Mat[9]), ov2);
			const auto rm22 = _mm256_mul_pd(_mm256_set1_pd(Mat[10]), ov3);
			const auto rm23 = _mm256_mul_pd(_mm256_set1_pd(Mat[11]), ov4);

			const auto rr3 = _mm256_add_pd(_mm256_add_pd(rm20, rm21), _mm256_add_pd(rm22, rm23));

			_mm256_stream_pd(result.Mat + 8, rr3);

			// 4
			const auto rm30 = _mm256_mul_pd(_mm256_set1_pd(Mat[12]), ov1);
			const auto rm31 = _mm256_mul_pd(_mm256_set1_pd(Mat[13]), ov2);
			const auto rm32 = _mm256_mul_pd(_mm256_set1_pd(Mat[14]), ov3);
			const auto rm33 = _mm256_mul_pd(_mm256_set1_pd(Mat[15]), ov4);

			const auto rr4 = _mm256_add_pd(_mm256_add_pd(rm30, rm31), _mm256_add_pd(rm32, rm33));

			_mm256_stream_pd(result.Mat + 12, rr4);

			return *this;
		}

		Matrix Matrix::operator+(const double val) const noexcept
		{
			return *this + Once(val);
		}

		Matrix& Matrix::operator+=(const double val) noexcept
		{
			*this += Once(val);

			return *this;
		}

		Matrix Matrix::operator-(const double val) const noexcept
		{
			return *this - Once(val);
		}

		Matrix& Matrix::operator-=(const double val) noexcept
		{
			*this -= Once(val);
			return *this;
		}

		Matrix Matrix::operator*(const double val) const noexcept
		{
			const auto vm = _mm512_set1_pd(val);

			Matrix res{};

			_mm512_stream_pd(res.Mat, _mm512_mul_pd(_mm512_loadu_pd(Mat), vm));
			_mm512_stream_pd(res.Mat + 8, _mm512_mul_pd(_mm512_loadu_pd(Mat + 8), vm));

			return res;
		}

		Matrix& Matrix::operator*=(const double val) noexcept
		{
			const auto vm = _mm512_set1_pd(val);

			_mm512_stream_pd(Mat, _mm512_mul_pd(_mm512_loadu_pd(Mat), vm));
			_mm512_stream_pd(Mat + 8, _mm512_mul_pd(_mm512_loadu_pd(Mat + 8), vm));

			return *this;
		}

		Matrix Matrix::operator/(const double val) const noexcept
		{
			const auto vm = _mm512_set1_pd(val);

			Matrix res{};

			_mm512_stream_pd(res.Mat, _mm512_div_pd(_mm512_loadu_pd(Mat), vm));
			_mm512_stream_pd(res.Mat + 8, _mm512_div_pd(_mm512_loadu_pd(Mat + 8), vm));

			return *this;
		}

		Matrix& Matrix::operator/=(const double val) noexcept
		{
			const auto vm = _mm512_set1_pd(val);

			_mm512_stream_pd(Mat, _mm512_div_pd(_mm512_loadu_pd(Mat), vm));
			_mm512_stream_pd(Mat + 8, _mm512_div_pd(_mm512_loadu_pd(Mat + 8), vm));

			return *this;
		}

		Vector3D Matrix::operator*(const Vector3D& vec) const noexcept
		{
			const auto normalized = Vector3D::NormalizeW(vec);
			Vector3D newVec;


			const auto sn = _mm256_loadu_pd(normalized.AsArray());

			auto xx = _mm256_loadu_pd(newVec.AsArray());

			xx = _mm256_mask_mul_pd(xx, 0b0111, _mm256_loadu_pd(Mat), sn);

			auto yy = _mm256_loadu_pd(newVec.AsArray());

			yy = _mm256_mask_mul_pd(yy, 0b0111, _mm256_loadu_pd(Mat + 4), sn);

			auto zz = _mm256_loadu_pd(newVec.AsArray());

			zz = _mm256_mask_mul_pd(zz, 0b0111, _mm256_loadu_pd(Mat + 8), sn);

			_mm256_mask_store_pd(newVec.Vec, 0b0001, xx);
			_mm256_mask_store_pd(newVec.Vec, 0b0010, yy);
			_mm256_mask_store_pd(newVec.Vec, 0b0100, zz);

			return newVec;
		}


		Vector3D Matrix::Scale(const Vector3D& vec, const Vector3D& pos) noexcept
		{
			const auto normalized = Vector3D::NormalizeW(vec);
			auto identity = Identity();

			_mm256_storeu_pd(identity.Mat, _mm256_mask_mul_pd(_mm256_loadu_pd(identity.Mat), 0b1110,
			                                                  _mm256_loadu_pd(identity.Mat),
			                                                  _mm256_loadu_pd(normalized.AsArray())));
			_mm256_storeu_pd(identity.Mat + 4,
			                 _mm256_mask_mul_pd(_mm256_loadu_pd(identity.Mat + 4), 0b1101,
			                                    _mm256_loadu_pd(identity.Mat + 4),
			                                    _mm256_loadu_pd(normalized.AsArray())));
			_mm256_storeu_pd(identity.Mat + 8,
			                 _mm256_mask_mul_pd(_mm256_loadu_pd(identity.Mat + 8), 0b1011,
			                                    _mm256_loadu_pd(identity.Mat + 8),
			                                    _mm256_loadu_pd(normalized.AsArray())));

			return identity * Vector3D::NormalizeW(pos);
		}

		Vector3D Matrix::Translate(const Vector3D& vec, const Vector3D& pos) noexcept
		{
			const auto normalized = Vector3D::NormalizeW(vec);
			auto identity = Identity();

			identity[0][3] = normalized.X;
			identity[1][3] = normalized.Y;
			identity[2][3] = normalized.Z;


			return identity * Vector3D::NormalizeW(pos);
		}

		Vector3D Matrix::Rotate(const double angle, const Vector3D& vec, const Vector3D& pos) noexcept
		{
			// Rodrigo Rotate


			const Vector3D norm = Vector3D::NormalizeW(vec);

			const auto vVec = _mm256_loadu_pd(norm.AsArray());
			const auto vInverseVec = _mm256_set_pd(norm.Z, norm.Y, norm.X, 1);

			const auto vecCos = _mm256_cos_pd(_mm256_set1_pd(angle));
			const auto tVecCos = _mm256_sub_pd(_mm256_set1_pd(1), vecCos);
			const auto vecSin = _mm256_sin_pd(_mm256_set1_pd(angle));

			const auto quadVVec = _mm256_mul_pd(vVec, vVec);

			const auto pairsVec = _mm256_mul_pd(_mm256_set_pd(norm.X, norm.X, norm.Y, 1),
			                                    _mm256_set_pd(norm.Y, norm.Z, norm.Z, 1));

			const auto quadTVVec = _mm256_mask_mul_pd(quadVVec, 0b0111, quadVVec, tVecCos);
			const auto tvCosPairs = _mm256_mask_mul_pd(pairsVec, 0b0111, pairsVec, tVecCos);
			const auto vVecSin = _mm256_mask_mul_pd(vVec, 0b0111, vecSin, vInverseVec);

			const auto quadTVVecCos = _mm256_mask_add_pd(quadTVVec, 0b0111, quadTVVec, vecCos);
			const auto tvCosPairSubVec = _mm256_mask_sub_pd(tvCosPairs, 0b0111, tvCosPairs, vVecSin);
			const auto tvCosPaisAddVec = _mm256_mask_add_pd(tvCosPairs, 0b0111, tvCosPairs, vVecSin);

			Matrix rotate = Identity();

			auto* ptr = rotate.AsArray();

			const auto r1 = _mm256_loadu_pd(ptr);
			const auto r2 = _mm256_loadu_pd(ptr + 4);
			const auto r3 = _mm256_loadu_pd(ptr + 8);

			const auto zero = _mm256_set1_pd(0);


			auto rr1 = _mm256_mask_add_pd(r1, 0b1001, quadTVVecCos, zero);
			auto rr2 = _mm256_mask_add_pd(r2, 0b1010, quadTVVecCos, zero);
			auto rr3 = _mm256_mask_add_pd(r3, 0b1100, quadTVVecCos, zero);

			rr1 = _mm256_mask_add_pd(rr1, 0b1010, rr1, tvCosPairSubVec);
			rr1 = _mm256_mask_add_pd(rr1, 0b1100, rr1, tvCosPaisAddVec);

			rr2 = _mm256_mask_add_pd(rr2, 0b1001, rr2, tvCosPaisAddVec);
			rr2 = _mm256_mask_add_pd(rr2, 0b1001, rr2, tvCosPairSubVec);

			rr3 = _mm256_mask_add_pd(rr3, 0b1001, rr3, tvCosPairSubVec);
			rr3 = _mm256_mask_add_pd(rr3, 0b1010, rr3, tvCosPaisAddVec);

			_mm256_stream_pd(ptr, rr1);
			_mm256_stream_pd(ptr + 4, rr2);
			_mm256_stream_pd(ptr + 8, rr3);

			return rotate * pos;
		}

		Matrix Matrix::LookAt(const Vector3D& eye, const Vector3D& pos, const Vector3D& up) noexcept
		{
			Vector3D f((pos - eye).Normalize());
			const Vector3D s(Vector3D::Cross(f, up).Normalize());
			const Vector3D u(Vector3D::Cross(s, f));

			Matrix result(1);
			result[0][0] = s.X;
			result[1][0] = s.Y;
			result[2][0] = s.Z;
			result[0][1] = u.X;
			result[1][1] = u.Y;
			result[2][1] = u.Z;
			result[0][2] = -f.X;
			result[1][2] = -f.Y;
			result[2][2] = -f.Z;
			result[3][0] = -Vector3D::Dot(s, eye);
			result[3][1] = -Vector3D::Dot(u, eye);
			result[3][2] = Vector3D::Dot(f, eye);
			return result;
		}


		Matrix Matrix::Once() noexcept
		{
			return Matrix{
				1, 1, 1, 1,
				1, 1, 1, 1,
				1, 1, 1, 1,
				1, 1, 1, 1
			};
		}

		Matrix Matrix::Identity() noexcept
		{
			return Matrix{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		Matrix Matrix::Identity(const double val) noexcept
		{
			return Matrix{
				val, 0, 0, 0,
				0, val, 0, 0,
				0, 0, val, 0,
				0, 0, 0, val
			};
		}

		Matrix Matrix::Ortho(const double left, const double right, const double bottom, const double top,
		                     const double zNear, const double zFar) noexcept
		{
			Matrix result{0};


			result[0][0] = static_cast<double>(2) / (right - left);
			result[1][1] = static_cast<double>(2) / (top - bottom);
			result[2][2] = static_cast<double>(1) / (zFar - zNear);
			result[3][0] = -(right + left) / (right - left);
			result[3][1] = -(top + bottom) / (top - bottom);
			result[3][2] = -zNear / (zFar - zNear);
			return result;
		}

		Matrix Matrix::Frustum(const double left, const double right, const double bottom, const double top,
		                       const double nearVal, const double farVal) noexcept
		{
			Matrix result{0};
			result[0][0] = (static_cast<double>(2) * nearVal) / (right - left);
			result[1][1] = (static_cast<double>(2) * nearVal) / (top - bottom);
			result[2][0] = (right + left) / (right - left);
			result[2][1] = (top + bottom) / (top - bottom);
			result[2][2] = -(farVal + nearVal) / (farVal - nearVal);
			result[2][3] = static_cast<double>(-1);
			result[3][2] = -(static_cast<double>(2) * farVal * nearVal) / (farVal - nearVal);
			return result;
		}

		Matrix Matrix::Perspective(const double fovy, const double aspect,
		                           const double zNear, const double zFar) noexcept
		{
			const double fTan = tan(fovy / 2.0);

			Matrix result(0);
			result[0][0] = 1.0 / (aspect * fTan);
			result[1][1] = 1.0 / (fTan);
			result[2][2] = -(zFar + zNear) / (zFar - zNear);
			result[2][3] = -1.0;
			result[3][2] = -(2.0 * zFar * zNear) / (zFar - zNear);
			return result;
		}


		Matrix Matrix::Once(const double value) noexcept
		{
			return Matrix{
				value, value, value, value,
				value, value, value, value,
				value, value, value, value,
				value, value, value, value
			};
		}

		[[nodiscard]] double* Matrix::AsArray() noexcept
		{
			return Mat;
		}

		[[nodiscard]] const double* Matrix::AsArray() const noexcept
		{
			return Mat;
		}

		Matrix operator+(const double val, const Matrix& mat) noexcept
		{
			return mat + val;
		}

		Matrix operator-(const double val, const Matrix& mat) noexcept
		{
			return mat - val;
		}

		Matrix operator*(const double val, const Matrix& mat) noexcept
		{
			return mat * val;
		}

		Matrix operator/(const double val, const Matrix& mat) noexcept
		{
			return mat / val;
		}

		double& Matrix::operator()(const size_t row, const size_t col) & noexcept
		{
			return Mat[col + row * 4];
		}

		const double& Matrix::operator()(const size_t row, const size_t col) const & noexcept
		{
			return Mat[col + row * 4];
		}

		double* Matrix::operator[](const size_t row) & noexcept
		{
			return &Mat[row];
		}

		const double* Matrix::operator[](const size_t row) const & noexcept
		{
			return &Mat[row];
		}

		__m256d Matrix::AsSimd256(const size_t i) const noexcept
		{
			return _mm256_loadu_pd(Mat + i * sizeof(double) / 2);
		}

		__m512d Matrix::AsSimd512(const size_t i) const noexcept
		{
			return _mm512_loadu_pd(Mat + i * sizeof(double));
		}
	}
}
