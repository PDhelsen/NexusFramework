#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Numbers/Decimal.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"

#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Math/Rotation.h"
#include "NexusFramework/Math/Matrix.h"

namespace NxFr
{
#pragma region Declaration

	struct Ray;
	struct Plane;
	struct Triangle;
	struct Rectangle;
	struct Circle;
	struct Box;
	struct Cuboid;
	struct Sphere;

	namespace ShapeUtility
	{
		bool Contains(const Rectangle& Shape, const Rectangle& Other);
		bool Contains(const Circle& Shape, const Circle& Other);
		bool Contains(const Box& Shape, const Box& Other);
		bool Contains(const Cuboid& Shape, const Cuboid& Other);
		bool Contains(const Sphere& Shape, const Sphere& Other);
	}

#pragma endregion

#pragma region Ray

	struct Ray
	{
	public:
		Ray() : Origin(), Direction(Vector<3, float>::One) { }
		Ray(Vector<3, float> Direction) : Origin(Vector<3, float>::Zero), Direction(Direction) {}
		Ray(Vector<3, float> Origin, Vector<3, float> Direction) : Origin(Origin), Direction(Direction) {}

		bool operator==(const Ray& Other) const { return Origin == Other.Origin && Direction == Other.Direction; }
		bool operator!=(const Ray& Other) const { return !(*this == Other); }

		Ray& Flip() { Direction = -Direction; return *this; }
		Ray& Translate(Vector<3, float> Offset) { Origin += Offset; return *this; }
		Ray& Rotate(Quaternion Rotation) { Direction = Rotation * Direction; return *this; }

		String ToString() const { return StringUtility::Format("Origin: %s - Direction: %s", Origin.ToString().C(), Direction.ToString().C()); }

	public:
		Vector<3, float> Origin;
		Vector<3, float> Direction;
	};

#pragma endregion

#pragma region Plane

	struct Plane
	{
	public:
		Plane() : Normal(Vector<3, float>::Up), Distance(0.0f) {}
		Plane(float Distance) : Normal(Vector<3, float>::Up), Distance(Distance) {}
		Plane(Vector<3, float> Normal) : Normal(Normal), Distance(0.0f) {}
		Plane(Vector<3, float> Normal, float Distance) : Normal(Normal), Distance(Distance) {}

		bool operator==(const Plane& Other) const { return Normal == Other.Normal && Math::Equals(Distance, Other.Distance); }
		bool operator!=(const Plane& Other) const { return !(*this == Other); }

		Plane& Flip() { Normal = -Normal; return *this; }
		Plane& Translate(float  Delta) { Distance += Delta; return *this; }
		Plane& Rotate(Quaternion Rotation) { Normal = Rotation * Normal; return *this; }

		String ToString() const { return StringUtility::Format("Normal: %s - Distance: %f", Normal.ToString().C(), Distance); }

	public:
		Vector<3, float> Normal;
		float Distance;
	};

#pragma endregion

#pragma region Triangle

	struct Triangle
	{
	public:
		Triangle() : A(0), B(0), C(0) {}
		Triangle(Vector<3, float> A, Vector<3, float> B, Vector<3, float> C) : A(A), B(B), C(C) {}

		bool operator==(const Triangle& Other) const { return A == Other.A && B == Other.B && C == Other.C; }
		bool operator!=(const Triangle& Other) const { return !(*this == Other); }

		Triangle& Translate(Vector<3, float>  Delta) { A += Delta; B += Delta; C += Delta; return *this; }
		Triangle& Rotate(Quaternion Rotation) { A = Rotation * A; B = Rotation * B; C = Rotation * C; return *this; }
		Triangle& Scale(Vector<3, float>  Factor) { auto M = Matrix4x4f::Origin(Matrix4x4f::Scale(Factor), Center()); A = M.MultiplyPosition(A); B = M.MultiplyPosition(B); C = M.MultiplyPosition(C); return *this; }

		Vector<3, float> Center() const { return (A + B + C) / 3.0f; }
		Vector<3, float> Edge(uint8 Index) const { return Index == 0 ? B - A : Index == 1 ? C - B : A - C; }
		Vector<3, float> Normal() const { return VectorUtility::Normalize(VectorUtility::Cross((C - A).Normalized(), (B - A).Normalized())); }

		String ToString() const { return StringUtility::Format("A: %s - B: s - C: %s", A.ToString().C(), B.ToString().C(), C.ToString().C()); }

	public:
		Vector<3, float> A;
		Vector<3, float> B;
		Vector<3, float> C;
	};

#pragma endregion

#pragma region Rectangle

	struct Rectangle
	{
	public:
		Rectangle() : Center(Vector<2, float>::Zero), Extents(Vector<2, float>::One) { }
		Rectangle(Vector<2, float> Extents) : Center(Vector<2, float>::Zero), Extents(Extents) {}
		Rectangle(Vector<2, float> Center, Vector<2, float> Extents) : Center(Center), Extents(Extents) {}
		Rectangle(float X, float Y, float Width, float Height) : Center(X, Y), Extents(Width * 0.5f, Height * 0.5f) {}

		bool operator==(const Rectangle& Other) const { return Center == Other.Center && Extents == Other.Extents; }
		bool operator!=(const Rectangle& Other) const { return !(*this == Other); }

		Rectangle& Translate(Vector<2, float> Offset) { Center += Offset; return *this; }
		Rectangle& Scale(Vector<2, float> Factor) { Extents *= Factor; return *this; }
		Rectangle& Grow(Vector<2, float> Factor) { Extents += Factor; return *this; }
		Rectangle& Encapsulate(const Rectangle& Other)
		{
			if (ShapeUtility::Contains(*this, Other))
			{
				return *this;
			}
			if (ShapeUtility::Contains(Other, *this))
			{
				*this = Other;
				return *this;
			}

			Vector<2, float> Min = VectorUtility::Min(GetBottomLeft(), Other.GetBottomLeft());
			Vector<2, float> Max = VectorUtility::Max(GetTopRight(), Other.GetTopRight());
			SetCorner(Min, Max);

			return *this;
		}

