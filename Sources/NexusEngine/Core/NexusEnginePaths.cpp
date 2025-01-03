#include "Core/NexusEnginePch.h"
#include "NexusEnginePaths.h"

namespace NxEn
{
	namespace Paths
	{
		Path Root = Path::GetWorkingDirectory();
		Path Assets = Root + "Assets";
		Path Binaries = Root + "binaries";
		Path Build = Root + "builds";
		Path Configs = Root + "Configs";
		Path Cook = Root + "cook";
		Path Intermediates = Root + "intermediates";
		Path Libraries = Root + "Libraries";
		Path Resources = Root + "Resources";
		Path Saved = Root + "saved";
		Path Scripts = Root + "Scripts";
		Path Sources = Root + "Sources";
	}
}
