#pragma once

#include "Core/NexusCore.h"
#include "Debug/Logger.h"

namespace NxEn
{
		class Platform
	{
	public:
		enum class Target : int8
		{
			None,
			Windows
		};
		
		NEXUS_ENGINE_API virtual void WaitForUserToCloseConsole() const {};
		NEXUS_ENGINE_API virtual void WriteToConsole(const char* Message, Logger::Verbosity Verbosity) const {};
		NEXUS_ENGINE_API virtual void WriteToOutput(const char* Message, Logger::Verbosity Verbosity) const {};
		NEXUS_ENGINE_API virtual void ExecuteFromDll(const char* DllName, int8 Ordinal) const {};
		NEXUS_ENGINE_API virtual const char* GetTimestamp(const char* Format) const { return nullptr; };

		NEXUS_ENGINE_API inline virtual Target GetTarget() { return Target::None; }

		NEXUS_ENGINE_API inline static Platform* GetInstance() { return Instance; }

	protected:
		Platform() = default;
		~Platform() = default;

	private:
		static Platform* Create();

		static Platform* Instance;
	};
}