		Vector<2, float> GetCorner(uint8 Index) const { return Index == 0 ? GetBottomLeft() : Index == 1 ? GetBottomRight() : Index == 2 ? GetTopRight() : GetTopLeft(); }
		void SetCorner(Vector<2, float> Min, Vector<2, float> Max) { Center = (Min + Max) * 0.5f; Extents = (Max - Min) * 0.5f; }
		Vector<2, float> GetBottomLeft() const { return Center + Vector<2, float>(-Extents.x, -Extents.y); }
		void SetBottomLeft(Vector<2, float> Position) { Vector<2, float> Opposite = GetTopRight(); SetCorner(Position, Opposite); }
		Vector<2, float> GetBottomRight() const { return Center + Vector<2, float>(Extents.x, -Extents.y); }
		void SetBottomRight(Vector<2, float> Position) { Vector<2, float> Opposite = GetTopLeft(); SetCorner(Vector<2, float>(Opposite.x, Position.y), Vector<2, float>(Position.x, Opposite.y)); }
		Vector<2, float> GetTopRight() const { return Center + Vector<2, float>(Extents.x, Extents.y);}
		void SetTopRight(Vector<2, float> Position) { Vector<2, float> Opposite = GetBottomLeft(); SetCorner(Opposite, Position); }
		Vector<2, float> GetTopLeft() const { return Center + Vector<2, float>(-Extents.x, Extents.y); }
		void SetTopLeft(Vector<2, float> Position) { Vector<2, float> Opposite = GetBottomRight(); SetCorner(Vector<2, float>(Position.x, Opposite.y), Vector<2, float>(Opposite.x, Position.y)); }
		Vector<2, float> GetSize() const { return 2.0f * Extents; }
		void GetSize(Vector<2, float> Size) { Extents = Size * 0.5f; }
		float GetWidth() const { return 2.0f * Extents.x; }
		void SetWidth(float Width) { Extents.x = Width * 0.5f; }
		float GetHeight() const { return 2.0f * Extents.y; }
		void SetHeight(float Height) { Extents.y = Height * 0.5f; }

		String ToString() const { return StringUtility::Format("Center: %s - Extents: %s", Center.ToString().C(), Extents.ToString().C()); }

	public:
		Vector<2, float> Center;
		Vector<2, float> Extents;
	};

#pragma endregion

#pragma region Circle

	struct Circle
	{
	public:
		Circle() : Center(Vector<2, float>::Zero), Radius(1.0f) { }
		Circle(float Radius) : Center(Vector<2, float>::Zero), Radius(Radius) {}
		Circle(Vector<2, float> Center) : Center(Center), Radius(1.0f) {}
		Circle(Vector<2, float> Center, float Radius) : Center(Center), Radius(Radius) {}

		bool operator==(const Circle& Other) const { return Center == Other.Center && Radius == Other.Radius; }
		bool operator!=(const Circle& Other) const { return !(*this == Other); }

		Circle& Translate(Vector<2, float> Offset) { Center += Offset; return *this; }
		Circle& Scale(float Factor) { Radius *= Factor; return *this; }
		Circle& Grow(float Factor) { Radius += Factor; return *this; }
		Circle& Encapsulate(const Circle& Other)
		{
			if (ShapeUtility::Contains(*this, Other))
			{
				return *this;
			}
			if (ShapeUtility::Contains(Other, *this))
			{
				*this = Other;
				return *this;
			}

			float Distance = VectorUtility::Distance(Center, Other.Center);
			float Size = (Distance + Radius + Other.Radius) * 0.5f;
			Vector<2, float> Direction = (Other.Center - Center) / Distance;
			Center = Center + Direction * (Size - Radius);
			Radius = Size;

			return *this;
		}

		float GetDiameter() const { return 2.0f * Radius; }
		void SetDiameter(float Diameter) { Radius = Diameter * 0.5f; }

		String ToString() const { return StringUtility::Format("Center: %s - Radius: %f", Center.ToString().C(), Radius); }

	public:
		Vector<2, float> Center;
		float Radius;
	};

#pragma endregion

#pragma region Box & Cuboid

	struct Box
	{
	public:
		Box() : Center(Vector<3, float>::Zero), Extents(Vector<3, float>::One) { }
		Box(Vector<3, float> Extents) : Center(Vector<3, float>::Zero), Extents(Extents) {}
		Box(Vector<3, float> Center, Vector<3, float> Extents) : Center(Center), Extents(Extents) {}

		bool operator==(const Box& Other) const { return Center == Other.Center && Extents == Other.Extents; }
		bool operator!=(const Box& Other) const { return !(*this == Other); }

		Box& Translate(Vector<3, float> Offset) { Center += Offset; return *this; }
		Box& Scale(Vector<3, float> Factor) { Extents *= Factor; return *this; }
		Box& Grow(Vector<3, float> Factor) { Extents += Factor; return *this; }
		Box& Encapsulate(const Box& Other)
		{
			if (ShapeUtility::Contains(*this, Other))
			{
				return *this;
			}
			if (ShapeUtility::Contains(Other, *this))
			{
				*this = Other;
				return *this;
			}

			Vector<3, float> Min = VectorUtility::Min(GetMin(), Other.GetMin());
			Vector<3, float> Max = VectorUtility::Max(GetMax(), Other.GetMax());
			SetMinMax(Min, Max);
			return *this;
		}

		Vector<3, float> GetMin() const { return Center - Extents; }
		void SetMin(Vector<3, float> Min) { SetMinMax(Min, GetMax()); }
		Vector<3, float> GetMax() const { return Center + Extents; }
		void SetMax(Vector<3, float> Max) { SetMinMax(GetMin(), Max); }
		void SetMinMax(Vector<3, float> Min, Vector<3, float> Max) { Center = (Min + Max) * 0.5f; Extents = (Max - Min) * 0.5f; }
		Vector<3, float> GetSize() const { return Extents * 2.0f; }
		void SetSize(Vector<3, float> Size) { Extents = Size * 0.5f; }

		String ToString() const { return StringUtility::Format("Center: %s - Extents: %s", Center.ToString().C(), Extents.ToString().C()); }

	public:
		Vector<3, float> Center;
		Vector<3, float> Extents;
	};

	struct Cuboid
	{
	public:
		Cuboid() : Center(Vector<3, float>::Zero), Orientation(Vector<3, float>::Zero), Extents(Vector<3, float>::One) { }
		Cuboid(Vector<3, float> Extents) : Center(Vector<3, float>::Zero), Orientation(Quaternion::Identity), Extents(Extents) {}
		Cuboid(Vector<3, float> Center, Vector<3, float> Extents) : Center(Center), Orientation(Quaternion::Identity), Extents(Extents) {}
		Cuboid(Vector<3, float> Center, Quaternion Orientation, Vector<3, float> Extents) : Center(Center), Orientation(Orientation), Extents(Extents) {}

		bool operator==(const Cuboid& Other) const { return Center == Other.Center && Orientation == Other.Orientation && Extents == Other.Extents; }
		bool operator!=(const Cuboid& Other) const { return !(*this == Other); }

