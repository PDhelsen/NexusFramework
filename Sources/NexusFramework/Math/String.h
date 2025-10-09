#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Debug/Logger/Log.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"
#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Math/Rotation.h"
#include "NexusFramework/Math/Matrix.h"
#include "NexusFramework/Math/Shape.h"

namespace NxFr
{
	template<uint8 D, typename T>
	struct StringConverter<Vector<D, T>>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "";
		}

		static void ToString(const Vector<D, T>& Data, String& Result, StringView Format = "")
		{
			for (uint8 Index = 0; Index < Vector<D, T>::Count; Index++)
			{
				Result += StringUtility::ToString<T>(Data[Index]);
			}
		}

		static void FromString(StringView Data, Vector<D, T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with un-specified vector");
		}
	};

	template<>
	struct StringConverter<Vector2f>
	{
		static StringView GetFormat(bool Pretty)
		{
			return Pretty ? "(%.2f, %.2f)" : "(%f, %f)";
		}

		static void ToString(const Vector2f& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Vector2f>(Format, true), Data.x, Data.y);
		}

		static void FromString(StringView Data, Vector2f& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Vector2f>(Format, false), &Result.x, &Result.y);
		}
	};

	template<>
	struct StringConverter<Vector2i>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "(%d, %d)";
		}

		static void ToString(const Vector2i& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Vector2i>(Format), Data.x, Data.y);
		}

		static void FromString(StringView Data, Vector2i& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Vector2i>(Format), &Result.x, &Result.y);
		}
	};

	template<>
	struct StringConverter<Vector3f>
	{
		static StringView GetFormat(bool Pretty)
		{
			return Pretty ? "(%.2f, %.2f, %.2f)" : "(%f, %f, %f)";
		}

		static void ToString(const Vector3f& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Vector3f>(Format, true), Data.x, Data.y, Data.z);
		}

		static void FromString(StringView Data, Vector3f& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Vector3f>(Format, false), &Result.x, &Result.y, &Result.z);
		}
	};

	template<>
	struct StringConverter<Vector3i>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "(%d, %d, %d)";
		}

		static void ToString(const Vector3i& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Vector3i>(Format), Data.x, Data.y, Data.z);
		}

		static void FromString(StringView Data, Vector3i& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Vector3i>(Format), &Result.x, &Result.y, &Result.z);
		}
	};

	template<>
	struct StringConverter<Vector4f>
	{
		static StringView GetFormat(bool Pretty)
		{
			return Pretty ? "(%.2f, %.2f, %.2f, %.2f)" : "(%f, %f, %f, %f)";
		}

		static void ToString(const Vector4f& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Vector4f>(Format, true), Data.x, Data.y, Data.z, Data.w);
		}

		static void FromString(StringView Data, Vector4f& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Vector4f>(Format, false), &Result.x, &Result.y, &Result.z, &Result.w);
		}
	};

	template<>
	struct StringConverter<Vector4i>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "(%d, %d, %d, %d)";
		}

		static void ToString(const Vector4i& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Vector4i>(Format), Data.x, Data.y, Data.z, Data.w);
		}

		static void FromString(StringView Data, Vector4i& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Vector4i>(Format), &Result.x, &Result.y, &Result.z, &Result.w);
		}
	};

	template<uint8 R, uint8 C, typename T>
	struct StringConverter<Matrix<R, C, T>>
	{
		static void ToString(const Matrix<R, C, T>& Data, String& Result, StringView Format = "")
		{
			for (uint8 Column = 0; Column < C; ++Column)
			{
				for (uint8 Row = 0; Row < R; ++Row)
				{
					Result += StringUtility::ToString<T>(Data(Row, Column));
					Result += ", ";
				}
				Result += StringUtility::NewLine;

			}
		}

		static void FromString(StringView Data, Matrix<R, C, T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with matrix");
		}
	};

	template<>
	struct StringConverter<Matrix4x4f>
	{
		static void ToString(const Matrix4x4f& Data, String& Result, StringView Format = "")
		{
			for (uint8 Column = 0; Column < 4; ++Column)
			{
				for (uint8 Row = 0; Row < 4; ++Row)
				{
					Result += StringUtility::ToString<float>(Data(Row, Column));
					Result += ", ";
				}
				Result += StringUtility::NewLine;

			}
		}

		static void FromString(StringView Data, Matrix4x4f& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with matrix");
		}
	};

	template<>
	struct StringConverter<AxisAngle>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "Axis: (%f, %f, %f) - Angle: %f";
		}

		static void ToString(const AxisAngle& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<AxisAngle>(Format), Data.Axis.x, Data.Axis.y, Data.Axis.z, Data.Angle);
		}

		static void FromString(StringView Data, AxisAngle& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<AxisAngle>(Format), &Result.Axis.x, &Result.Axis.y, &Result.Axis.z, &Result.Angle);
		}
	};

	template<>
	struct StringConverter<Euler>
	{
		static StringView GetFormat(bool Pretty)
		{
			return Pretty ? "(%.2f, %.2f, %.2f)" : "(%f, %f, %f)";
		}

		static void ToString(const Euler& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Euler>(Format, true), Data.x, Data.y, Data.z);
		}

		static void FromString(StringView Data, Euler& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Euler>(Format, false), &Result.x, &Result.y, &Result.z);
		}
	};

	template<>
	struct StringConverter<Quaternion>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "(%f, %f, %f, %f)";
		}

		static void ToString(const Quaternion& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Quaternion>(Format), Data.x, Data.y, Data.z, Data.w);
		}

		static void FromString(StringView Data, Quaternion& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Quaternion>(Format), &Result.x, &Result.y, &Result.z, &Result.w);
		}
	};

	template<>
	struct StringConverter<Ray>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "Origin: (%f, %f, %f) - Direction: (%f, %f, %f)";
		}

		static void ToString(const Ray& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Ray>(Format), Data.Origin.x, Data.Origin.y, Data.Direction.z, Data.Direction.x, Data.Direction.y, Data.Direction.z);
		}

		static void FromString(StringView Data, Ray& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Ray>(Format), &Result.Origin.x, &Result.Origin.y, &Result.Direction.z, &Result.Direction.x, &Result.Direction.y, &Result.Direction.z);
		}
	};

	template<>
	struct StringConverter<Plane>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "Normal: (%f, %f, %f) - Distance: %f";
		}

		static void ToString(const Plane& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Plane>(Format), Data.Normal.x, Data.Normal.y, Data.Normal.z, Data.Distance);
		}

		static void FromString(StringView Data, Plane& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Plane>(Format), &Result.Normal.x, &Result.Normal.y, &Result.Normal.z, &Result.Distance);
		}
	};

	template<>
	struct StringConverter<Triangle>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "A: (%f, %f, %f) - B: (%f, %f, %f) - C: (%f, %f, %f)";
		}

		static void ToString(const Triangle& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Triangle>(Format), Data.A.x, Data.A.y, Data.A.z, Data.B.x, Data.B.y, Data.B.z, Data.C.x, Data.C.y, Data.C.z);
		}

		static void FromString(StringView Data, Triangle& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Triangle>(Format), &Result.A.x, &Result.A.y, &Result.A.z, &Result.B.x, &Result.B.y, &Result.B.z, &Result.C.x, &Result.C.y, &Result.C.z);
		}
	};

	template<>
	struct StringConverter<Rectangle>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "Center: (%f, %f) - Extents: (%f, %f)";
		}

		static void ToString(const Rectangle& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Rectangle>(Format), Data.Center.x, Data.Center.y, Data.Extents.x, Data.Extents.y);
		}

		static void FromString(StringView Data, Rectangle& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Rectangle>(Format), &Result.Center.x, &Result.Center.y, &Result.Extents.x, &Result.Extents.y);
		}
	};

	template<>
	struct StringConverter<Circle>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "Center: (%f, %f) - Radius: %f";
		}

		static void ToString(const Circle& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Circle>(Format), Data.Center.x, Data.Center.y, Data.Radius);
		}

		static void FromString(StringView Data, Circle& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Circle>(Format), &Result.Center.x, &Result.Center.y, &Result.Radius);
		}
	};

	template<>
	struct StringConverter<Box>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "Center: (%f, %f, %f) - Extents: (%f, %f, %f)";
		}

		static void ToString(const Box& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Box>(Format), Data.Center.x, Data.Center.y, Data.Center.z, Data.Extents.x, Data.Extents.y, Data.Extents.z);
		}

		static void FromString(StringView Data, Box& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Box>(Format), &Result.Center.x, &Result.Center.y, &Result.Center.z, &Result.Extents.x, &Result.Extents.y, &Result.Extents.z);
		}
	};

	template<>
	struct StringConverter<Cuboid>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "Center: (%f, %f, %f) - Orientation: (%f, %f, %f, %f) - Extents: (%f, %f, %f)";
		}

		static void ToString(const Cuboid& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Cuboid>(Format), Data.Center.x, Data.Center.y, Data.Center.z, Data.Orientation.x, Data.Orientation.y, Data.Orientation.z, Data.Orientation.w, Data.Extents.x, Data.Extents.y, Data.Extents.z);
		}

		static void FromString(StringView Data, Cuboid& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Cuboid>(Format), &Result.Center.x, &Result.Center.y, &Result.Center.z, &Result.Orientation.x, &Result.Orientation.y, &Result.Orientation.z, &Result.Orientation.w, &Result.Extents.x, &Result.Extents.y, &Result.Extents.z);
		}
	};

	template<>
	struct StringConverter<Sphere>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "Center: (%f, %f, %f) - Radius: %f";
		}

		static void ToString(const Sphere& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Sphere>(Format), Data.Center.x, Data.Center.y, Data.Center.z, Data.Radius);
		}

		static void FromString(StringView Data, Sphere& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Sphere>(Format), &Result.Center.x, &Result.Center.y, &Result.Center.z, Result.Radius);
		}
	};
}
