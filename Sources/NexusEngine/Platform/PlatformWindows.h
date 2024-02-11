#pragma once

#include "Core/NexusCore.h"
#include "Platform.h"

namespace NxEn
{
	class PlatformWindows : public Platform
	{
		friend Platform;

		NEXUS_ENGINE_API void WaitForUserCloseConsole() const override;
		NEXUS_ENGINE_API void WriteToConsole(const char* Message, Logger::Verbosity Verbosity) const override;
		NEXUS_ENGINE_API void WriteToOutput(const char* Message, Logger::Verbosity Verbosity) const override;
		NEXUS_ENGINE_API void ExecuteFromDll(const char* DllName, int8 Ordinal) const override;

		NEXUS_ENGINE_API inline Target GetTarget() override { return Target::Windows; }

	protected:
		PlatformWindows();
		~PlatformWindows();

	private:
		void InitializeConsole();
	};
}
