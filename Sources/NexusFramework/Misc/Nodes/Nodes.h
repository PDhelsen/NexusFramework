#pragma once

namespace NxFr
{
	namespace Node
	{
		template<typename T, typename N>
		static N* GetNode(T* Value)
		{
			return reinterpret_cast<N*>(Value);
		}

		template<typename T, typename N>
		static const N* GetNode(const T* Value)
		{
			return reinterpret_cast<const N*>(Value);
		}

		template<typename N>
		static N* RelinkBackward(N* Head)
		{
			N* Current = Head;
			Current->Prev = nullptr;

			while (Current != nullptr)
			{
				N* Prev = Current;
				Current = Current->Next;

				if (Current)
				{
					Current->Prev = Prev;
				}
				else
				{
					Prev->Next = nullptr;
					return Prev;
				}
			}

			return nullptr;
		}
	}

	template<typename K, typename T>
	class KeyValuePair
	{
	public:
		KeyValuePair()
			: Key(), Value()
		{

		}

		KeyValuePair(const K& Key, const T& Value)
			: Key(Key), Value(Value)
		{

		}

		KeyValuePair(K&& Key, T&& Value)
			: Key(Move(Key)), Value(Move(Value))
		{

		}

		template<typename... Args>
		KeyValuePair(const K& Key, Args&&... args)
			: Key(Key), Value(args...)
		{

		}

		template<typename... Args>
		KeyValuePair(K&& Key, Args&&... args)
			: Key(Move(Key)), Value(args...)
		{

		}

		KeyValuePair(const KeyValuePair<K, T>& Other)
			: Key(Other.Key), Value(Other.Value)
		{
		}

		KeyValuePair(KeyValuePair<K, T>&& Other) noexcept
			: Key(Move(Other.Key)), Value(Move(Other.Value))
		{
		}

		KeyValuePair<K, T>& operator=(const KeyValuePair<K, T>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Key = Other.Key;
			Value = Other.Value;

			return *this;
		}

		KeyValuePair<K, T>& operator=(KeyValuePair<K, T>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			Key = Move(Other.Key);
			Value = Move(Other.Value);

			return *this;
		}

		bool operator==(const KeyValuePair<K, T>& Other) const
		{
			return Key == Other.Key && Value == Other.Value;
		}

		bool operator!=(const KeyValuePair<K, T>& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const KeyValuePair<K, T>& Other) const
		{
			return Key > Other.Key;
		}

		bool operator>=(const KeyValuePair<K, T>& Other) const
		{
			return Key >= Other.Key;
		}

		bool operator<(const KeyValuePair<K, T>& Other) const
		{
			return Key < Other.Key;
		}

		bool operator<=(const KeyValuePair<K, T>& Other) const
		{
			return Key <= Other.Key;
		}

		K Key;
		T Value;
	};
}
