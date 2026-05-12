#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Containers/Buffer.h"

namespace NxFr
{
	template<typename T>
	struct RBSConverter;

	class NEXUS_FRAMEWORK_API RBS
	{
	public:
		struct NEXUS_FRAMEWORK_API OffsetScope
		{
		public:
			NEXUS_NOCOPY_NOMOVE(OffsetScope)
			OffsetScope(RBS& Rbs, uint64 Offset);
			~OffsetScope();

		private:
			RBS& Rbs;
			uint64 Cursor;
		};

		static Buffer Serialize(const RBS& Data);
		static void SerializeFile(const RBS& Data, StringView Path);
		static RBS Deserialize(BufferView Data);
		static RBS DeserializeFile(StringView Path);

		RBS();
		RBS(BufferView View);
		RBS(Buffer&& Data);

		template<typename T>
		T ReadObject()
		{
			return RBSConverter<T>::Decode(*this);
		}
		template<typename T>
		T ReadObject(uint64 Offset)
		{
			OffsetScope CursorOffset(*this, Offset);
			return ReadObject();
		}
		template<typename T>
		void WriteObject(const T& Object)
		{
			RBSConverter<T>::Encode(*this, Object);
		}
		template<typename T>
		void WriteObject(const T& Object, uint64 Offset)
		{
			OffsetScope CursorOffset(*this, Offset);
			WriteObject(Object);
		}

		template<typename T>
		const T* ReadData(uint64 Size)
		{
			return (const T*)ReadByte(Size);
		}
		template<typename T>
		const T* ReadData(uint64 Size, uint64 Offset)
		{
			return (const T*)ReadByte(Size, Offset);
		}
		template<typename T>
		void WriteData(const T* Pointer, uint64 Size)
		{
			WriteByte(Pointer, Size);
		}
		template<typename T>
		void WriteData(const T* Pointer, uint64 Size, uint64 Offset)
		{
			WriteByte(Pointer, Size, Offset);
		}

		const void* ReadByte(uint64 Size);
		const void* ReadByte(uint64 Size, uint64 Offset);
		void WriteByte(const void* Pointer, uint64 Size);
		void WriteByte(const void* Pointer, uint64 Size, uint64 Offset);

		const Iterator::IteratorPointer Begin() const;
		const Iterator::IteratorPointer End() const;

		uint64 GetCursor() const { return Cursor; }
		void SetCursor(uint64 Offset) { Cursor = Offset; }

	private:
		Buffer Data;
		uint64 Cursor;
	};

	template<typename T>
	struct RBSConverter
	{
	public:
		static T Decode(RBS& Rbs)
		{
			return *Rbs.ReadData<T>(sizeof(T));
		}

		static void Encode(RBS& Rbs, const T& Object)
		{
			Rbs.WriteData<T>(&Object, sizeof(T));
		}
	};
}
