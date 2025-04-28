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
#pragma endregion

#pragma endregion
}
