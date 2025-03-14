#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

#include "NexusFramework/IO/Directory.h"

namespace NxFr
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

		void CreateFrameworkFolders()
		{
			Directory(Paths::Cooked).Create();
			Directory(Paths::Saved).Create();
		}
	}
}
