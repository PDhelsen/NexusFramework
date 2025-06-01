#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Containers/Buffer.h"

#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Math/Rotation.h"
#include "NexusFramework/Math/Matrix.h"
#include "NexusFramework/Math/Shape.h"

namespace NxFr
{
	class RBS
	{
	public:
		struct OffsetScope
		{
		public:
			NEXUS_FRAMEWORK_API OffsetScope(RBS& Rbs, uint64 Offset);
			NEXUS_FRAMEWORK_API ~OffsetScope();

		private:
			RBS& Rbs;
			uint64 Cursor;
		};

		NEXUS_FRAMEWORK_API static Buffer Serialize(const RBS& Data);
		NEXUS_FRAMEWORK_API static void SerializeFile(const RBS& Data, StringView Path);
		NEXUS_FRAMEWORK_API static RBS Deserialize(BufferView Data);
		NEXUS_FRAMEWORK_API static RBS DeserializeFile(StringView Path);

		NEXUS_FRAMEWORK_API RBS();
		NEXUS_FRAMEWORK_API RBS(BufferView View);
		NEXUS_FRAMEWORK_API RBS(Buffer&& Data);
		NEXUS_FRAMEWORK_API ~RBS();

		template<typename T>
		T ReadObject();
		template<typename T>
		T ReadObject(uint64 Offset);
		template<typename T>
		void WriteObject(const T& Object);
		template<typename T>
		void WriteObject(const T& Object, uint64 Offset);

		template<typename T>
		const T* ReadData(uint64 Size);
		template<typename T>
		const T* ReadData(uint64 Size, uint64 Offset);
		template<typename T>
		void WriteData(const T* Pointer, uint64 Size);
		template<typename T>
		void WriteData(const T* Pointer, uint64 Size, uint64 Offset);

		NEXUS_FRAMEWORK_API const void* ReadByte(uint64 Size);
		NEXUS_FRAMEWORK_API const void* ReadByte(uint64 Size, uint64 Offset);
		NEXUS_FRAMEWORK_API void WriteByte(const void* Pointer, uint64 Size);
		NEXUS_FRAMEWORK_API void WriteByte(const void* Pointer, uint64 Size, uint64 Offset);

		NEXUS_FRAMEWORK_API uint64 GetCursor() const { return Cursor; }
		NEXUS_FRAMEWORK_API void SetCursor(uint64 Offset) { Cursor = Offset; }

	private:
		Buffer Data;
		uint64 Cursor;
	};

	template<typename T>
	struct RBSConverter
	{
	public:
		static T Decode(RBS& Rbs);
		static void Encode(RBS& Rbs, const T& Object);
	};

#pragma region Templates

#pragma region RBS

	template<typename T>
	inline T RBS::ReadObject()
	{
		return RBSConverter<T>::Decode(*this);
	}

	template<typename T>
	inline T RBS::ReadObject(uint64 Offset)
	{
		OffsetScope CursorOffset(*this, Offset);
		return ReadObject();
	}

	template<typename T>
	inline void RBS::WriteObject(const T& Object)
	{
		RBSConverter<T>::Encode(*this, Object);
	}

	template<typename T>
	inline void RBS::WriteObject(const T& Object, uint64 Offset)
	{
		OffsetScope CursorOffset(*this, Offset);
		WriteObject(Object);
	}

	template<typename T>
	inline const T* RBS::ReadData(uint64 Size)
	{
		return (const T*)ReadByte(Size);
	}

	template<typename T>
	inline const T* RBS::ReadData(uint64 Size, uint64 Offset)
	{
		return (const T*)ReadByte(Size, Offset);
	}

	template<typename T>
	inline void RBS::WriteData(const T* Pointer, uint64 Size)
	{
		WriteByte(Pointer, Size);
	}

	template<typename T>
	inline void RBS::WriteData(const T* Pointer, uint64 Size, uint64 Offset)
	{
		WriteByte(Pointer, Size, Offset);
	}

#pragma endregion

#pragma region Converter

	template<typename T>
	inline T RBSConverter<T>::Decode(RBS& Rbs)
	{
		return *Rbs.ReadData<T>(sizeof(T));
	}

	template<typename T>
	inline void RBSConverter<T>::Encode(RBS& Rbs, const T& Object)
	{
		Rbs.WriteData<T>(&Object, sizeof(T));
	}

#pragma endregion

#pragma region Specialization

	template<>
	struct RBSConverter<const char*>
	{
		static const char* Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			const char* Text = Rbs.ReadData<char>(Size);
			Rbs.ReadByte(sizeof(StringCApi::NullChar));
			return Text;
		}

