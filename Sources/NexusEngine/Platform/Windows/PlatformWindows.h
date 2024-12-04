#pragma once

#include "Core/NexusEngineCore.h"
#include "Platform/Platform.h"

namespace NxEn
{
	class PlatformWindows : public Platform
	{
		friend Platform;

	public:
		NEXUS_ENGINE_API void WaitForUserToCloseTerminal() const override;
		NEXUS_ENGINE_API void WriteToTerminal(StringView Message, TerminalColor Color = TerminalColor::White) const override;
		NEXUS_ENGINE_API void WriteToDebugger(StringView Message) const override;
		NEXUS_ENGINE_API void ExecuteFromDll(StringView DllName, uint8 Ordinal) const override;
		NEXUS_ENGINE_API double GetProcessorTimer(double Unit = 1.0) const override;
		NEXUS_ENGINE_API PathType GetPathType(StringView Path) const override;
		NEXUS_ENGINE_API String GetWorkingDirectory() const override;
		NEXUS_ENGINE_API List<String> DirectoryContent(StringView Path) const override;
		NEXUS_ENGINE_API bool DirectoryCreate(StringView Path) const override;
		NEXUS_ENGINE_API bool DirectoryMove(StringView Path, StringView Target) const override;
		NEXUS_ENGINE_API bool DirectoryDelete(StringView Path) const override;
		NEXUS_ENGINE_API bool FileCreate(StringView Path) const override;
		NEXUS_ENGINE_API bool FileMove(StringView Path, StringView Target) const override;
		NEXUS_ENGINE_API bool FileDelete(StringView Path) const override;

		NEXUS_ENGINE_API inline PlatformTarget GetTarget() override { return PlatformTarget::Windows; }

	protected:
		PlatformWindows();
		~PlatformWindows();

	private:
		void InitializeTerminal();
		void InitializePerformanceTimer();

		double PerformanceFrequency;
	};
}
