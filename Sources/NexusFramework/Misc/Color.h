#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Decimal.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Serialization/Rbs.h"
#include "NexusFramework/Serialization/Yaml.h"

namespace NxFr
{
	namespace Colors
	{
		struct sRGB;
		struct Linear;
		struct Bits;
		struct Hsv;
	}

	using Color = Colors::sRGB;

	namespace Colors
	{
		NEXUS_FRAMEWORK_API extern const Color Clear;
		NEXUS_FRAMEWORK_API extern const Color Black;
		NEXUS_FRAMEWORK_API extern const Color Gray;
		NEXUS_FRAMEWORK_API extern const Color White;
		NEXUS_FRAMEWORK_API extern const Color Red;
		NEXUS_FRAMEWORK_API extern const Color Green;
		NEXUS_FRAMEWORK_API extern const Color Blue;
		NEXUS_FRAMEWORK_API extern const Color Yellow;
		NEXUS_FRAMEWORK_API extern const Color Cyan;
		NEXUS_FRAMEWORK_API extern const Color Magenta;
		NEXUS_FRAMEWORK_API extern const Color Orange;
	}

	namespace Colors
	{
		struct sRGB
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			NEXUS_FRAMEWORK_API sRGB();
			NEXUS_FRAMEWORK_API sRGB(Type V);
			NEXUS_FRAMEWORK_API sRGB(Type R, Type G, Type B);
			NEXUS_FRAMEWORK_API sRGB(Type R, Type G, Type B, Type A);
			NEXUS_FRAMEWORK_API sRGB(Vector3f V);
			NEXUS_FRAMEWORK_API sRGB(Vector4f V);
			NEXUS_FRAMEWORK_API sRGB(Linear Other);
			NEXUS_FRAMEWORK_API sRGB(Bits Other);
			NEXUS_FRAMEWORK_API sRGB(Hsv Other);

			NEXUS_FRAMEWORK_API Vector3f ToVector3f() const;
			NEXUS_FRAMEWORK_API Vector4f ToVector4f() const;
			NEXUS_FRAMEWORK_API float ToGrayscale() const;
			NEXUS_FRAMEWORK_API Linear ToLinear() const;
			NEXUS_FRAMEWORK_API Bits ToBits() const;
			NEXUS_FRAMEWORK_API Hsv ToHsv() const;

			NEXUS_FRAMEWORK_API bool operator==(sRGB Other) const;
			NEXUS_FRAMEWORK_API bool operator!=(sRGB Other) const;
			NEXUS_FRAMEWORK_API Type& operator[](uint64 Index);
			NEXUS_FRAMEWORK_API const Type& operator[](uint64 Index) const;

			NEXUS_FRAMEWORK_API sRGB& operator+=(Type Other);
			NEXUS_FRAMEWORK_API sRGB& operator-=(Type Other);
			NEXUS_FRAMEWORK_API sRGB& operator*=(Type Other);
			NEXUS_FRAMEWORK_API sRGB& operator/=(Type Other);
			NEXUS_FRAMEWORK_API sRGB& operator+=(sRGB Other);
			NEXUS_FRAMEWORK_API sRGB& operator-=(sRGB Other);
			NEXUS_FRAMEWORK_API sRGB& operator*=(sRGB Other);
			NEXUS_FRAMEWORK_API sRGB& operator/=(sRGB Other);
			
			NEXUS_FRAMEWORK_API Type Min() const;
			NEXUS_FRAMEWORK_API Type Max() const;
			NEXUS_FRAMEWORK_API sRGB Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;
			
			NEXUS_FRAMEWORK_API Type& GetX();
			NEXUS_FRAMEWORK_API Type& GetY();
			NEXUS_FRAMEWORK_API Type& GetZ();
			NEXUS_FRAMEWORK_API Type& GetW();
			NEXUS_FRAMEWORK_API const Type& GetX() const;
			NEXUS_FRAMEWORK_API const Type& GetY() const;
			NEXUS_FRAMEWORK_API const Type& GetZ() const;
			NEXUS_FRAMEWORK_API const Type& GetW() const;
			NEXUS_FRAMEWORK_API void SetX(Type X);
			NEXUS_FRAMEWORK_API void SetY(Type Y);
			NEXUS_FRAMEWORK_API void SetZ(Type Z);
			NEXUS_FRAMEWORK_API void SetW(Type W);

