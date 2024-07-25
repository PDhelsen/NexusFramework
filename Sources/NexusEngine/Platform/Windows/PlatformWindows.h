#pragma once

#include "Core/NexusEngineCore.h"
#include "Platform/Platform.h"

namespace NxEn
{
	// Windows implementation of the platform API
	// Only Windows x64 is supported

	class PlatformWindows : public Platform
	{
		friend Platform;

	public:
		NEXUS_ENGINE_API void WaitForUserToCloseConsole() const override;
		NEXUS_ENGINE_API void WriteToConsole(const StringView& Message, ConsoleColor Color = ConsoleColor::White) const override;
		NEXUS_ENGINE_API void WriteToOutput(const StringView& Message) const override;
		NEXUS_ENGINE_API void ExecuteFromDll(const StringView& DllName, uint8 Ordinal) const override;

		NEXUS_ENGINE_API inline PlatformTarget GetTarget() override { return PlatformTarget::Windows; }

	protected:
		PlatformWindows();
		~PlatformWindows();

	private:
		void InitializeConsole();
	};
}
