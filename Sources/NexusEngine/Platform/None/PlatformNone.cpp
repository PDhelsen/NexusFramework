#include "Core/NexusEnginePch.h"
#include "PlatformNone.h"

namespace NxEn
{
    void PlatformNone::WaitForUserToCloseConsole() const
    {
    }

    void PlatformNone::WriteToConsole(const StringView& Message, ConsoleColor Color) const
    {
    }

    void PlatformNone::WriteToOutput(const StringView& Message) const
    {
    }

    void PlatformNone::ExecuteFromDll(const StringView& DllName, uint8 Ordinal) const
    {
    }

    PlatformNone::PlatformNone()
    {
    }

    PlatformNone::~PlatformNone()
    {
    }
}