			Type r, g, b, a;
		};

		NEXUS_FRAMEWORK_API sRGB operator+(sRGB Instance, sRGB::Type Other);
		NEXUS_FRAMEWORK_API sRGB operator-(sRGB Instance, sRGB::Type Other);
		NEXUS_FRAMEWORK_API sRGB operator*(sRGB Instance, sRGB::Type Other);
		NEXUS_FRAMEWORK_API sRGB operator/(sRGB Instance, sRGB::Type Other);
		NEXUS_FRAMEWORK_API sRGB operator+(sRGB Instance, sRGB Other);
		NEXUS_FRAMEWORK_API sRGB operator-(sRGB Instance, sRGB Other);
		NEXUS_FRAMEWORK_API sRGB operator*(sRGB Instance, sRGB Other);
		NEXUS_FRAMEWORK_API sRGB operator/(sRGB Instance, sRGB Other);

		struct Linear
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			NEXUS_FRAMEWORK_API Linear();
			NEXUS_FRAMEWORK_API Linear(Type V);
			NEXUS_FRAMEWORK_API Linear(Type R, Type G, Type B);
			NEXUS_FRAMEWORK_API Linear(Type R, Type G, Type B, Type A);
			NEXUS_FRAMEWORK_API Linear(Vector3f V);
			NEXUS_FRAMEWORK_API Linear(Vector4f V);
			NEXUS_FRAMEWORK_API Linear(sRGB Other);
			NEXUS_FRAMEWORK_API Linear(Bits Other);
			NEXUS_FRAMEWORK_API Linear(Hsv Other);

			NEXUS_FRAMEWORK_API Vector3f ToVector3f() const;
			NEXUS_FRAMEWORK_API Vector4f ToVector4f() const;
			NEXUS_FRAMEWORK_API float ToGrayscale() const;
			NEXUS_FRAMEWORK_API sRGB TosRGB() const;
			NEXUS_FRAMEWORK_API Bits ToBits() const;
			NEXUS_FRAMEWORK_API Hsv ToHsv() const;

			NEXUS_FRAMEWORK_API bool operator==(Linear Other) const;
			NEXUS_FRAMEWORK_API bool operator!=(Linear Other) const;
			NEXUS_FRAMEWORK_API Type& operator[](uint64 Index);
			NEXUS_FRAMEWORK_API const Type& operator[](uint64 Index) const;

			NEXUS_FRAMEWORK_API Linear& operator+=(Type Other);
			NEXUS_FRAMEWORK_API Linear& operator-=(Type Other);
			NEXUS_FRAMEWORK_API Linear& operator*=(Type Other);
			NEXUS_FRAMEWORK_API Linear& operator/=(Type Other);
			NEXUS_FRAMEWORK_API Linear& operator+=(Linear Other);
			NEXUS_FRAMEWORK_API Linear& operator-=(Linear Other);
			NEXUS_FRAMEWORK_API Linear& operator*=(Linear Other);
			NEXUS_FRAMEWORK_API Linear& operator/=(Linear Other);

			NEXUS_FRAMEWORK_API Type Min() const;
			NEXUS_FRAMEWORK_API Type Max() const;
			NEXUS_FRAMEWORK_API Linear Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			NEXUS_FRAMEWORK_API Type& GetX();
			NEXUS_FRAMEWORK_API Type& GetY();
			NEXUS_FRAMEWORK_API Type& GetZ();
			NEXUS_FRAMEWORK_API Type& GetW();
			NEXUS_FRAMEWORK_API const Type& GetX() const;
			NEXUS_FRAMEWORK_API const Type& GetY() const;
			NEXUS_FRAMEWORK_API const Type& GetZ() const;
			NEXUS_FRAMEWORK_API const Type& GetW() const;
			NEXUS_FRAMEWORK_API void SetX(Type X);
			NEXUS_FRAMEWORK_API void SetY(Type Y);
			NEXUS_FRAMEWORK_API void SetZ(Type Z);
			NEXUS_FRAMEWORK_API void SetW(Type W);