		static void Encode(RBS& Rbs, const char* Object)
		{
			uint64 Size = StringCApi::Length(Object);

			Rbs.WriteObject(Size);
			Rbs.WriteData(Object, StringCApi::Length(Object));
			Rbs.WriteObject(StringCApi::NullChar);
		}
	};

	template<>
	struct RBSConverter<String>
	{
		static String Decode(RBS& Rbs)
		{
			return Rbs.ReadObject<StringView>().ToString();
		}

		static void Encode(RBS& Rbs, const String& Object)
		{
			Rbs.WriteObject(Object.ToView());
		}
	};

	template<>
	struct RBSConverter<StringView>
	{
		static StringView Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			const char* Text = Rbs.ReadData<char>(Size);
			Rbs.ReadByte(sizeof(StringCApi::NullChar));
			return StringView(Text, Size);
		}

		static void Encode(RBS& Rbs, const StringView& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			Rbs.WriteData(Object.C(), Object.GetCount());
			Rbs.WriteObject(StringCApi::NullChar);
		}
	};

	template<>
	struct RBSConverter<StringId>
	{
		static StringId Decode(RBS& Rbs)
		{
			return StringId(Rbs.ReadObject<StringView>());
		}

		static void Encode(RBS& Rbs, const StringId& Object)
		{
			Rbs.WriteObject(Object.C());
		}
	};

	template<typename T>
	struct RBSConverter<Array<T>>
	{
		static Array<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Array<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result[Index] = Rbs.ReadObject<T>();
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Array<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (auto& It : Object)
			{
				Rbs.WriteObject(It);
			}
		}
	};

	template<typename T>
	struct RBSConverter<List<T>>
	{
		static List<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			List<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result.Append(Rbs.ReadObject<T>());
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const List<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (auto& It : Object)
			{
				Rbs.WriteObject(It);
			}
		}
	};

	template<typename T>
	struct RBSConverter<Set<T>>
	{
		static Set<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Set<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result.Append(Rbs.ReadObject<T>());
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Set<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (auto& It : Object)
			{
				Rbs.WriteObject(It);
			}
		}
	};

	template<typename K, typename T>
	struct RBSConverter<Dictionary<K, T>>
	{
		static Dictionary<K, T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Dictionary<K, T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				K Key = Rbs.ReadObject<K>();
				T Value = Rbs.ReadObject<T>();
				Result.Append(Move(Key), Move(Value));
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Dictionary<K, T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (auto& It : Object)
			{
				Rbs.WriteObject(It.Key);
				Rbs.WriteObject(It.Value);
			}
		}
	};

	template<uint8 D, typename T>
	struct RBSConverter<Vector<D, T>>
	{
		static Vector<D, T> Decode(RBS& Rbs)
		{
			Vector<D, T> Result;
			for (uint8 I = 0; I < D; ++I)
			{
				Result[I] = Rbs.ReadObject<T>();
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Vector<D, T>& Object)
		{
			for (uint8 I = 0; I < D; ++I)
			{
				Rbs.WriteObject(Object[I]);
			}
		}
	};

	template<>
	struct RBSConverter<Euler>
	{
		static Euler Decode(RBS& Rbs)
		{
			Euler Result;
			Result.x = Rbs.ReadObject<float>();
			Result.y = Rbs.ReadObject<float>();
			Result.z = Rbs.ReadObject<float>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Euler& Object)
		{
			Rbs.WriteObject(Object.x);
			Rbs.WriteObject(Object.y);
			Rbs.WriteObject(Object.z);
		}
	};

	template<>
	struct RBSConverter<AxisAngle>
	{
		static AxisAngle Decode(RBS& Rbs)
		{
			AxisAngle Result;
			Result.Angle = Rbs.ReadObject<float>();
			Result.Axis.x = Rbs.ReadObject<float>();
			Result.Axis.y = Rbs.ReadObject<float>();
			Result.Axis.z = Rbs.ReadObject<float>();
			return Result;
		}

		static void Encode(RBS& Rbs, const AxisAngle& Object)
		{
			Rbs.WriteObject(Object.Angle);
			Rbs.WriteObject(Object.Axis.x);
			Rbs.WriteObject(Object.Axis.y);
			Rbs.WriteObject(Object.Axis.z);
		}
	};

	template<>
	struct RBSConverter<Quaternion>
	{
		static Quaternion Decode(RBS& Rbs)
		{
			Quaternion Result;
			Result.w = Rbs.ReadObject<float>();
			Result.x = Rbs.ReadObject<float>();
			Result.y = Rbs.ReadObject<float>();
			Result.z = Rbs.ReadObject<float>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Quaternion& Object)
		{
			Rbs.WriteObject(Object.w);
			Rbs.WriteObject(Object.x);
			Rbs.WriteObject(Object.y);
			Rbs.WriteObject(Object.z);
		}
	};

	template<uint8 R, uint8 C, typename T>
	struct RBSConverter<Matrix<R, C, T>>
	{
		static Matrix<R, C, T> Decode(RBS& Rbs)
		{
			Matrix<R, C, T> Result;
			for (uint8 I = 0; I < Matrix<R, C, T>::Count; ++I)
			{
				Result[I] = Rbs.ReadObject<T>();
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Matrix<R, C, T>& Object)
		{
			for (uint8 I = 0; I < Matrix<R, C, T>::Count; ++I)
			{
				Rbs.WriteObject(Object[I]);
			}
		}
	};

	template<>
	struct RBSConverter<Ray>
	{
		static Ray Decode(RBS& Rbs)
		{
			Ray Result;
			Result.Origin = Rbs.ReadObject<Vector3f>();
			Result.Direction = Rbs.ReadObject<Vector3f>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Ray& Object)
		{
			Rbs.WriteObject(Object.Origin);
			Rbs.WriteObject(Object.Direction);
		}
	};

	template<>
	struct RBSConverter<Plane>
	{
		static Plane Decode(RBS& Rbs)
		{
			Plane Result;
			Result.Normal = Rbs.ReadObject<Vector3f>();
			Result.Distance = Rbs.ReadObject<float>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Plane& Object)
		{
			Rbs.WriteObject(Object.Normal);
			Rbs.WriteObject(Object.Distance);
		}
	};

	template<>
	struct RBSConverter<Triangle>
	{
		static Triangle Decode(RBS& Rbs)
		{
			Triangle Result;
			Result.A = Rbs.ReadObject<Vector3f>();
			Result.B = Rbs.ReadObject<Vector3f>();
			Result.C = Rbs.ReadObject<Vector3f>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Triangle& Object)
		{
			Rbs.WriteObject(Object.A);
			Rbs.WriteObject(Object.B);
			Rbs.WriteObject(Object.C);
		}
	};

	template<>
	struct RBSConverter<Rectangle>
	{
		static Rectangle Decode(RBS& Rbs)
		{
			Rectangle Result;
			Result.Center = Rbs.ReadObject<Vector2f>();
			Result.Extents = Rbs.ReadObject<Vector2f>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Rectangle& Object)
		{
			Rbs.WriteObject(Object.Center);
			Rbs.WriteObject(Object.Extents);
		}
	};

	template<>
	struct RBSConverter<Circle>
	{
		static Circle Decode(RBS& Rbs)
		{
			Circle Result;
			Result.Center = Rbs.ReadObject<Vector2f>();
			Result.Radius = Rbs.ReadObject<float>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Circle& Object)
		{
			Rbs.WriteObject(Object.Center);
			Rbs.WriteObject(Object.Radius);
		}
	};

	template<>
	struct RBSConverter<Box>
	{
		static Box Decode(RBS& Rbs)
		{
			Box Result;
			Result.Center = Rbs.ReadObject<Vector3f>();
			Result.Extents = Rbs.ReadObject<Vector3f>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Box& Object)
		{
			Rbs.WriteObject(Object.Center);
			Rbs.WriteObject(Object.Extents);
		}
	};

	template<>
	struct RBSConverter<Cuboid>
	{
		static Cuboid Decode(RBS& Rbs)
		{
			Cuboid Result;
			Result.Center = Rbs.ReadObject<Vector3f>();
			Result.Orientation = Rbs.ReadObject<Quaternion>();
			Result.Extents = Rbs.ReadObject<Vector3f>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Cuboid& Object)
		{
			Rbs.WriteObject(Object.Center);
			Rbs.WriteObject(Object.Orientation);
			Rbs.WriteObject(Object.Extents);
		}
	};

	template<>
	struct RBSConverter<Sphere>
	{
		static Sphere Decode(RBS& Rbs)
		{
			Sphere Result;
			Result.Center = Rbs.ReadObject<Vector3f>();
			Result.Radius = Rbs.ReadObject<float>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Sphere& Object)
		{
			Rbs.WriteObject(Object.Center);
			Rbs.WriteObject(Object.Radius);
		}
	};

#pragma endregion

#pragma endregion
}
