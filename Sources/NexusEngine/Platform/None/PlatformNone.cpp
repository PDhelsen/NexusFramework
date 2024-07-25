#include "Core/NexusEnginePch.h"
#include "PlatformNone.h"

namespace NxEn
{
    void PlatformNone::WaitForUserToCloseConsole() const
    {
    }

    void PlatformNone::WriteToConsole(StringView Message, ConsoleColor Color) const
    {
    }

    void PlatformNone::WriteToOutput(StringView Message) const
    {
    }

    void PlatformNone::ExecuteFromDll(StringView DllName, uint8 Ordinal) const
    {
    }

    PlatformNone::PlatformNone()
    {
    }

    PlatformNone::~PlatformNone()
    {
    }
}
