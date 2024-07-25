#pragma once

#include "Core/NexusEngineCore.h"
#include "Platform/Platform.h"

namespace NxEn
{
	class PlatformNone : public Platform
	{
		friend Platform;

	public:
		NEXUS_ENGINE_API void WaitForUserToCloseConsole() const override;
		NEXUS_ENGINE_API void WriteToConsole(StringView Message, ConsoleColor Color = ConsoleColor::White) const override;
		NEXUS_ENGINE_API void WriteToOutput(StringView Message) const override;
		NEXUS_ENGINE_API void ExecuteFromDll(StringView DllName, uint8 Ordinal) const override;

		NEXUS_ENGINE_API inline PlatformTarget GetTarget() override { return PlatformTarget::None; }

	protected:
		PlatformNone();
		~PlatformNone();
	};
}
