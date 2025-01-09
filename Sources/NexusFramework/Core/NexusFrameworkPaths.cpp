#include "Core/NexusFrameworkPch.h"
#include "NexusFrameworkPaths.h"

namespace NxEn
{
	namespace Paths
	{
		Path Root = Path::GetWorkingDirectory();

		Path Assets = Root + "Assets";
		Path Configs = Root + "Configs";
		Path Libraries = Root + "Libraries";
		Path Resources = Root + "Resources";
		Path Scripts = Root + "Scripts";
		Path Sources = Root + "Sources";

		Path Builds = Root + "builds";
		Path Cooked = Root + "cooked";
		Path Saved = Root + "saved";
	}
}
