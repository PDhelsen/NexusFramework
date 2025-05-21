#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Numbers/Decimal.h"
#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"

#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Math/Rotation.h"

namespace NxFr
{
#pragma region Declaration

	template<uint8 R, uint8 C, typename T>
	struct Matrix;

	using Matrix4x4f = Matrix<4, 4, float>;

	namespace MatrixUtility
	{
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Identity();
		template<uint8 R, uint8 C,			typename T> Matrix<R - 1, C - 1, T>		Minor(const Matrix<R, C, T>& M, uint8 Row, uint8 Column);
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Add(const Matrix<R, C, T>& A, T B);
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Add(const Matrix<R, C, T>& A, const Matrix<R, C, T>& B);
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Subtract(const Matrix<R, C, T>& A, T B);
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Subtract(const Matrix<R, C, T>& A, const Matrix<R, C, T>& B);
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Multiply(const Matrix<R, C, T>& A, T B);
		template<uint8 R, uint8 C,			typename T> Vector<C, T>				Multiply(const Matrix<R, C, T>& A, Vector<C, T> B);
		template<uint8 R, uint8 C, int8 K,	typename T> Matrix<R, C, T>				Multiply(const Matrix<R, K, T>& A, const Matrix<K, C, T>& B);
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Tensor(Vector<R, T>& A, Vector<C, T> B);
		template<uint8 R, uint8 C,			typename T> Matrix<C, R, T>				Transpose(const Matrix<R, C, T>& M);
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Inverse(const Matrix<R, C, T>& M);
		template<uint8 R, uint8 C,			typename T> Matrix<R, C, T>				Cofactor(const Matrix<R, C, T>& M);
		template<uint8 R, uint8 C,			typename T> float						Cofactor(const Matrix<R, C, T>& M, uint8 Row, uint8 Column);
		template<uint8 R, uint8 C,			typename T> float						Determinant(const Matrix<R, C, T>& M);
		template<uint8 R, uint8 C,			typename T> float						Trace(const Matrix<R, C, T>& M);
	}

#pragma endregion

#pragma region Matrix

	template<uint8 R, uint8 C, typename T>
	struct Matrix
	{
	public:
		inline static const bool Square = R == C;
		inline static const uint8 Dimension = Square ? R : 0;
		inline static const uint8 Count = R * C;
		inline static const uint8 Size = Count * sizeof(T);

		static const Matrix<R, C, T> Zero;
		static const Matrix<R, C, T> Identity;

		Matrix() { Memory::MemSet(&m[0], 0, Size); }
		Matrix(const Matrix<R, C, T>& Other) { Memory::MemCopy(&Other.m[0], &m[0], Size); }
		~Matrix() { }

		Matrix<R, C, T>& operator=(const Matrix<R, C, T>& Other) { Memory::MemCopy(&Other.m[0], &m[0], Size); return *this; }
		bool operator==(const Matrix<R, C, T>& Other) const { return Memory::MemCompare(&m[0], &Other.m[0], Size); }
		bool operator!=(const Matrix<R, C, T>& Other) const { return !(*this == Other); }
		T& operator[](uint8 Index) { return m[Index]; }
		const T& operator[](uint8 Index) const { return m[Index]; }
		T& operator()(uint8 Row, uint8 Column) { return m[GetIndex(Row, Column)]; }
		const T& operator()(uint8 Row, uint8 Column) const { return m[GetIndex(Row, Column)]; }

		Matrix<R, C, T>& operator+=(T Other) { *this = MatrixUtility::Add(*this, Other); return *this; }
		Matrix<R, C, T>& operator+=(const Matrix<R, C, T>& Other) { *this = MatrixUtility::Add(*this, Other); return *this; }
		Matrix<R, C, T>& operator*=(T Other) { *this = MatrixUtility::Multiply(*this, Other); return *this; }
		Matrix<R, C, T>& operator*=(const Matrix<R, C, T>& Other) { *this = MatrixUtility::Multiply(*this, Other); return *this; }

