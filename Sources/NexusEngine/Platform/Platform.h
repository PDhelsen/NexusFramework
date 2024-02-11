#pragma once

#include "Core/NexusCore.h"

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
		NEXUS_ENGINE_API virtual void WaitForUserToCloseConsole() const {};
		NEXUS_ENGINE_API virtual void WriteToConsole(const char* Message, uint8 Color) const {};
		NEXUS_ENGINE_API virtual void WriteToOutput(const char* Message) const {};
		NEXUS_ENGINE_API virtual void ExecuteFromDll(const char* DllName, uint8 Ordinal) const {};
		NEXUS_ENGINE_API virtual const char* GetTimestamp(const char* Format) const { return nullptr; };

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
