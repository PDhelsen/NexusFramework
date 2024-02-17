#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	// All the supported platforms

	enum class PlatformTarget : uint8
	{
		None,
		Windows
	};

	// Abstract API of platform dependant code

	class Platform
	{
	public:
		enum class ConsoleColor : uint8
		{
			White,
			Black,
			Red,
			Green,
			Blue,
			Yellow,
			Cyan,
			Magenta
		};

		NEXUS_ENGINE_API virtual void WaitForUserToCloseConsole() const = 0;
		NEXUS_ENGINE_API virtual void WriteToConsole(const char* Message, ConsoleColor Color = ConsoleColor::White) const = 0;
		NEXUS_ENGINE_API virtual void WriteToOutput(const char* Message) const = 0;
		NEXUS_ENGINE_API virtual void ExecuteFromDll(const char* DllName, uint8 Ordinal) const = 0;
		NEXUS_ENGINE_API virtual const char* GetTimestamp(const char* Format) const = 0;

		NEXUS_ENGINE_API inline virtual PlatformTarget GetTarget() { return PlatformTarget::None; }

		NEXUS_ENGINE_API inline static Platform* GetInstance() { return Instance; }

	protected:
		Platform() = default;
		~Platform() = default;

	private:
		static Platform* Create();

		static Platform* Instance;
	};
}