		Cuboid& Translate(Vector<3, float> Offset) { Center += Offset; return *this;}
		Cuboid& Rotate(Quaternion Rotation) { Orientation = Rotation * (Quaternion)Orientation; return *this; }
		Cuboid& Scale(Vector<3, float> Factor) { Extents *= Factor; return *this; }
		Cuboid& Grow(Vector<3, float> Factor) { Extents += Factor; return *this; }
		Cuboid& Encapsulate(const Cuboid& Other)
		{
			if (ShapeUtility::Contains(*this, Other))
			{
				return *this;
			}
			if (ShapeUtility::Contains(Other, *this))
			{
				*this = Other;
				return *this;
			}

			Quaternion WorldToLocal = Orientation.Inverse();
			Vector<3, float> Min = Vector<3, float>(Decimal::MaxF);
			Vector<3, float> Max = Vector<3, float>(-Decimal::MaxF);

			for (int8 Z = -1; Z <= 1; Z += 2)
				for (int8 Y = -1; Y <= 1; Y += 2)
					for (int8 X = -1; X <= 1; X += 2)
					{
						Vector<3, float> Corner = Vector<3, float>(X * Extents.x, Y * Extents.y, Z * Extents.z);
						Vector<3, float> World = Center + Orientation * Corner;
						Vector<3, float> Local = WorldToLocal * (World - Center);
						Min = VectorUtility::Min(Min, Local);
						Max = VectorUtility::Max(Max, Local);

						Corner = Vector<3, float>(X * Other.Extents.x, Y * Other.Extents.y, Z * Other.Extents.z);
						World = Other.Center + Other.Orientation * Corner;
						Local = WorldToLocal * (World - Center);
						Min = VectorUtility::Min(Min, Local);
						Max = VectorUtility::Max(Max, Local);
					}

			Center = Center + Orientation * ((Min + Max) * 0.5f);
			Extents = (Max - Min) * 0.5f;
			return *this;
		}

		void GetMinMax(Vector<3, float>& Min, Vector<3, float>& Max) const
		{
			Min = Vector<3, float>(Decimal::MaxF);
			Max = Vector<3, float>(-Decimal::MaxF);

			for (int8 Z = -1; Z <= 1; Z += 2)
				for (int8 Y = -1; Y <= 1; Y += 2)
					for (int8 X = -1; X <= 1; X += 2)
					{
						Vector<3, float> Position = Vector<3, float>(X * Extents.x, Y * Extents.y, Z * Extents.z);
						Position = Center + Orientation * Position;
						Min = VectorUtility::Min(Min, Position);
						Max = VectorUtility::Max(Max, Position);
					}
		}

		Vector<3, float> GetMin() const { Vector<3, float> Min, Max; GetMinMax(Min, Max); return Min; }
		Vector<3, float> GetMax() const { Vector<3, float> Min, Max; GetMinMax(Min, Max); return Max; }
		Vector<3, float> GetNegativeCorner() const { return Center - GetRotatedPosition(Extents); }
		Vector<3, float> GetPositiveCorner() const { return Center + GetRotatedPosition(Extents); }
		Vector<3, float> GetSize() const { return Extents * 2.0f; }
		void SetSize(Vector<3, float> Size) { Extents = Size * 0.5f; }

		Vector<3, float> GetRotatedPosition(Vector<3, float> Position) const { return Position.x * (Orientation * Vector<3, float>::Right) + Position.y * (Orientation * Vector<3, float>::Up) + Position.z * (Orientation * Vector<3, float>::Forward); }

		String ToString() const { return StringUtility::Format("Center: %s - Orientation: %s - Extents: %s", Center.ToString().C(), Orientation.ToString().C(), Extents.ToString().C()); }

	public:
		Vector<3, float> Center;
		Quaternion Orientation;
		Vector<3, float> Extents;
	};

#pragma endregion

#pragma region Sphere

	struct Sphere
	{
	public:
		Sphere() : Center(Vector<3, float>::Zero), Radius(1.0f) { }
		Sphere(float Radius) : Center(Vector<3, float>::Zero), Radius(Radius) {}
		Sphere(Vector<3, float> Center) : Center(Center), Radius(1.0f) {}
		Sphere(Vector<3, float> Center, float Radius) : Center(Center), Radius(Radius) {}

		bool operator==(const Sphere& Other) const { return Center == Other.Center && Radius == Other.Radius; }
		bool operator!=(const Sphere& Other) const { return !(*this == Other); }

		Sphere& Translate(Vector<3, float> Offset) { Center += Offset; return *this; }
		Sphere& Scale(float Factor) { Radius *= Factor; return *this;}
		Sphere& Grow(float Factor) { Radius += Factor; return *this; }
		Sphere& Encapsulate(const Sphere& Other)
		{
			if (ShapeUtility::Contains(*this, Other))
			{
				return *this;
			}
			if (ShapeUtility::Contains(Other, *this))
			{
				*this = Other;
				return *this;
			}

			float Distance = VectorUtility::Distance(Center, Other.Center);
			float Size = (Distance + Radius + Other.Radius) * 0.5f;
			Vector<3, float> Direction = (Other.Center - Center) / Distance;

			Center = Center + Direction * (Size - Radius);
			Radius = Size;
			return *this;
		}

		float GetDiameter() const { return 2.0f * Radius; }
		void SetDiameter(float Diameter) { Radius = Diameter * 0.5f; }

		String ToString() const { return StringUtility::Format("Center: %s - Radius: %f", Center.ToString().C(), Radius); }

	public:
		Vector<3, float> Center;
		float Radius;
	};

#pragma endregion

#pragma region ShapeUtility

	namespace ShapeUtility
	{
		inline Circle ConvertRectangleToCircle(const Rectangle& Shape)
		{
			return Circle(Shape.Center, VectorUtility::Magnitude(Shape.Extents));
		}

		inline Rectangle ConvertCircleToRectangle(const Circle& Shape)
		{
			return Rectangle(Shape.Center, Vector<2, float>::One * Shape.Radius);
		}

		inline Sphere ConvertBoxToSphere(const Box& Shape)
		{
			return Sphere(Shape.Center, VectorUtility::Magnitude(Shape.Extents));
		}

		inline Sphere ConvertCuboidToSphere(const Cuboid& Shape)
		{
			return Sphere(Shape.Center, VectorUtility::Magnitude(Shape.Orientation * Shape.Extents));
		}

		inline Box ConvertSphereToBox(const Sphere & Shape)
		{
			return Box(Shape.Center, Vector<3, float>::One * Shape.Radius);
		}

		inline Cuboid ConvertSphereToCuboid(const Sphere& Shape)
		{
			return Cuboid(Shape.Center, Vector<3, float>::One * Shape.Radius);
		}

		inline Cuboid ConvertBoxToCuboid(const Box& Shape)
		{
			return Cuboid(Shape.Center, Shape.Extents);
		}

		inline Box ConvertCuboidToBox(const Cuboid& Shape)
		{
			Vector<3, float> Min, Max;
			Shape.GetMinMax(Min, Max);

			Box Result;
			Result.SetMinMax(Min, Max);
			return Result;
		}

		inline Vector<3, float> Position(const Ray& Shape, float Distance)
		{
			return Shape.Origin + Shape.Direction * Distance;
		}

