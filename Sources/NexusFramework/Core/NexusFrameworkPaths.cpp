#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

#include "NexusFramework/IO/Directory.h"

namespace NxFr
{
	namespace Paths
	{
		Path Root = Path("");

		Path Assets = Path("");
		Path Configs = Path("");
		Path Libraries = Path("");
		Path Resources = Path("");
		Path Scripts = Path("");
		Path Sources = Path("");

		Path Builds = Path("");
		Path Cooked = Path("");
		Path Saved = Path("");

		void SetupPathsAndFolders()
		{
			Root = Path::GetWorkingDirectory();

			Assets = Root + "Assets";
			Configs = Root + "Configs";
			Libraries = Root + "Libraries";
			Resources = Root + "Resources";
			Scripts = Root + "Scripts";
			Sources = Root + "Sources";

			Builds = Root + "builds";
			Cooked = Root + "cooked";
			Saved = Root + "saved";

			Directory(Paths::Cooked).Create();
			Directory(Paths::Saved).Create();
		}
	}
}
