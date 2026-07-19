#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Types/Containers/Misc/Iterator.h"

namespace NxFr
{
	namespace RBSUtility
	{
		template<typename T>
		struct Converter;
	}

	class NX_FRAMEWORK_API RBS
	{
	public:
		static Buffer Serialize(const RBS& Data);
		static void SerializeAndSave(const RBS& Data, StringView Path);
		static RBS Deserialize(BufferView Data);
		static RBS LoadAndDeserialize(StringView Path);

		RBS();
		RBS(BufferView View);
		RBS(Buffer&& Data);

		template<typename T>
		T ReadObject() const
		{
			return RBSUtility::Converter<T>::Decode(*this);
		}
		template<typename T>
		void WriteObject(const T& Object)
		{
			RBSUtility::Converter<T>::Encode(*this, Object);
		}

		template<typename T>
		const T* ReadData(uint64 Size) const
		{
			return (const T*)ReadByte(Size);
		}
		template<typename T>
		void WriteData(const T* Pointer, uint64 Size)
		{
			WriteByte(Pointer, Size);
		}
		
		const void* ReadByte(uint64 Size) const;
		void WriteByte(const void* Pointer, uint64 Size);

		const Iterator::Pointer Begin() const;
		const Iterator::Pointer End() const;

		uint64 GetCursor() const { return Cursor; }
		void SetCursor(uint64 Offset) const { Cursor = Offset; }

	private:
		Buffer Data;
		mutable uint64 Cursor;
	};

	namespace RBSUtility
	{
		template<typename T>
		struct Converter
		{
		public:
			static T Decode(const RBS& Rbs)
			{
				return *Rbs.ReadData<T>(sizeof(T));
			}

			static void Encode(RBS& Rbs, const T& Object)
			{
				Rbs.WriteData<T>(&Object, sizeof(T));
			}
		};
	}
}