		inline Vector<3, float> Position(const Plane& Shape, Vector<2, float> Uv)
		{
			Vector<3, float> Position = -Shape.Distance * Shape.Normal;

			Vector<3, float> Axis = Math::Abs(Shape.Normal.z) < 0.99f ? Vector<3, float>::Forward : Vector<3, float>::Up;
			Vector<3, float> Tangent1 = VectorUtility::Normalize(VectorUtility::Cross(Axis, Shape.Normal));
			Vector<3, float> Tangent2 = VectorUtility::Normalize(VectorUtility::Cross(Shape.Normal, Tangent1));

			Uv -= 0.5f;
			return Position + Uv.x * Tangent1 + Uv.y * Tangent2;
		}

		inline Vector<3, float> Position(const Triangle& Shape, Vector<3, float> Barycentric)
		{
			return Shape.A * Barycentric.x + Shape.B * Barycentric.y + Shape.C * Barycentric.z;
		}

		inline Vector<3, float> Position(const Triangle& Shape, Vector<2, float> Barycentric)
		{
			return Position(Shape, Vector<3, float>(Barycentric.x, Barycentric.y, 1.0f - Barycentric.x - Barycentric.y));
		}

		inline Vector<2, float> Position(const Rectangle& Shape, Vector<2, float> Uv)
		{
			return Shape.Center + Shape.Extents * (Uv * 2.0f - 1.0f);
		}

		inline Vector<2, float> Position(const Circle& Shape, float Angle)
		{
			Angle *= 2.0f * Math::Pi;
			return Shape.Center + Shape.Radius * Math::Cos(Angle) * Vector<2, float>::Right + Shape.Radius * Math::Sin(Angle) * Vector<2, float>::Up;
		}

		inline Vector<3, float> Position(const Box& Shape, Vector<3, float> Uv)
		{
			return Shape.Center + Shape.Extents * (Uv * 2.0f - 1.0f);
		}

		inline Vector<3, float> Position(const Cuboid& Shape, Vector<3, float> Uv)
		{
			return Shape.Center + Shape.Orientation * (Shape.Extents * (Uv * 2.0f - 1.0f));
		}

		inline Vector<3, float> Position(const Sphere& Shape, Vector<2, float> Uv)
		{
			float Theta = Uv.x * 2.0f * Math::Pi;
			float Phi = Uv.y * Math::Pi;

			float X = Shape.Radius * Math::Sin(Phi) * Math::Cos(Theta);
			float Z = Shape.Radius * Math::Sin(Phi) * -Math::Sin(Theta);
			float Y = Shape.Radius * Math::Cos(Phi);

			return Shape.Center + Vector<3, float>(X, Y, Z);
		}

		inline Vector<2, float> Coordinates(const Rectangle& Shape, Vector<2, float> Position, bool Clamp = true)
		{
			Position -= Shape.GetBottomLeft();
			Position /= Shape.GetSize();
			if (Clamp)
			{
				Position = VectorUtility::Clamp(Position, Vector<2, float>::Zero, Vector<2, float>::One);
			}
			return Position;
		}

		inline float Coordinates(const Circle& Shape, Vector<2, float> Position)
		{
			Position -= Shape.Center;
			return Math::Atan(Position.y, Position.x) / (2.0f * Math::Pi);
		}

		inline Vector<3, float> Coordinates(const Box& Shape, Vector<3, float> Position, bool Clamp = true)
		{
			Position -= Shape.GetMin();
			Position /= Shape.GetSize();
			if (Clamp)
			{
				Position = VectorUtility::Clamp(Position, Vector<3, float>::Zero, Vector<3, float>::One);
			}
			return Position;
		}

		inline Vector<3, float> Coordinates(const Cuboid& Shape, Vector<3, float> Position, bool Clamp = true)
		{
			Position -= Shape.Center;
			Position = Shape.Orientation.Inverse() * Position;
			Position = (Position + Shape.Extents) / Shape.GetSize();
			if (Clamp)
			{
				Position = VectorUtility::Clamp(Position, Vector<3, float>::Zero, Vector<3, float>::One);
			}
			return Position;
		}

		inline Vector<2, float> Coordinates(const Sphere& Shape, Vector<3, float> Position)
		{
			Position -= Shape.Center;
			Position = Position.Normalized();

			float Theta = Math::Atan(Position.z, Position.x) / (2.0f * Math::Pi) + 0.5f;
			float Phi = Math::Acos(Position.y) / Math::Pi;

			return Vector<2, float>(Theta, Phi);
		}

		inline bool Side(const Ray& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Direction = Position - Shape.Origin;
			return VectorUtility::Dot(Shape.Direction, Direction) >= 0.0f;
		}

		inline bool Side(const Plane& Shape, Vector<3, float> Position)
		{
			return VectorUtility::Dot(Shape.Normal, Position) + Shape.Distance >= 0.0f;
		}

		inline bool Side(const Triangle& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Direction = Position - Shape.Center();
			return VectorUtility::Dot(Shape.Normal(), Direction) >= 0.0f;
		}

		inline bool Contains(const Ray& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Direction = Position - Shape.Origin;
			float Factor = VectorUtility::Dot(Shape.Direction, Direction);
			Vector<3, float> Projected = Shape.Origin + Shape.Direction * Factor;
			float Distance = VectorUtility::SqrMagnitude(Position - Projected);
			return Factor >= 0.0f && Math::Equals(Distance, 0.0f);
		}

		inline bool Contains(const Plane& Shape, Vector<3, float> Position)
		{
			return Math::Equals(VectorUtility::Dot(Shape.Normal, Position) + Shape.Distance, 0.0f);
		}

		inline bool Contains(const Triangle& Shape, Vector<3, float> Position)
		{
			Vector<3, float> V0 = Shape.B - Shape.A;
			Vector<3, float> V1 = Shape.C - Shape.A;
			Vector<3, float> V2 = Position - Shape.A;

			float Dot00 = VectorUtility::Dot(V0, V0);
			float Dot01 = VectorUtility::Dot(V0, V1);
			float Dot11 = VectorUtility::Dot(V1, V1);
			float Dot20 = VectorUtility::Dot(V2, V0);
			float Dot21 = VectorUtility::Dot(V2, V1);

			float Denominator = Dot00 * Dot11 - Dot01 * Dot01;
			if (Math::Equals(Denominator, 0.0f))
			{
				return false;
			}

			float Inv = 1.0f / Denominator;
			float U = (Dot11 * Dot20 - Dot01 * Dot21) * Inv;
			float V = (Dot00 * Dot21 - Dot01 * Dot20) * Inv;
			float W = 1.0f - U - V;

			return U >= 0.0f && V >= 0.0f && W >= 0.0f;
		}

		inline bool Contains(const Rectangle& Shape, Vector<2, float> Position)
		{
			Vector<2, float> Min = Shape.GetBottomLeft();
			Vector<2, float> Max = Shape.GetTopRight();
			return Position.x >= Min.x && Position.x <= Max.x && Position.y >= Min.y && Position.y <= Max.y;
		}