		constexpr uint8 GetIndex(uint8 Row, uint8 Column) const { return Column * R + Row; }
		T Get(uint8 Row, uint8 Column) const { return m[GetIndex(Row, Column)]; }
		void Set(uint8 Row, uint8 Column, T Value) { m[GetIndex(Row, Column)] = Value; }
		Vector<C, T> GetRow(uint8 Index) const { Vector<C, T> Result; for (uint8 I = 0; I < C; I++) Result[I] = m[GetIndex(Index, I)]; return Result; }
		void SetRow(uint8 Index, Vector<C, T> Value) { for (uint8 I = 0; I < C; I++) m[GetIndex(Index, I)] = Value[I]; }
		Vector<R, T> GetColumn(uint8 Index) const { Vector<R, T> Result; for (uint8 I = 0; I < R; I++) Result[I] = m[GetIndex(I, Index)]; return Result; }
		void SetColumn(uint8 Index, Vector<R, T> Value) { for (uint8 I = 0; I < R; I++) m[GetIndex(I, Index)] = Value[I]; }
		Vector<Dimension, T> GetDiagonal() const { Vector<R, T> Result; for (uint8 I = 0; I < R; I++) Result[I] = m[GetIndex(I, I)]; return Result; }
		void SetDiagonal(Vector<Dimension, T> Value) const { Vector<R, T> Result; for (uint8 I = 0; I < R; I++) m[GetIndex(I, I)] = Value[I]; return Result; }
		Matrix<R - 1, C - 1, T> GetMinor(uint8 Row, uint8 Column) const { return MatrixUtility::Minor(*this, Row, Column); }

		Matrix<C, R, T> Transpose() const { return MatrixUtility::Transpose(*this); }
		Matrix<R, C, T> Inverse() const { return MatrixUtility::Inverse(*this); }
		Matrix<R, C, T> Cofactor() const { return MatrixUtility::Cofactor(*this); }
		float Cofactor(uint8 Row, uint8 Column) const { return MatrixUtility::Cofactor(*this, Row, Column); }
		float Determinant() const { return MatrixUtility::Determinant(*this); }
		float Trace() const { return MatrixUtility::Trace(*this); }

		String ToString() const
		{
			String Result;
			for (uint8 Column = 0; Column < C; ++Column)
			{
				for (uint8 Row = 0; Row < R; ++Row)
				{
					Result += StringUtility::ToStringF((float)*this(Row, Column));
					Result += ", ";
				}
				Result += StringUtility::NewLine;

			}
			return Result;
		}

	public:
		T m[Count];
	};

	template<uint8 R, uint8 C, typename T> inline const Matrix<R, C, T> Matrix<R, C, T>::Zero = Matrix<R, C, T>();
	template<uint8 R, uint8 C, typename T> inline const Matrix<R, C, T> Matrix<R, C, T>::Identity = MatrixUtility::Identity<R, C, T>();

	template<uint8 R, uint8 C,			typename T> inline Matrix<R, C, T>	operator+(const Matrix<R, C, T>& A, T B) { return MatrixUtility::Add(A, B); }
	template<uint8 R, uint8 C,			typename T> inline Matrix<R, C, T>	operator+(const Matrix<R, C, T>& A, const Matrix<R, C, T>& B) { return MatrixUtility::Add(A, B); }
	template<uint8 R, uint8 C,			typename T> inline Matrix<R, C, T>	operator-(const Matrix<R, C, T>& A) { return MatrixUtility::Multiply(A, -1.0f); }
	template<uint8 R, uint8 C,			typename T> inline Matrix<R, C, T>	operator-(const Matrix<R, C, T>& A, T B) { return MatrixUtility::Subtract(A, B); }
	template<uint8 R, uint8 C,			typename T> inline Matrix<R, C, T>	operator-(const Matrix<R, C, T>& A, const Matrix<R, C, T>& B) { return MatrixUtility::Subtract(A, B); }
	template<uint8 R, uint8 C,			typename T> inline Matrix<R, C, T>	operator*(const Matrix<R, C, T>& A, T B) { return MatrixUtility::Multiply(A, B); }
	template<uint8 R, uint8 C,			typename T> inline Vector<C, T>		operator*(const Matrix<R, C, T>& A, Vector<C, T> B) { return MatrixUtility::Multiply(A, B); }
	template<uint8 R, uint8 C, uint8 K, typename T> inline Matrix<R, K, T>	operator*(const Matrix<R, K, T>& A, const Matrix<K, C, T>& B) { return MatrixUtility::Multiply(A, B); }

#pragma endregion

#pragma region Matrix4x4f

