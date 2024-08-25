#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	enum class PlatformTarget : uint8
	{
		None,
		Windows
	};

	class Platform
	{
	public:
		enum class TerminalColor : uint8
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

		NEXUS_ENGINE_API virtual void WaitForUserToCloseTerminal() const = 0;
		NEXUS_ENGINE_API virtual void WriteToTerminal(StringView Message, TerminalColor Color = TerminalColor::White) const = 0;
		NEXUS_ENGINE_API virtual void WriteToDebugger(StringView Message) const = 0;
		NEXUS_ENGINE_API virtual void ExecuteFromDll(StringView DllName, uint8 Ordinal) const = 0;

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
