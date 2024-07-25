#pragma once

#include "Core/NexusEngineCore.h"
#include "Platform/Platform.h"

namespace NxEn
{
	// Empty implementation used when no platform are defined

	class PlatformNone : public Platform
	{
		friend Platform;

	public:
		NEXUS_ENGINE_API void WaitForUserToCloseConsole() const override;
		NEXUS_ENGINE_API void WriteToConsole(const StringView& Message, ConsoleColor Color = ConsoleColor::White) const override;
		NEXUS_ENGINE_API void WriteToOutput(const StringView& Message) const override;
		NEXUS_ENGINE_API void ExecuteFromDll(const StringView& DllName, uint8 Ordinal) const override;

		NEXUS_ENGINE_API inline PlatformTarget GetTarget() override { return PlatformTarget::None; }

	protected:
		PlatformNone();
		~PlatformNone();
	};
}