	template<>
	struct Matrix<4, 4, float>
	{
	public:
		inline static const bool Square = 4 == 4;
		inline static const uint8 Dimension = Square ? 4 : 0;
		inline static const uint8 Count = 4 * 4;
		inline static const uint8 Size = Count * sizeof(float);

		static const Matrix<4, 4, float> Zero;
		static const Matrix<4, 4, float> Identity;

		static Matrix<4, 4, float> Origin(const Matrix<4, 4, float>& Transformation, Vector<3, float> Center)
		{
			Vector<4, float> Delta = (Identity - Transformation) * Vector<4, float>(Center.x, Center.y, Center.z, 1.0f);
			Delta.w = 1.0f;

			Matrix<4, 4, float> Result = Transformation;
			Result.SetColumn(3, Delta);
			return Result;
		}

		static Matrix<4, 4, float> Basis(Vector<3, float> X, Vector<3, float> Y, Vector<3, float> Z)
		{
			Matrix<4, 4, float> Result = Identity;
			Result[ 0] = X.x;
			Result[ 1] = X.y;
			Result[ 2] = X.z;
			Result[ 4] = Y.x;
			Result[ 5] = Y.y;
			Result[ 6] = Y.z;
			Result[ 8] = Z.x;
			Result[ 9] = Z.y;
			Result[10] = Z.z;
			return Result;
		}

		static Matrix<4, 4, float> LookAt(Vector<3, float> Position, Vector<3, float> Target, Vector<3, float> Up)
		{
			return LookAt(VectorUtility::Normalize(Target - Position), Up);
		}

		static Matrix<4, 4, float> LookAt(Vector<3, float> Direction, Vector<3, float> Up)
		{
			Vector<3, float> Right = VectorUtility::Normalize(VectorUtility::Cross(Up, Direction));
			Up = VectorUtility::Cross(Direction, Right);

			return Basis(Right, Up, Direction);
		}

		static Matrix<4, 4, float> TRS(Vector<3, float> Translation, Quaternion Rotation, Vector<3, float> Scaling)
		{
			return Translate(Translation) * Rotate(Rotation) * Scale(Scaling);
		}

		static Matrix<4, 4, float> Translate(Vector<3, float> Value)
		{
			Matrix<4, 4, float> Result = Identity;
			Result[12] = Value.x;
			Result[13] = Value.y;
			Result[14] = Value.z;
			return Result;
		}

		static Matrix<4, 4, float> Rotate(Quaternion Value)
		{
			Matrix<4, 4, float> Result = Identity;
			Result[ 0] = 1.0f - 2.0f * Math::Square(Value.y) - 2.0f * Math::Square(Value.z);
			Result[ 1] = 2.0f * Value.x * Value.y + 2.0f * Value.w * Value.z;
			Result[ 2] = 2.0f * Value.x * Value.z - 2.0f * Value.w * Value.y;
			Result[ 4] = 2.0f * Value.x * Value.y - 2.0f * Value.w * Value.z;
			Result[ 5] = 1.0f - 2.0f * Math::Square(Value.x) - 2.0f * Math::Square(Value.z);
			Result[ 6] = 2.0f * Value.y * Value.z + 2.0f * Value.w * Value.x;
			Result[ 8] = 2.0f * Value.x * Value.z + 2.0f * Value.w * Value.y;
			Result[ 9] = 2.0f * Value.y * Value.z - 2.0f * Value.w * Value.x;
			Result[10] = 1.0f - 2.0f * Math::Square(Value.x) - 2.0f * Math::Square(Value.y);
			return Result;
		}