		inline bool Contains(const Rectangle& Shape, const Rectangle& Other)
		{
			Vector<2, float> ShapeMin = Shape.GetBottomLeft();
			Vector<2, float> ShapeMax = Shape.GetTopRight();
			Vector<2, float> OtherMin = Other.GetBottomLeft();
			Vector<2, float> OtherMax = Other.GetTopRight();
			return OtherMin.x >= ShapeMin.x && OtherMax.x <= ShapeMax.x && OtherMin.y >= ShapeMin.y && OtherMax.y <= ShapeMax.y;
		}

		inline bool Contains(const Circle& Shape, Vector<2, float> Position)
		{
			return VectorUtility::SqrMagnitude(Position - Shape.Center) <= Math::Square(Shape.Radius);
		}

		inline bool Contains(const Circle& Shape, const Circle& Other)
		{
			float RadiusDelta = Shape.Radius - Other.Radius;
			Vector<2, float> Offset = Other.Center - Shape.Center;
			return RadiusDelta >= 0.0f && VectorUtility::SqrMagnitude(Offset) <= Math::Square(RadiusDelta);
		}

		inline bool Contains(const Box& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Min = Shape.GetMin();
			Vector<3, float> Max = Shape.GetMax();
			return Position.x >= Min.x && Position.x <= Max.x && Position.y >= Min.y && Position.y <= Max.y && Position.z >= Min.z && Position.z <= Max.z;
		}

		inline bool Contains(const Box& Shape, const Box& Other)
		{
			Vector<3, float> ShapeMin = Shape.GetMin();
			Vector<3, float> ShapeMax = Shape.GetMax();
			Vector<3, float> OtherMin = Other.GetMin();
			Vector<3, float> OtherMax = Other.GetMax();
			return OtherMin.x >= ShapeMin.x && OtherMax.x <= ShapeMax.x && OtherMin.y >= ShapeMin.y && OtherMax.y <= ShapeMax.y && OtherMin.z >= ShapeMin.z && OtherMax.z <= ShapeMax.z;
		}

		inline bool Contains(const Cuboid& Shape, Vector<3, float> Position)
		{
			Position -= Shape.Center;

			float X = VectorUtility::Dot(Position, Shape.Orientation * Vector<3, float>::Right);
			float Y = VectorUtility::Dot(Position, Shape.Orientation * Vector<3, float>::Up);
			float Z = VectorUtility::Dot(Position, Shape.Orientation * Vector<3, float>::Forward);

			return Math::Abs(X) <= Shape.Extents.x && Math::Abs(Y) <= Shape.Extents.y && Math::Abs(Z) <= Shape.Extents.z;
		}

		inline bool Contains(const Cuboid& Shape, const Cuboid& Other)
		{
			for (int8 Z = -1; Z <= 1; Z += 2)
				for (int8 Y = -1; Y <= 1; Y += 2)
					for (int8 X = -1; X <= 1; X += 2)
					{
						Vector<3, float> Position = Other.Center + Other.Orientation * (Other.Extents * Vector<3, float>(X, Y, Z));
						if (!Contains(Shape, Position))
						{
							return false;
						}
					}

			return true;
		}

		inline bool Contains(const Sphere& Shape, Vector<3, float> Position)
		{
			return VectorUtility::SqrMagnitude(Position - Shape.Center) <= Math::Square(Shape.Radius);
		}

		inline bool Contains(const Sphere& Shape, const Sphere& Other)
		{
			float RadiusDelta = Shape.Radius - Other.Radius;
			Vector<3, float> Offset = Other.Center - Shape.Center;
			return RadiusDelta >= 0.0f && VectorUtility::SqrMagnitude(Offset) <= Math::Square(RadiusDelta);
		}

		inline bool Overlap(const Rectangle& Shape, const Rectangle& Other)
		{
			Vector<2, float> ShapeMin = Shape.GetBottomLeft();
			Vector<2, float> ShapeMax = Shape.GetTopRight();
			Vector<2, float> OtherMin = Other.GetBottomLeft();
			Vector<2, float> OtherMax = Other.GetTopRight();
			return !(ShapeMax.x < OtherMin.x || ShapeMin.x > OtherMax.x || ShapeMax.y < OtherMin.y || ShapeMin.y > OtherMax.y);
		}

		inline bool Overlap(const Circle& Shape, const Circle& Other)
		{
			float RadiusSum = Shape.Radius + Other.Radius;
			Vector<2, float> Offset = Other.Center - Shape.Center;
			return VectorUtility::SqrMagnitude(Offset) <= Math::Square(RadiusSum);
		}

		inline bool Overlap(const Box& Shape, const Box& Other)
		{
			Vector<3, float> ShapeMin = Shape.GetMin();
			Vector<3, float> ShapeMax = Shape.GetMax();
			Vector<3, float> OtherMin = Other.GetMin();
			Vector<3, float> OtherMax = Other.GetMax();
			return !(ShapeMax.x < OtherMin.x || ShapeMin.x > OtherMax.x || ShapeMax.y < OtherMin.y || ShapeMin.y > OtherMax.y || ShapeMax.z < OtherMin.z || ShapeMin.z > OtherMax.z);
		}

		inline bool Overlap(const Sphere& Shape, const Sphere& Other)
		{
			float RadiusSum = Shape.Radius + Other.Radius;
			Vector<3, float> Offset = Other.Center - Shape.Center;
			return VectorUtility::SqrMagnitude(Offset) <= Math::Square(RadiusSum);
		}

		inline bool Intersect(const Plane& Shape, const Ray& Other, Vector<3, float>& Point)
		{
			float Denominator = VectorUtility::Dot(Shape.Normal, Other.Direction);
			if (Math::Equals(Denominator, 0.0f))
			{
				return false;
			}

			float Distance = -(VectorUtility::Dot(Shape.Normal, Other.Origin) + Shape.Distance) / Denominator;
			if (Distance < 0.0f)
			{
				return false;
			}

			Point = Position(Other, Distance);
			return true;
		}

		inline bool Intersect(const Plane& Shape, const Ray& Other)
		{
			Vector<3, float> Point;
			return Intersect(Shape, Other, Point);
		}

		inline bool Intersect(const Triangle& Shape, const Ray& Other, Vector<3, float>& Point)
		{
			Vector<3, float> Edge1 = Shape.B - Shape.A;
			Vector<3, float> Edge2 = Shape.C - Shape.A;
			Vector<3, float> Offset = VectorUtility::Cross(Other.Direction, Edge2);
			float Det = VectorUtility::Dot(Edge1, Offset);

			if (Math::Equals(Det, 0.0f))
			{
				return false;
			}

			float InvDet = 1.0f / Det;
			Vector<3, float> T = Other.Origin - Shape.A;
			float U = VectorUtility::Dot(T, Offset) * InvDet;
			if (U < 0.0f || U > 1.0f)
			{
				return false;
			}

			Vector<3, float> Q = VectorUtility::Cross(T, Edge1);
			float V = VectorUtility::Dot(Other.Direction, Q) * InvDet;
			if (V < 0.0f || U + V > 1.0f)
			{
				return false;
			}

			float Distance = VectorUtility::Dot(Edge2, Q) * InvDet;
			if (Distance < 0.0f)
			{
				return false;
			}

			Point = Other.Origin + Other.Direction * Distance;
			return true;
		}