			Type r, g, b, a;
		};

		NEXUS_FRAMEWORK_API Linear operator+(Linear Instance, Linear::Type Other);
		NEXUS_FRAMEWORK_API Linear operator-(Linear Instance, Linear::Type Other);
		NEXUS_FRAMEWORK_API Linear operator*(Linear Instance, Linear::Type Other);
		NEXUS_FRAMEWORK_API Linear operator/(Linear Instance, Linear::Type Other);
		NEXUS_FRAMEWORK_API Linear operator+(Linear Instance, Linear Other);
		NEXUS_FRAMEWORK_API Linear operator-(Linear Instance, Linear Other);
		NEXUS_FRAMEWORK_API Linear operator*(Linear Instance, Linear Other);
		NEXUS_FRAMEWORK_API Linear operator/(Linear Instance, Linear Other);

		struct Bits
		{
			using Type = uint8;

			static inline Type BoundLow = 0;
			static inline Type BoundHigh = 255;

			NEXUS_FRAMEWORK_API Bits();
			NEXUS_FRAMEWORK_API Bits(Type V);
			NEXUS_FRAMEWORK_API Bits(Type R, Type G, Type B);
			NEXUS_FRAMEWORK_API Bits(Type R, Type G, Type B, Type A);
			NEXUS_FRAMEWORK_API Bits(Vector3f V);
			NEXUS_FRAMEWORK_API Bits(Vector4f V);
			NEXUS_FRAMEWORK_API Bits(sRGB Other);
			NEXUS_FRAMEWORK_API Bits(Linear Other);
			NEXUS_FRAMEWORK_API Bits(Hsv Other);

			NEXUS_FRAMEWORK_API Vector3f ToVector3f() const;
			NEXUS_FRAMEWORK_API Vector4f ToVector4f() const;
			NEXUS_FRAMEWORK_API float ToGrayscale() const;
			NEXUS_FRAMEWORK_API sRGB TosRGB() const;
			NEXUS_FRAMEWORK_API Linear ToLinear() const;
			NEXUS_FRAMEWORK_API Hsv ToHsv() const;

			NEXUS_FRAMEWORK_API bool operator==(Bits Other) const;
			NEXUS_FRAMEWORK_API bool operator!=(Bits Other) const;
			NEXUS_FRAMEWORK_API Type& operator[](uint64 Index);
			NEXUS_FRAMEWORK_API const Type& operator[](uint64 Index) const;

			NEXUS_FRAMEWORK_API Bits& operator+=(Type Other);
			NEXUS_FRAMEWORK_API Bits& operator-=(Type Other);
			NEXUS_FRAMEWORK_API Bits& operator*=(Type Other);
			NEXUS_FRAMEWORK_API Bits& operator/=(Type Other);
			NEXUS_FRAMEWORK_API Bits& operator+=(Bits Other);
			NEXUS_FRAMEWORK_API Bits& operator-=(Bits Other);
			NEXUS_FRAMEWORK_API Bits& operator*=(Bits Other);
			NEXUS_FRAMEWORK_API Bits& operator/=(Bits Other);

			NEXUS_FRAMEWORK_API Type Min() const;
			NEXUS_FRAMEWORK_API Type Max() const;
			NEXUS_FRAMEWORK_API Bits Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			NEXUS_FRAMEWORK_API Type& GetX();
			NEXUS_FRAMEWORK_API Type& GetY();
			NEXUS_FRAMEWORK_API Type& GetZ();
			NEXUS_FRAMEWORK_API Type& GetW();
			NEXUS_FRAMEWORK_API const Type& GetX() const;
			NEXUS_FRAMEWORK_API const Type& GetY() const;
			NEXUS_FRAMEWORK_API const Type& GetZ() const;
			NEXUS_FRAMEWORK_API const Type& GetW() const;
			NEXUS_FRAMEWORK_API void SetX(Type X);
			NEXUS_FRAMEWORK_API void SetY(Type Y);
			NEXUS_FRAMEWORK_API void SetZ(Type Z);
			NEXUS_FRAMEWORK_API void SetW(Type W);

			Type r, g, b, a;
		};

		NEXUS_FRAMEWORK_API Bits operator+(Bits Instance, Bits::Type Other);
		NEXUS_FRAMEWORK_API Bits operator-(Bits Instance, Bits::Type Other);
		NEXUS_FRAMEWORK_API Bits operator*(Bits Instance, Bits::Type Other);
		NEXUS_FRAMEWORK_API Bits operator/(Bits Instance, Bits::Type Other);
		NEXUS_FRAMEWORK_API Bits operator+(Bits Instance, Bits Other);
		NEXUS_FRAMEWORK_API Bits operator-(Bits Instance, Bits Other);
		NEXUS_FRAMEWORK_API Bits operator*(Bits Instance, Bits Other);
		NEXUS_FRAMEWORK_API Bits operator/(Bits Instance, Bits Other);

		struct Hsv
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			NEXUS_FRAMEWORK_API Hsv();
			NEXUS_FRAMEWORK_API Hsv(Type V);
			NEXUS_FRAMEWORK_API Hsv(Type H, Type S, Type V);
			NEXUS_FRAMEWORK_API Hsv(Type H, Type S, Type V, Type A);
			NEXUS_FRAMEWORK_API Hsv(Vector3f V);
			NEXUS_FRAMEWORK_API Hsv(Vector4f V);
			NEXUS_FRAMEWORK_API Hsv(sRGB Other);
			NEXUS_FRAMEWORK_API Hsv(Linear Other);
			NEXUS_FRAMEWORK_API Hsv(Bits Other);

			NEXUS_FRAMEWORK_API Vector3f ToVector3f() const;
			NEXUS_FRAMEWORK_API Vector4f ToVector4f() const;
			NEXUS_FRAMEWORK_API float ToGrayscale() const;
			NEXUS_FRAMEWORK_API sRGB TosRGB() const;
			NEXUS_FRAMEWORK_API Linear ToLinear() const;
			NEXUS_FRAMEWORK_API Bits ToBits() const;

			NEXUS_FRAMEWORK_API bool operator==(Hsv Other) const;
			NEXUS_FRAMEWORK_API bool operator!=(Hsv Other) const;
			NEXUS_FRAMEWORK_API Type& operator[](uint64 Index);
			NEXUS_FRAMEWORK_API const Type& operator[](uint64 Index) const;

			NEXUS_FRAMEWORK_API Hsv& operator+=(Type Other);
			NEXUS_FRAMEWORK_API Hsv& operator-=(Type Other);
			NEXUS_FRAMEWORK_API Hsv& operator*=(Type Other);
			NEXUS_FRAMEWORK_API Hsv& operator/=(Type Other);
			NEXUS_FRAMEWORK_API Hsv& operator+=(Hsv Other);
			NEXUS_FRAMEWORK_API Hsv& operator-=(Hsv Other);
			NEXUS_FRAMEWORK_API Hsv& operator*=(Hsv Other);
			NEXUS_FRAMEWORK_API Hsv& operator/=(Hsv Other);

			NEXUS_FRAMEWORK_API Type Min() const;
			NEXUS_FRAMEWORK_API Type Max() const;
			NEXUS_FRAMEWORK_API Hsv Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			NEXUS_FRAMEWORK_API Type& GetX();
			NEXUS_FRAMEWORK_API Type& GetY();
			NEXUS_FRAMEWORK_API Type& GetZ();
			NEXUS_FRAMEWORK_API Type& GetW();
			NEXUS_FRAMEWORK_API const Type& GetX() const;
			NEXUS_FRAMEWORK_API const Type& GetY() const;
			NEXUS_FRAMEWORK_API const Type& GetZ() const;
			NEXUS_FRAMEWORK_API const Type& GetW() const;
			NEXUS_FRAMEWORK_API void SetX(Type X);
			NEXUS_FRAMEWORK_API void SetY(Type Y);
			NEXUS_FRAMEWORK_API void SetZ(Type Z);
			NEXUS_FRAMEWORK_API void SetW(Type W);

			Type h, s, v, a;
		};

		NEXUS_FRAMEWORK_API Hsv operator+(Hsv Instance, Hsv::Type Other);
		NEXUS_FRAMEWORK_API Hsv operator-(Hsv Instance, Hsv::Type Other);
		NEXUS_FRAMEWORK_API Hsv operator*(Hsv Instance, Hsv::Type Other);
		NEXUS_FRAMEWORK_API Hsv operator/(Hsv Instance, Hsv::Type Other);
		NEXUS_FRAMEWORK_API Hsv operator+(Hsv Instance, Hsv Other);
		NEXUS_FRAMEWORK_API Hsv operator-(Hsv Instance, Hsv Other);
		NEXUS_FRAMEWORK_API Hsv operator*(Hsv Instance, Hsv Other);
		NEXUS_FRAMEWORK_API Hsv operator/(Hsv Instance, Hsv Other);

		namespace Utility
		{
			NEXUS_FRAMEWORK_API float ToGrayscale(sRGB Instance);
			NEXUS_FRAMEWORK_API sRGB TosRGB(Linear Instance);
			NEXUS_FRAMEWORK_API sRGB TosRGB(Bits Instance);
			NEXUS_FRAMEWORK_API sRGB TosRGB(Hsv Instance);
			NEXUS_FRAMEWORK_API Linear ToLinear(sRGB Instance);
			NEXUS_FRAMEWORK_API Bits ToBits(sRGB Instance);
			NEXUS_FRAMEWORK_API Hsv ToHsv(sRGB Instance);
			NEXUS_FRAMEWORK_API sRGB ToHue(float Hue);

			template<typename T>
			Vector3f ToVector3f(T Instance)
			{
				return Vector3f(Instance.GetX(), Instance.GetY(), Instance.GetZ());
			}

			template<typename T>
			Vector4f ToVector4f(T Instance)
			{
				return Vector4f(Instance.GetX(), Instance.GetY(), Instance.GetZ(), Instance.GetW());
			}

			template<typename T>
			typename T::Type& Access(T& Instance, uint64 Index)
			{
				return Index == 0 ? Instance.GetX() : Index == 1 ? Instance.GetY() : Index == 2 ? Instance.GetZ() : Index == 3 ? Instance.GetW() : T::BoundLow;
			}

			template<typename T>
			const typename T::Type& Access(const T& Instance, uint64 Index)
			{
				return Index == 0 ? Instance.GetX() : Index == 1 ? Instance.GetY() : Index == 2 ? Instance.GetZ() : Index == 3 ? Instance.GetW() : T::BoundLow;
			}

			template<typename T>
			bool Equals(T Instance, T Other)
			{
				return Math::Equals(Instance.GetX(), Other.GetX()) && Math::Equals(Instance.GetY(), Other.GetY()) && Math::Equals(Instance.GetZ(), Other.GetZ()) && Math::Equals(Instance.GetW(), Other.GetW());
			}

			template<typename T>
			typename T::Type Min(T Instance)
			{
				return Math::Min(Instance.GetX(), Math::Min(Instance.GetY(), Instance.GetZ()));
			}
			
			template<typename T>
			T Min(T Instance, T Other)
			{
				return T(Math::Min(Instance.GetX(), Other.GetX()), Math::Min(Instance.GetY(), Other.GetY()), Math::Min(Instance.GetZ(), Other.GetZ()), Math::Min(Instance.GetW(), Other.GetW()));
			}

			template<typename T>
			typename T::Type Max(T Instance)
			{
				return Math::Max(Instance.GetX(), Math::Max(Instance.GetY(), Instance.GetZ()));
			}

			template<typename T>
			T Max(T Instance, T Other)
			{
				return T(Math::Max(Instance.GetX(), Other.GetX()), Math::Max(Instance.GetY(), Other.GetY()), Math::Max(Instance.GetZ(), Other.GetZ()), Math::Max(Instance.GetW(), Other.GetW()));
			}

			template<typename T>
			T Clamp(T Instance, typename T::Type Min, typename T::Type Max)
			{
				return T(Math::Clamp(Instance.GetX(), Min, Max), Math::Clamp(Instance.GetY(), Min, Max), Math::Clamp(Instance.GetZ(), Min, Max), Math::Clamp(Instance.GetW(), Min, Max));
			}

			template<typename T>
			T Lerp(T A, T B, float Weight)
			{
				return VectorUtility::Lerp(A.ToVector4f(), B.ToVector4f(), Weight);
			}

			template<typename T>
			T Add(T Instance, typename T::Type Other)
			{
				return T(Instance.GetX() + Other, Instance.GetY() + Other, Instance.GetZ() + Other, Instance.GetW() + Other);
			}

			template<typename T>
			T Add(T Instance, T Other)
			{
				return T(Instance.GetX() + Other.GetX(), Instance.GetY() + Other.GetY(), Instance.GetZ() + Other.GetZ(), Instance.GetW() + Other.GetW());
			}

			template<typename T>
			T Subtract(T Instance, typename T::Type Other)
			{
				return T(Instance.GetX() - Other, Instance.GetY() - Other, Instance.GetZ() - Other, Instance.GetW() - Other);
			}

			template<typename T>
			T Subtract(T Instance, T Other)
			{
				return T(Instance.GetX() - Other.GetX(), Instance.GetY() - Other.GetY(), Instance.GetZ() - Other.GetZ(), Instance.GetW() - Other.GetW());
			}

			template<typename T>
			T Multiply(T Instance, typename T::Type Other)
			{
				return T(Instance.GetX() * Other, Instance.GetY() * Other, Instance.GetZ() * Other, Instance.GetW() * Other);
			}

			template<typename T>
			T Multiply(T Instance, T Other)
			{
				return T(Instance.GetX() * Other.GetX(), Instance.GetY() * Other.GetY(), Instance.GetZ() * Other.GetZ(), Instance.GetW() * Other.GetW());
			}

			template<typename T>
			T Divide(T Instance, typename T::Type Other)
			{
				return T(Instance.GetX() / Other, Instance.GetY() / Other, Instance.GetZ() / Other, Instance.GetW() / Other);
			}

			template<typename T>
			T Divide(T Instance, T Other)
			{
				return T(Instance.GetX() / Other.GetX(), Instance.GetY() / Other.GetY(), Instance.GetZ() / Other.GetZ(), Instance.GetW() / Other.GetW());
			}
		}
	}

	namespace Hashing
	{
		template<typename H>
		class HashProcess<Color, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Color& Data)
			{
				HashProcess<Color::Type, H>::Accumulate(State, Data.r);
				HashProcess<Color::Type, H>::Accumulate(State, Data.g);
				HashProcess<Color::Type, H>::Accumulate(State, Data.b);
				HashProcess<Color::Type, H>::Accumulate(State, Data.a);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Color& Data)
			{
				HashProcess<Color, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};
	}

	template<>
	struct StringConverter<Color>
	{
		static StringView GetFormat(bool Pretty)
		{
			return Pretty ? "(%.2f, %.2f, %.2f, %.2f)" : "(%f, %f, %f, %f)";
		}

		static void ToString(const Color& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Color>(Format, true), Data.r, Data.g, Data.b, Data.a);
		}

		static void FromString(StringView Data, Color& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Color>(Format, false), &Result.r, &Result.g, &Result.b, &Result.a);
		}
	};

	template<>
	struct RBSConverter<Color>
	{
		static Color Decode(RBS& Rbs)
		{
			Color Result;
			Result.r = Rbs.ReadObject<Color::Type>();
			Result.g = Rbs.ReadObject<Color::Type>();
			Result.b = Rbs.ReadObject<Color::Type>();
			Result.a = Rbs.ReadObject<Color::Type>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Color& Object)
		{
			Rbs.WriteObject(Object.r);
			Rbs.WriteObject(Object.g);
			Rbs.WriteObject(Object.b);
			Rbs.WriteObject(Object.a);
		}
	};
}

namespace YAML
{
	template<>
	struct convert<NxFr::Color>
	{
		static Node encode(const NxFr::Color& rhs)
		{
			Node node;
			node.SetStyle(YAML::EmitterStyle::Flow);

			node.push_back(rhs.r);
			node.push_back(rhs.g);
			node.push_back(rhs.b);
			node.push_back(rhs.a);

			return node;
		}

		static bool decode(const Node& node, NxFr::Color& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.r = node[0].as<NxFr::Color::Type>();
			rhs.g = node[1].as<NxFr::Color::Type>();
			rhs.b = node[2].as<NxFr::Color::Type>();
			rhs.a = node[3].as<NxFr::Color::Type>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Color& rhs)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;

		out << rhs.r;
		out << rhs.g;
		out << rhs.b;
		out << rhs.a;

		out << YAML::EndSeq;

		return out;
	}
}