		static Matrix<4, 4, float> Scale(Vector<3, float> Value)
		{
			Matrix<4, 4, float> Result = Identity;
			Result[ 0] = Value.x;
			Result[ 5] = Value.y;
			Result[10] = Value.z;
			return Result;
		}

		static Matrix<4, 4, float> Reflect(Vector<3, float> Normal)
		{
			Matrix<4, 4, float> Result = Identity;
			for (uint8 Row = 0; Row < 3; ++Row)
			{
				for (uint8 Column = 0; Column < 3; ++Column)
				{
					Result(Row, Column) = (Row == Column ? 1.0f : 0.0f) - 2.0f * Normal[Row] * Normal[Column];
				}
			}
			return Result;
		}

		static Matrix<4, 4, float> Shear(Vector<3, float> Normal, Vector<3, float> Value)
		{
			Matrix<4, 4, float> Result = Identity;
			for (uint8 Row = 0; Row < 3; ++Row)
			{
				for (uint8 Column = 0; Column < 3; ++Column)
				{
					Result(Row, Column) = (Row == Column ? 1.0f : 0.0f) + Value[Row] * Normal[Column];
				}
			}
			return Result;
		}

		Matrix() { Memory::MemSet(&m[0], 0, Size); }
		Matrix(const Matrix<4, 4, float>& Other) { Memory::MemCopy(&Other.m[0], &m[0], Size); }
		~Matrix() { }

		Matrix<4, 4, float>& operator=(const Matrix<4, 4, float>& Other) { Memory::MemCopy(&Other.m[0], &m[0], Size); return *this; }
		bool operator==(const Matrix<4, 4, float>& Other) const { return Memory::MemCompare(&m[0], &Other.m[0], Size); }
		bool operator!=(const Matrix<4, 4, float>& Other) const { return !(*this == Other); }
		float& operator[](uint8 Index) { return m[Index]; }
		const float& operator[](uint8 Index) const { return m[Index]; }
		float& operator()(uint8 Row, uint8 Column) { return m[GetIndex(Row, Column)]; }
		const float& operator()(uint8 Row, uint8 Column) const { return m[GetIndex(Row, Column)]; }

		Matrix<4, 4, float>& operator+=(float Other) { *this = MatrixUtility::Add(*this, Other); return *this; }
		Matrix<4, 4, float>& operator+=(const Matrix<4, 4, float>& Other) { *this = MatrixUtility::Add(*this, Other); return *this; }
		Matrix<4, 4, float>& operator*=(float Other) { *this = MatrixUtility::Multiply(*this, Other); return *this; }
		Matrix<4, 4, float>& operator*=(const Matrix<4, 4, float>& Other) { *this = MatrixUtility::Multiply(*this, Other); return *this; }

		Vector<3, float> MultiplyPosition(Vector<3, float> Other) const { return (Vector<3, float>)MatrixUtility::Multiply(*this, Vector<4, float>(Other.x, Other.y, Other.z, 1.0f)); }
		Vector<3, float> MultiplyDirection(Vector<3, float> Other) const { return (Vector<3, float>)MatrixUtility::Multiply(*this, Vector<4, float>(Other.x, Other.y, Other.z, 0.0f)); }