		inline bool Intersect(const Triangle& Shape, const Ray& Other)
		{
			Vector<3, float> Point;
			return Intersect(Shape, Other, Point);
		}

		inline bool Intersect(const Triangle& Shape, const Plane& Other, Vector<3, float>& Entry, Vector<3, float>& Exit)
		{
			float DistA = VectorUtility::Dot(Other.Normal, Shape.A) + Other.Distance;
			float DistB = VectorUtility::Dot(Other.Normal, Shape.B) + Other.Distance;
			float DistC = VectorUtility::Dot(Other.Normal, Shape.C) + Other.Distance;

			bool SideA = DistA >= 0.0f;
			bool SideB = DistB >= 0.0f;
			bool SideC = DistC >= 0.0f;

			if ((SideA && SideB && SideC) || (!SideA && !SideB && !SideC))
			{
				return false;
			}

			Vector<3, float> Intersections[2];
			int Count = 0;

			if (SideA != SideB)
				Intersections[Count++] = Shape.A + (Shape.B - Shape.A) * (DistA / (DistA - DistB));
			if (SideB != SideC)
				Intersections[Count++] = Shape.B + (Shape.C - Shape.B) * (DistB / (DistB - DistC));
			if (Count < 2 && SideC != SideA)
				Intersections[Count++] = Shape.C + (Shape.A - Shape.C) * (DistC / (DistC - DistA));

			if (Count < 2)
				return false;

			Entry = Intersections[0];
			Exit = Intersections[1];
			return true;
		}

		inline bool Intersect(const Triangle& Shape, const Plane& Other)
		{
			Vector<3, float> Entry, Exit;
			return Intersect(Shape, Other, Entry, Exit);
		}

		inline bool Intersect(const Box& Shape, const Ray& Other, Vector<3, float>& Entry, Vector<3, float>& Exit)
		{
			Vector<3, float> Min = Shape.GetMin();
			Vector<3, float> Max = Shape.GetMax();

			float EntryDistance = -Decimal::MaxF;
			float ExitDistance = Decimal::MaxF;

			if (Math::Equals(Other.Direction.x, 0.0f) && (Other.Origin.x < Min.x || Other.Origin.x > Max.x))
			{
				return false;
			}
			float Rec = 1.0f / Other.Direction.x;
			EntryDistance = Math::Max(EntryDistance, ((Rec >= 0.0f ? Min.x : Max.x) - Other.Origin.x) * Rec);
			ExitDistance = Math::Min(ExitDistance, ((Rec >= 0.0f ? Max.x : Min.x) - Other.Origin.x) * Rec);
			if (EntryDistance > ExitDistance)
			{
				return false;
			}

			if (Math::Equals(Other.Direction.y, 0.0f) && (Other.Origin.y < Min.y || Other.Origin.y > Max.y))
			{
				return false;
			}
			Rec = 1.0f / Other.Direction.y;
			EntryDistance = Math::Max(EntryDistance, ((Rec >= 0.0f ? Min.y : Max.y) - Other.Origin.y) * Rec);
			ExitDistance = Math::Min(ExitDistance, ((Rec >= 0.0f ? Max.y : Min.y) - Other.Origin.y) * Rec);
			if (EntryDistance > ExitDistance)
			{
				return false;
			}

			if (Math::Equals(Other.Direction.z, 0.0f) && (Other.Origin.z < Min.z || Other.Origin.z > Max.z))
			{
				return false;
			}
			Rec = 1.0f / Other.Direction.z;
			EntryDistance = Math::Max(EntryDistance, ((Rec >= 0.0f ? Min.z : Max.z) - Other.Origin.z) * Rec);
			ExitDistance = Math::Min(ExitDistance, ((Rec >= 0.0f ? Max.z : Min.z) - Other.Origin.z) * Rec);
			if (EntryDistance > ExitDistance)
			{
				return false;
			}

			Entry = Position(Other, EntryDistance);
			Exit = Position(Other, ExitDistance);
			return ExitDistance >= 0.0f;
		}

		inline bool Intersect(const Box& Shape, const Ray& Other)
		{
			Vector<3, float> Entry, Exit;
			return Intersect(Shape, Other, Entry, Exit);
		}

		inline bool Intersect(const Box& Shape, const Plane& Other, Vector<3, float>& Entry, Vector<3, float>& Exit)
		{
			Vector<3, float> Min = Shape.GetMin();
			Vector<3, float> Max = Shape.GetMax();

			Vector<3, float> DiagonalMin, DiagonalMax;
			DiagonalMin.x = Other.Normal.x >= 0.0f ? Min.x : Max.x;
			DiagonalMax.x = Other.Normal.x >= 0.0f ? Max.x : Min.x;
			DiagonalMin.y = Other.Normal.y >= 0.0f ? Min.y : Max.y;
			DiagonalMax.y = Other.Normal.y >= 0.0f ? Max.y : Min.y;
			DiagonalMin.z = Other.Normal.z >= 0.0f ? Min.z : Max.z;
			DiagonalMax.z = Other.Normal.z >= 0.0f ? Max.z : Min.z;

			float DistanceMin = VectorUtility::Dot(Other.Normal, DiagonalMin) + Other.Distance;
			float DistanceMax = VectorUtility::Dot(Other.Normal, DiagonalMax) + Other.Distance;

			if (DistanceMin > 0.0f && DistanceMax > 0.0f)
			{
				return false;
			}
			if (DistanceMin < 0.0f && DistanceMax < 0.0f)
			{
				return false;
			}

			Vector<3, float> Center = (Min + Max) * 0.5f;
			float CenterDistance = VectorUtility::Dot(Other.Normal, Center) + Other.Distance;

			Vector<3, float> PlaneProjection = Center - Other.Normal * CenterDistance;
			Vector<3, float> HalfExtents = (Max - Min) * 0.5f;
			float Radius =
				HalfExtents.x * Math::Abs(Other.Normal.x) +
				HalfExtents.y * Math::Abs(Other.Normal.y) +
				HalfExtents.z * Math::Abs(Other.Normal.z);

			Entry = PlaneProjection - Other.Normal * Radius;
			Exit = PlaneProjection + Other.Normal * Radius;

			return true;
		}


		inline bool Intersect(const Box& Shape, const Plane& Other)
		{
			Vector<3, float> Entry, Exit;
			return Intersect(Shape, Other, Entry, Exit);
		}

