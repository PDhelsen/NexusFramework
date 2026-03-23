#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

#include "NexusFramework/IO/Directory.h"

namespace NxFr
{
	namespace Paths
	{
		String Root = "";

		String Assets = "";
		String Configs = "";
		String Libraries = "";
		String Resources = "";
		String Scripts = "";
		String Sources = "";

		String Builds = "";
		String Cooked = "";
		String Saved = "";
		String Temp = "";

		void SetupPathsAndFolders()
		{
			Root = Path::GetWorkingDirectory();

			Assets = Path::Combine(Root, "Assets");
			Configs = Path::Combine(Root, "Configs");
			Libraries = Path::Combine(Root, "Libraries");
			Resources = Path::Combine(Root, "Resources");
			Scripts = Path::Combine(Root, "Scripts");
			Sources = Path::Combine(Root, "Sources");

			Builds = Path::Combine(Root, "builds");
			Cooked = Path::Combine(Root, "cooked");
			Saved = Path::Combine(Root, "saved");
			Temp = Path::Combine(Root, "temp");

			Directory(Paths::Cooked).Create();
			Directory(Paths::Saved).Create();
			Directory(Paths::Temp).Create();
		}

		void CleanupFolders()
		{
			Directory(Paths::Temp).Delete();
		}
	}
}