		constexpr uint8 GetIndex(uint8 Row, uint8 Column) const { return Column * 4 + Row; }
		float Get(uint8 Row, uint8 Column) const { return m[GetIndex(Row, Column)]; }
		void Set(uint8 Row, uint8 Column, float Value) { m[GetIndex(Row, Column)] = Value; }
		Vector<4, float> GetRow(uint8 Index) const { return Vector<4, float>(m[GetIndex(Index, 0)], m[GetIndex(Index, 1)], m[GetIndex(Index, 2)], m[GetIndex(Index, 3)]); }
		void SetRow(uint8 Index, Vector<4, float> Value) { m[GetIndex(Index, 0)] = Value[0]; m[GetIndex(Index, 1)] = Value[1]; m[GetIndex(Index, 2)] = Value[2]; m[GetIndex(Index, 3)] = Value[3]; }
		Vector<4, float> GetColumn(uint8 Index) const { return Vector<4, float>(m[GetIndex(0, Index)], m[GetIndex(1, Index)], m[GetIndex(2, Index)], m[GetIndex(3, Index)]); }
		void SetColumn(uint8 Index, Vector<4, float> Value) { m[GetIndex(0, Index)] = Value[0]; m[GetIndex(1, Index)] = Value[1]; m[GetIndex(2, Index)] = Value[2]; m[GetIndex(3, Index)] = Value[3]; }
		Vector<4, float> GetDiagonal() const { return Vector<4, float>(m[GetIndex(0, 0)], m[GetIndex(1, 1)], m[GetIndex(2, 2)], m[GetIndex(3, 3)]); }
		void SetDiagonal(Vector<4, float> Value) { m[GetIndex(0, 0)] = Value[0]; m[GetIndex(1, 1)] = Value[1]; m[GetIndex(2, 2)] = Value[2]; m[GetIndex(3, 3)] = Value[3]; }
		Matrix<4 - 1, 4 - 1, float> GetMinor(uint8 Row, uint8 Column) const { return MatrixUtility::Minor(*this, Row, Column); }

		Vector<3, float> GetPosition() const
		{
			return GetColumn(3);
		}
		Quaternion GetRotation() const
		{
			Quaternion Result = Quaternion::Identity;

			Matrix<3, 3, float> Pure = Matrix<3, 3, float>::Identity;
			Pure.SetColumn(0, VectorUtility::Normalize(Vector<3, float>(m[ 0], m[ 1], m[ 2])));
			Pure.SetColumn(1, VectorUtility::Normalize(Vector<3, float>(m[ 4], m[ 5], m[ 6])));
			Pure.SetColumn(2, VectorUtility::Normalize(Vector<3, float>(m[ 8], m[ 9], m[10])));

			float trace = Pure.Trace();
			if (trace > 0.0f)
			{
				float S = Math::Sqrt(trace + 1.0f);
				Result.w = S * 0.5f;
				float Inv = 0.5f / S;
				Result.x = (Pure.Get(2, 1) - Pure.Get(1, 2)) * Inv;
				Result.y = (Pure.Get(0, 2) - Pure.Get(2, 0)) * Inv;
				Result.z = (Pure.Get(1, 0) - Pure.Get(0, 1)) * Inv;
			}
			else
			{
				uint8 I = 0;
				if (Pure.Get(1, 1) > Pure.Get(0, 0))
					I = 1;
				if (Pure.Get(2, 2) > Pure.Get(I, I))
					I = 2;
				uint8 J = (I + 1) % 3;
				uint8 K = (J + 1) % 3;

				float S = Math::Sqrt(Pure.Get(I, I) - Pure.Get(J, J) - Pure.Get(K, K) + 1.0f);
				float Inv = 0.5f / S;

				Result[I] = 0.5f * S;
				Result[J] = (Pure.Get(J, I) + Pure.Get(I, J)) * Inv;
				Result[K] = (Pure.Get(K, I) + Pure.Get(I, K)) * Inv;
				Result.w = (Pure.Get(K, J) - Pure.Get(J, K)) * Inv;
			}

			return Result;
		}
		Vector<3, float> GetScale() const
		{
			float x = VectorUtility::Magnitude(Vector<3, float>(m[ 0], m[ 1], m[ 2]));
			float y = VectorUtility::Magnitude(Vector<3, float>(m[ 4], m[ 5], m[ 6]));
			float z = VectorUtility::Magnitude(Vector<3, float>(m[ 8], m[ 9], m[10]));
			return Vector<3, float>(x, y, z);
		}

		Matrix<4, 4, float> Transpose() const { return MatrixUtility::Transpose(*this); }
		Matrix<4, 4, float> Inverse() const { return MatrixUtility::Inverse(*this); }
		Matrix<4, 4, float> Cofactor() const { return MatrixUtility::Cofactor(*this); }
		float Cofactor(uint8 Row, uint8 Column) const { return MatrixUtility::Cofactor(*this, Row, Column); }
		float Determinant() const { return MatrixUtility::Determinant(*this); }
		float Trace() const { return MatrixUtility::Trace(*this); }