		inline bool Intersect(const Cuboid& Shape, const Ray& Other, Vector<3, float>& Entry, Vector<3, float>& Exit)
		{
			Matrix<4, 4, float> Axis = Matrix<4, 4, float>::Rotate(Shape.Orientation);

			Vector<3, float> Offset = Shape.Center - Other.Origin;
			float EntryDistance = -Decimal::MaxF;
			float ExitDistance = Decimal::MaxF;

			for (int I = 0; I < 3; ++I)
			{
				float E = VectorUtility::Dot(Vector<3, float>(Axis.GetColumn(I)), Offset);
				float F = VectorUtility::Dot(Vector<3, float>(Axis.GetColumn(I)), Other.Direction);

				if (Math::Equals(F, 0.0f))
				{
					if (-E - Shape.Extents[I] > 0 || -E + Shape.Extents[I] < 0)
					{
						return false;
					}

					continue;
				}

				float EntryD = (E + Shape.Extents[I]) / F;
				float ExitD = (E - Shape.Extents[I]) / F;

				if (EntryD > ExitD)
				{
					float Temp = EntryD;
					EntryD = ExitD;
					ExitD = Temp;
				}

				EntryDistance = Math::Max(EntryDistance, EntryD);
				ExitDistance = Math::Min(ExitDistance, ExitD);

				if (EntryDistance > ExitDistance || ExitDistance < 0)
				{
					return false;
				}
			}

			Entry = Other.Origin + Other.Direction * EntryDistance;
			Exit = Other.Origin + Other.Direction * ExitDistance;

			return true;
		}

		inline bool Intersect(const Cuboid& Shape, const Ray& Other)
		{
			Vector<3, float> Entry, Exit;
			return Intersect(Shape, Other, Entry, Exit);
		}

		inline bool Intersect(const Cuboid& Shape, const Plane& Other, Vector<3, float>& Entry, Vector<3, float>& Exit)
		{
			Matrix<4, 4, float> Axis = Matrix<4, 4, float>::Rotate(Shape.Orientation);

			float Radius =
				Shape.Extents.x * Math::Abs(VectorUtility::Dot(Other.Normal, Vector<3, float>(Axis.GetColumn(0)))) +
				Shape.Extents.y * Math::Abs(VectorUtility::Dot(Other.Normal, Vector<3, float>(Axis.GetColumn(1)))) +
				Shape.Extents.z * Math::Abs(VectorUtility::Dot(Other.Normal, Vector<3, float>(Axis.GetColumn(2))));

			float CenterDistance = VectorUtility::Dot(Other.Normal, Shape.Center) + Other.Distance;
			if (Math::Abs(CenterDistance) > Radius)
			{
				return false;
			}

			Vector<3, float> PlaneProjection = Shape.Center - Other.Normal * CenterDistance;

			Entry = PlaneProjection - Other.Normal * Radius;
			Exit = PlaneProjection + Other.Normal * Radius;

			return true;
		}

		inline bool Intersect(const Cuboid& Shape, const Plane& Other)
		{
			Vector<3, float> Entry, Exit;
			return Intersect(Shape, Other, Entry, Exit);
		}

		inline bool Intersect(const Sphere& Shape, const Ray& Other, Vector<3, float>& Entry, Vector<3, float>& Exit)
		{
			Vector<3, float> Offset = Shape.Center - Other.Origin;

			float A = VectorUtility::SqrMagnitude(Other.Direction);
			float B = -2.0f * VectorUtility::Dot(Other.Direction, Offset);
			float C = VectorUtility::SqrMagnitude(Offset) - Math::Square(Shape.Radius);

			float Discriminant = B * B - 4.0f * A * C;
			if (Discriminant < 0.0f)
			{
				return false;
			}

			float SqrtDiscriminant = Math::Sqrt(Discriminant);
			float Inv2A = 1.0f / (2.0f * A);

			float EntryDistance = (B - SqrtDiscriminant) * Inv2A;
			float ExitDistance = (B + SqrtDiscriminant) * Inv2A;

			if (EntryDistance > ExitDistance)
			{
				float Temp = EntryDistance;
				EntryDistance = ExitDistance;
				ExitDistance = Temp;
			}

			if (ExitDistance < 0.0f)
			{
				return false;
			}

			if (EntryDistance < 0.0f)
			{
				EntryDistance = 0.0f;
			}

			Entry = Other.Origin + Other.Direction * EntryDistance;
			Exit = Other.Origin + Other.Direction * ExitDistance;

			return true;
		}

		inline bool Intersect(const Sphere& Shape, const Ray& Other)
		{
			Vector<3, float> Entry, Exit;
			return Intersect(Shape, Other, Entry, Exit);
		}

		inline bool Intersect(const Sphere& Shape, const Plane& Other, Vector<3, float>& Entry, Vector<3, float>& Exit)
		{
			float SignedDistance = VectorUtility::Dot(Other.Normal, Shape.Center) + Other.Distance;
			float AbsDistance = Math::Abs(SignedDistance);

			if (AbsDistance > Shape.Radius)
			{
				return false;
			}

			Vector<3, float> ClosestPoint = Shape.Center - Other.Normal * SignedDistance;
			float RadiusToEdge = Math::Sqrt(Math::Square(Shape.Radius) - Math::Square(AbsDistance));
			Vector<3, float> TangentDir = VectorUtility::Normalize(VectorUtility::Orthogonal(Other.Normal));
			Vector<3, float> Offset = TangentDir * RadiusToEdge;

			Entry = ClosestPoint - Offset;
			Exit = ClosestPoint + Offset;

			return true;
		}

		inline bool Intersect(const Sphere& Shape, const Plane& Other)
		{
			Vector<3, float> Entry, Exit;
			return Intersect(Shape, Other, Entry, Exit);
		}

		inline Vector<3, float> Closest(const Ray& Shape, Vector<3, float> Position)
		{
			float Projection = VectorUtility::Dot(Shape.Direction, Position - Shape.Origin);
			return Shape.Origin + Shape.Direction * Projection;
		}

		inline Vector<3, float> Closest(const Plane& Shape, Vector<3, float> Position)
		{
			float Projection = VectorUtility::Dot(Shape.Normal, Position);
			float Factor = Projection - Shape.Distance;
			return Position - Shape.Normal * Factor;
		}

