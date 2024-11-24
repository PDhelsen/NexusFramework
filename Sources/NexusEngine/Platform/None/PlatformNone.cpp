#include "Core/NexusEnginePch.h"
#include "PlatformNone.h"

namespace NxEn
{
    void PlatformNone::WaitForUserToCloseTerminal() const
    {
    }

    void PlatformNone::WriteToTerminal(StringView Message, TerminalColor Color) const
    {
    }

    void PlatformNone::WriteToDebugger(StringView Message) const
    {
    }

    void PlatformNone::ExecuteFromDll(StringView DllName, uint8 Ordinal) const
    {
    }

	double PlatformNone::GetProcessorTimer(double Unit) const
    {
		return 0.0;
    }

    PlatformNone::PlatformNone()
    {
    }

    PlatformNone::~PlatformNone()
    {
    }
}