		String ToString() const
		{
			String Result;
			for (uint8 Column = 0; Column < 4; ++Column)
			{
				for (uint8 Row = 0; Row < 4; ++Row)
				{
					Result += StringUtility::ToStringF((float)Get(Row, Column));
					Result += ", ";
				}
				Result += StringUtility::NewLine;

			}
			return Result;
		}

	public:
		float m[Count];
	};

	inline const Matrix<4, 4, float> Matrix<4, 4, float>::Zero = Matrix<4, 4, float>();
	inline const Matrix<4, 4, float> Matrix<4, 4, float>::Identity = MatrixUtility::Identity<4, 4, float>();

#pragma endregion

#pragma region MatrixUtility

	namespace MatrixUtility
	{
		template<uint8 R, uint8 C, typename T>
		inline bool Equals(const Matrix<R, C, T>& A, const Matrix<R, C, T>& B)
		{
			for (uint8 Index = 0; Index < A.Count; ++Index)
			{
				if (!Math::Equals(A[Index], B[Index]))
				{
					return false;
				}
			}

			return true;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Identity()
		{
			if constexpr (!Matrix<R, C, T>::Square)
			{
				return Matrix<R, C, T>::Zero;
			}

			Matrix<R, C, T> Result;
			for (uint8 Index = 0; Index < Matrix<R, C, T>::Dimension; ++Index)
			{
				Result(Index, Index) = 1.0f;
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R - 1, C - 1, T> Minor(const Matrix<R, C, T>& M, uint8 Row, uint8 Column)
		{
			Matrix<R - 1, C - 1, T> Result;
			for (uint8 Row1 = 0, Row2 = 0; Row1 < R; ++Row1)
			{
				if (Row1 == Row) continue;
				for (uint8 Column1 = 0, Column2 = 0; Column1 < C; ++Column1)
				{
					if (Column1 == Column) continue;

					Result(Row2, Column2) = M(Row1, Column1);
					Column2++;
				}

				Row2++;
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Add(const Matrix<R, C, T>& A, T B)
		{
			Matrix<R, C, T> Result;
			for (uint8 Index = 0; Index < Matrix<R, C, T>::Count; ++Index)
			{
				Result[Index] = A[Index] + B;
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Add(const Matrix<R, C, T>& A, const Matrix<R, C, T>& B)
		{
			Matrix<R, C, T> Result;
			for (uint8 Index = 0; Index < Matrix<R, C, T>::Count; ++Index)
			{
				Result[Index] = A[Index] + B[Index];
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Subtract(const Matrix<R, C, T>& A, T B)
		{
			Matrix<R, C, T> Result;
			for (uint8 Index = 0; Index < Matrix<R, C, T>::Count; ++Index)
			{
				Result[Index] = A[Index] - B;
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Subtract(const Matrix<R, C, T>& A, const Matrix<R, C, T>& B)
		{
			Matrix<R, C, T> Result;
			for (uint8 Index = 0; Index < Matrix<R, C, T>::Count; ++Index)
			{
				Result[Index] = A[Index] - B[Index];
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Multiply(const Matrix<R, C, T>& A, T B)
		{
			Matrix<R, C, T> Result;
			for (uint8 Index = 0; Index < Matrix<R, C, T>::Count; ++Index)
			{
				Result[Index] = A[Index] * B;
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Vector<C, T> Multiply(const Matrix<R, C, T>& A, Vector<C, T> B)
		{
			Vector<C, T> Result;
			for (uint8 Row = 0; Row < R; ++Row)
			{
				T Sum = (T)0;
				for (uint8 Index = 0; Index < C; ++Index)
				{
					Sum += A(Row, Index) * B[Index];
				}

				Result[Row] = Sum;
			}
			return Result;
		}

		template<uint8 R, uint8 C, int8 K, typename T>
		inline Matrix<R, C, T> Multiply(const Matrix<R, K, T>& A, const Matrix<K, C, T>& B)
		{
			Matrix<R, C, T> Result;
			for (uint8 Column = 0; Column < C; ++Column)
			{
				for (uint8 Row = 0; Row < R; ++Row)
				{
					T Sum = (T)0;
					for (uint8 Index = 0; Index < K; ++Index)
					{
						Sum += A(Row, Index) * B(Index, Column);
					}

					Result(Row, Column) = Sum;
				}
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Tensor(Vector<R, T>& A, Vector<C, T> B)
		{
			Matrix<R, C, T> Result;
			for (uint8 Row = 0; Row < R; ++Row)
			{
				for (uint8 Column = 0; Column < C; ++Column)
				{
					Result(Row, Column) = A[Row] * B[Column];
				}
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<C, R, T> Transpose(const Matrix<R, C, T>& M)
		{
			Matrix<C, R, T> Result;
			for (uint8 Column = 0; Column < C; ++Column)
			{
				for (uint8 Row = 0; Row < R; ++Row)
				{
					Result(Row, Column) = M(Column, Row);
				}
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Inverse(const Matrix<R, C, T>& M)
		{
			if constexpr (!Matrix<R, C, T>::Square)
			{
				return Matrix<R, C, T>::Zero;
			}

			float Det = Determinant(M);
			if (Math::Equals(Det, 0.0f))
			{
				return Matrix<R, C, T>::Zero;
			}

			Matrix<R, C, T> Result = Cofactor(M);
			Result = Result.Transpose();
			Result *= (1.0f / Det);
			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline Matrix<R, C, T> Cofactor(const Matrix<R, C, T>& M)
		{
			Matrix<R, C, T> Result;
			for (uint8 Column = 0; Column < C; ++Column)
			{
				for (uint8 Row = 0; Row < R; ++Row)
				{
					Result(Row, Column) = Cofactor(M, Row, Column);
				}
			}

			return Result;
		}

		template<uint8 R, uint8 C, typename T>
		inline float Cofactor(const Matrix<R, C, T>& M, uint8 Row, uint8 Column)
		{
			float Sign = (Row + Column) % 2 == 0 ? 1.0f : -1.0f;
			Matrix<R - 1, C - 1, T> Minor = M.GetMinor(Row, Column);
			return Sign * Determinant(Minor);
		}

		template<uint8 R, uint8 C, typename T>
		inline float Determinant(const Matrix<R, C, T>& M)
		{
			if constexpr (!Matrix<R, C, T>::Square)
			{
				return 0.0f;
			}
			else if constexpr (Matrix<R, C, T>::Dimension == 1)
			{
				return M[0];
			}
			else if constexpr (Matrix<R, C, T>::Dimension == 2)
			{
				return M[0] * M[3] - M[1] * M[2];
			}
			else if constexpr (Matrix<R, C, T>::Dimension == 3)
			{
				return M[0] * (M[4] * M[8] - M[7] * M[5]) - M[3] * (M[1] * M[8] - M[7] * M[2]) + M[6] * (M[1] * M[5] - M[4] * M[2]);
			}
			else
			{
				float Result = 0.0f;
				for (uint8 Column = 0; Column < C; ++Column)
				{
					Result += M(0, Column) * Cofactor(M, 0, Column);
				}

				return Result;
			}
		}

		template<uint8 R, uint8 C, typename T>
		inline float Trace(const Matrix<R, C, T>& M)
		{
			if constexpr (!Matrix<R, C, T>::Square)
			{
				return 0;
			}

			float Result = 0;
			for (uint8 Index = 0; Index < Matrix<R, C, T>::Dimension; ++Index)
			{
				Result += M(Index, Index);
			}

			return Result;
		}
	}

	namespace RotationUtility
	{
		inline Quaternion LookAt(Vector<3, float> Position, Vector<3, float> Target, Vector<3, float> Up)
		{
			return Matrix4x4f::LookAt(Position, Target, Up).GetRotation();
		}

		inline Quaternion LookAt(Vector<3, float> Direction, Vector<3, float> Up)
		{
			return Matrix4x4f::LookAt(Direction, Up).GetRotation();
		}
	}

#pragma endregion
}
