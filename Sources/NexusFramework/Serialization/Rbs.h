#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Containers/Buffer.h"

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

		NEXUS_FRAMEWORK_API const Iterator::IteratorPointer Begin() const;
		NEXUS_FRAMEWORK_API const Iterator::IteratorPointer End() const;

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

#pragma endregion
}
