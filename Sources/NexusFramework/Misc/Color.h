#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Decimal.h"
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
		struct NEXUS_FRAMEWORK_API sRGB
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			sRGB();
			sRGB(Type V);
			sRGB(Type R, Type G, Type B);
			sRGB(Type R, Type G, Type B, Type A);
			sRGB(Vector3f V);
			sRGB(Vector4f V);
			sRGB(Linear Other);
			sRGB(Bits Other);
			sRGB(Hsv Other);

			operator Vector3f() const;
			operator Vector4f() const;

			Vector3f ToVector3f() const;
			Vector4f ToVector4f() const;
			float ToGrayscale() const;
			Linear ToLinear() const;
			Bits ToBits() const;
			Hsv ToHsv() const;

			bool operator==(sRGB Other) const;
			bool operator!=(sRGB Other) const;
			Type& operator[](uint64 Index);
			const Type& operator[](uint64 Index) const;

			sRGB& operator+=(Type Other);
			sRGB& operator-=(Type Other);
			sRGB& operator*=(Type Other);
			sRGB& operator/=(Type Other);
			sRGB& operator+=(sRGB Other);
			sRGB& operator-=(sRGB Other);
			sRGB& operator*=(sRGB Other);
			sRGB& operator/=(sRGB Other);
			
			Type Min() const;
			Type Max() const;
			sRGB Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;
			
			Type& GetX();
			Type& GetY();
			Type& GetZ();
			Type& GetW();
			const Type& GetX() const;
			const Type& GetY() const;
			const Type& GetZ() const;
			const Type& GetW() const;
			void SetX(Type X);
			void SetY(Type Y);
			void SetZ(Type Z);
			void SetW(Type W);

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

		struct NEXUS_FRAMEWORK_API Linear
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			Linear();
			Linear(Type V);
			Linear(Type R, Type G, Type B);
			Linear(Type R, Type G, Type B, Type A);
			Linear(Vector3f V);
			Linear(Vector4f V);
			Linear(sRGB Other);
			Linear(Bits Other);
			Linear(Hsv Other);

			operator Vector3f() const;
			operator Vector4f() const;

			Vector3f ToVector3f() const;
			Vector4f ToVector4f() const;
			float ToGrayscale() const;
			sRGB TosRGB() const;
			Bits ToBits() const;
			Hsv ToHsv() const;

			bool operator==(Linear Other) const;
			bool operator!=(Linear Other) const;
			Type& operator[](uint64 Index);
			const Type& operator[](uint64 Index) const;

			Linear& operator+=(Type Other);
			Linear& operator-=(Type Other);
			Linear& operator*=(Type Other);
			Linear& operator/=(Type Other);
			Linear& operator+=(Linear Other);
			Linear& operator-=(Linear Other);
			Linear& operator*=(Linear Other);
			Linear& operator/=(Linear Other);

			Type Min() const;
			Type Max() const;
			Linear Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			Type& GetX();
			Type& GetY();
			Type& GetZ();
			Type& GetW();
			const Type& GetX() const;
			const Type& GetY() const;
			const Type& GetZ() const;
			const Type& GetW() const;
			void SetX(Type X);
			void SetY(Type Y);
			void SetZ(Type Z);
			void SetW(Type W);

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

		struct NEXUS_FRAMEWORK_API Bits
		{
			using Type = uint8;

			static inline Type BoundLow = 0;
			static inline Type BoundHigh = 255;

			Bits();
			Bits(Type V);
			Bits(Type R, Type G, Type B);
			Bits(Type R, Type G, Type B, Type A);
			Bits(Vector3f V);
			Bits(Vector4f V);
			Bits(sRGB Other);
			Bits(Linear Other);
			Bits(Hsv Other);

			operator Vector3f() const;
			operator Vector4f() const;

			Vector3f ToVector3f() const;
			Vector4f ToVector4f() const;
			float ToGrayscale() const;
			sRGB TosRGB() const;
			Linear ToLinear() const;
			Hsv ToHsv() const;

			bool operator==(Bits Other) const;
			bool operator!=(Bits Other) const;
			Type& operator[](uint64 Index);
			const Type& operator[](uint64 Index) const;

			Bits& operator+=(Type Other);
			Bits& operator-=(Type Other);
			Bits& operator*=(Type Other);
			Bits& operator/=(Type Other);
			Bits& operator+=(Bits Other);
			Bits& operator-=(Bits Other);
			Bits& operator*=(Bits Other);
			Bits& operator/=(Bits Other);

			Type Min() const;
			Type Max() const;
			Bits Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			Type& GetX();
			Type& GetY();
			Type& GetZ();
			Type& GetW();
			const Type& GetX() const;
			const Type& GetY() const;
			const Type& GetZ() const;
			const Type& GetW() const;
			void SetX(Type X);
			void SetY(Type Y);
			void SetZ(Type Z);
			void SetW(Type W);

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

		struct NEXUS_FRAMEWORK_API Hsv
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			Hsv();
			Hsv(Type V);
			Hsv(Type H, Type S, Type V);
			Hsv(Type H, Type S, Type V, Type A);
			Hsv(Vector3f V);
			Hsv(Vector4f V);
			Hsv(sRGB Other);
			Hsv(Linear Other);
			Hsv(Bits Other);

			operator Vector3f() const;
			operator Vector4f() const;

			Vector3f ToVector3f() const;
			Vector4f ToVector4f() const;
			float ToGrayscale() const;
			sRGB TosRGB() const;
			Linear ToLinear() const;
			Bits ToBits() const;

			bool operator==(Hsv Other) const;
			bool operator!=(Hsv Other) const;
			Type& operator[](uint64 Index);
			const Type& operator[](uint64 Index) const;

			Hsv& operator+=(Type Other);
			Hsv& operator-=(Type Other);
			Hsv& operator*=(Type Other);
			Hsv& operator/=(Type Other);
			Hsv& operator+=(Hsv Other);
			Hsv& operator-=(Hsv Other);
			Hsv& operator*=(Hsv Other);
			Hsv& operator/=(Hsv Other);

			Type Min() const;
			Type Max() const;
			Hsv Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			Type& GetX();
			Type& GetY();
			Type& GetZ();
			Type& GetW();
			const Type& GetX() const;
			const Type& GetY() const;
			const Type& GetZ() const;
			const Type& GetW() const;
			void SetX(Type X);
			void SetY(Type Y);
			void SetZ(Type Z);
			void SetW(Type W);

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
		class Hasher<Color, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Color& Data)
			{
				State.Accumulate(Data.r);
				State.Accumulate(Data.g);
				State.Accumulate(Data.b);
				State.Accumulate(Data.a);
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