		inline Vector<3, float> Closest(const Triangle& Shape, Vector<3, float> Position)
		{
			Vector<3, float> AB = Shape.B - Shape.A;
			Vector<3, float> AC = Shape.C - Shape.A;
			Vector<3, float> AP = Position - Shape.A;

			float D1 = VectorUtility::Dot(AB, AP);
			float D2 = VectorUtility::Dot(AC, AP);

			if (D1 <= 0.0f && D2 <= 0.0f)
			{
				return Shape.A;
			}

			Vector<3, float> BP = Position - Shape.B;
			float D3 = VectorUtility::Dot(AB, BP);
			float D4 = VectorUtility::Dot(AC, BP);
			if (D3 >= 0.0f && D4 <= D3)
			{
				return Shape.B;
			}

			float Vc = D1 * D4 - D3 * D2;
			if (Vc <= 0.0f && D1 >= 0.0f && D3 <= 0.0f)
			{
				float v = D1 / (D1 - D3);
				return Shape.A + AB * v;
			}

			Vector<3, float> Cp = Position - Shape.C;
			float D5 = VectorUtility::Dot(AB, Cp);
			float D6 = VectorUtility::Dot(AC, Cp);
			if (D6 >= 0.0f && D5 <= D6) return Shape.C;

			float Vb = D5 * D2 - D1 * D6;
			if (Vb <= 0.0f && D2 >= 0.0f && D6 <= 0.0f)
			{
				float W = D2 / (D2 - D6);
				return Shape.A + AC * W;
			}

			float Va = D3 * D6 - D5 * D4;
			if (Va <= 0.0f && (D4 - D3) >= 0.0f && (D5 - D6) >= 0.0f)
			{
				float W = (D4 - D3) / ((D4 - D3) + (D5 - D6));
				return Shape.B + (Shape.C - Shape.B) * W;
			}

			float Denom = 1.0f / (Va + Vb + Vc);
			float V = Vb * Denom;
			float W = Vc * Denom;
			return Shape.A + AB * V + AC * W;
		}

		inline Vector<2, float> Closest(const Rectangle& Shape, Vector<2, float> Position)
		{
			Vector<2, float> Min = Shape.GetBottomLeft();
			Vector<2, float> Max = Shape.GetTopRight();
			return VectorUtility::Clamp(Position, Min, Max);
		}

		inline Vector<2, float> Closest(const Circle& Shape, Vector<2, float> Position)
		{
			Vector<2, float> Direction = VectorUtility::Normalize(Position - Shape.Center);
			return Shape.Center + Shape.Radius * Direction;
		}

		inline Vector<3, float> Closest(const Box& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Min = Shape.GetMin();
			Vector<3, float> Max = Shape.GetMax();
			return VectorUtility::Clamp(Position, Min, Max);
		}

		inline Vector<3, float> Closest(const Cuboid& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Local = Shape.Orientation.Inverse() * (Position - Shape.Center);
			Vector<3, float> Point = VectorUtility::Clamp(Local, -Shape.Extents, Shape.Extents);
			return Shape.Center + Shape.Orientation * Point;
		}

		inline Vector<3, float> Closest(const Sphere& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Direction = VectorUtility::Normalize(Position - Shape.Center);
			return Shape.Center + Shape.Radius * Direction;
		}

		inline float Distance(const Ray& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Vec = Position - Shape.Origin;
			float Projection = Math::Max(0.0f, VectorUtility::Dot(Shape.Direction, Vec));
			float SquaredMagnitude = VectorUtility::SqrMagnitude(Vec);
			return Math::Sqrt(SquaredMagnitude - Math::Square(Projection));
		}

		inline float Distance(const Plane& Shape, Vector<3, float> Position)
		{
			return VectorUtility::Dot(Shape.Normal, Position) - Shape.Distance;
		}

		inline float Distance(const Triangle& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Point = Closest(Shape, Position);
			Vector<3, float> Target = Position - Point;
			return VectorUtility::Dot(Target, Shape.Normal());
		}

		inline float Distance(const Rectangle& Shape, Vector<2, float> Position)
		{
			Vector<2, float> Distance = VectorUtility::Abs(Position - Shape.Center) - Shape.Extents;
			Vector<2, float> Out = VectorUtility::Max(Distance, Vector<2, float>::Zero);

			float Outside = VectorUtility::Magnitude(Out);
			float Inside = Math::Max(Distance.x, Distance.y);

			return (Distance.x > 0.0f || Distance.y > 0.0f) ? Outside : Inside;
		}

		inline float Distance(const Circle& Shape, Vector<2, float> Position)
		{
			return VectorUtility::Distance(Shape.Center, Position) - Shape.Radius;
		}

		inline float Distance(const Box& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Distance = VectorUtility::Abs(Position - Shape.Center) - Shape.Extents;
			Vector<3, float> Out = VectorUtility::Max(Distance, Vector<3, float>::Zero);

			float Outside = VectorUtility::Magnitude(Out);
			float Inside = Math::Max(Distance.x, Math::Max(Distance.y, Distance.z));

			return (Distance.x > 0.0f || Distance.y > 0.0f || Distance.z > 0.0f) ? Outside : Inside;
		}

		inline float Distance(const Cuboid& Shape, Vector<3, float> Position)
		{
			Vector<3, float> Local = Shape.Orientation.Inverse() * (Position - Shape.Center);

			Vector<3, float> Distance = VectorUtility::Abs(Local) - Shape.Extents;
			Vector<3, float> Out = VectorUtility::Max(Distance, Vector<3, float>::Zero);

			float Outside = VectorUtility::Magnitude(Out);
			float Inside = Math::Max(Distance.x, Math::Max(Distance.y, Distance.z));

			return (Distance.x > 0.0f || Distance.y > 0.0f || Distance.z > 0.0f) ? Outside : Inside;
		}

		inline float Distance(const Sphere& Shape, Vector<3, float> Position)
		{
			return VectorUtility::Distance(Shape.Center, Position) - Shape.Radius;
		}
	}

#pragma endregion

#pragma region Hashing

	namespace Hashing
	{
		template<typename H>
		class HashProcess<Ray, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Ray& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Origin);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Direction);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Ray& Data)
			{
				HashProcess<Ray, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Plane, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Plane& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Normal);
				HashProcess<float, H>::Accumulate(State, Data.Distance);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Plane& Data)
			{
				HashProcess<Plane, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Triangle, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Triangle& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.A);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.B);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.C);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Triangle& Data)
			{
				HashProcess<Triangle, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Rectangle, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Rectangle& Data)
			{
				HashProcess<Vector<2, float>, H>::Accumulate(State, Data.Center);
				HashProcess<Vector<2, float>, H>::Accumulate(State, Data.Extents);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Rectangle& Data)
			{
				HashProcess<Rectangle, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Circle, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Circle& Data)
			{
				HashProcess<Vector<2, float>, H>::Accumulate(State, Data.Center);
				HashProcess<float, H>::Accumulate(State, Data.Radius);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Circle& Data)
			{
				HashProcess<Circle, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Box, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Box& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Center);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Extents);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Box& Data)
			{
				HashProcess<Box, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Cuboid, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Cuboid& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Center);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Extents);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Cuboid& Data)
			{
				HashProcess<Cuboid, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};
		
		template<typename H>
		class HashProcess<Sphere, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Sphere& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Center);
				HashProcess<float, H>::Accumulate(State, Data.Radius);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Sphere& Data)
			{
				HashProcess<Sphere, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};
	}

#pragma endregion
}
