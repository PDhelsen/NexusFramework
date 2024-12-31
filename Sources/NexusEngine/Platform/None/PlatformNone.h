#pragma once

#include "Core/NexusEngineCore.h"
#include "Platform/Platform.h"

namespace NxEn
{
	class PlatformNone : public Platform
	{
		friend Platform;

	public:
		NEXUS_ENGINE_API void ExecuteFromDll(StringView DllName, uint8 Ordinal) const override;
		NEXUS_ENGINE_API void Sleep(uint64 Milliseconds) const override;
		NEXUS_ENGINE_API double GetProcessorTimer(double Unit = 1.0) const override;

		NEXUS_ENGINE_API void WaitForUserToCloseTerminal() const override;
		NEXUS_ENGINE_API void WriteToTerminal(StringView Message) const override;
		NEXUS_ENGINE_API void WriteToDebugger(StringView Message) const override;

		NEXUS_ENGINE_API PathType GetPathType(StringView Path) const override;
		NEXUS_ENGINE_API String GetWorkingDirectory() const override;

		NEXUS_ENGINE_API void DirectoryCreate(StringView Path) const override;
		NEXUS_ENGINE_API void DirectoryMove(StringView Path, StringView Target, bool Override = false) const override;
		NEXUS_ENGINE_API void DirectoryCopy(StringView Path, StringView Target, bool Override = false) const override;
		NEXUS_ENGINE_API void DirectoryDelete(StringView Path) const override;
		NEXUS_ENGINE_API List<String> DirectoryContent(StringView Path) const override;

		NEXUS_ENGINE_API void* FileCreate(StringView Path, bool KeepOpen) const override;
		NEXUS_ENGINE_API void FileMove(StringView Path, StringView Target, bool Override = false) const override;
		NEXUS_ENGINE_API void FileCopy(StringView Path, StringView Target, bool Override = false) const override;
		NEXUS_ENGINE_API void FileDelete(StringView Path) const override;
		NEXUS_ENGINE_API void* FileOpen(StringView Path, FileMode Mode) const override;
		NEXUS_ENGINE_API void FileClose(void* File) const override;
		NEXUS_ENGINE_API uint64 FileSize(void* File) const override;
		NEXUS_ENGINE_API void FileWriteByte(void* File, BufferView<Byte> Data) const override;
		NEXUS_ENGINE_API Buffer<Byte> FileReadByte(void* File) const override;
		NEXUS_ENGINE_API void FileWriteText(void* File, StringView Text) const override;
		NEXUS_ENGINE_API String FileReadText(void* File) const override;

		NEXUS_ENGINE_API inline PlatformTarget GetTarget() override { return PlatformTarget::None; }

	protected:
		PlatformNone();
		virtual ~PlatformNone();
	};
}
