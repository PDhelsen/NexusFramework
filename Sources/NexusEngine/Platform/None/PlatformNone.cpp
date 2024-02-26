#include "Core/NexusEnginePch.h"
#include "PlatformNone.h"

namespace NxEn
{
    void PlatformNone::WaitForUserToCloseConsole() const
    {
    }

    void PlatformNone::WriteToConsole(const char* Message, ConsoleColor Color) const
    {
    }

    void PlatformNone::WriteToOutput(const char* Message) const
    {
    }

    void PlatformNone::ExecuteFromDll(const char* DllName, uint8 Ordinal) const
    {
    }

    PlatformNone::PlatformNone()
    {
    }

    PlatformNone::~PlatformNone()
    {
    }
}