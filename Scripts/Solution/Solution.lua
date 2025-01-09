workspace "NexusFramework"
    location "../../"
    startproject "NexusSandbox"
    debugcommand "../../builds/artifacts/NexusSandbox.exe"
	debugdir "../../"

    configurations { "Debug", "Release", "Ditrib" }
    platforms { "Win64" }
    flags { "MultiProcessorCompile" }

    filter "action:vs*"
        toolset "msc"

    filter "platforms:Win64"
        defines { "NEXUS_WINDOWS" }
        architecture "x64"

    filter "configurations:Debug"
        defines { "NEXUS_DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NEXUS_RELEASE" }
        symbols "On"
        optimize "On"

    filter "configurations:Ditrib"
        defines { "NEXUS_DISTRIB" }
        symbols "Off"
        optimize "On"

    filter "toolset:msc"
        defines { "NEXUS_MSVC" }

group "Test"
project "NexusSandbox"
project "NexusTest"
group ""
group "Libraries"
project "GoogleTest"
group ""

project "NexusFramework"
    location "../../Sources/%{prj.name}/"

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../builds/binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../builds/intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    pchheader "Core/NexusFrameworkPch.h"
	pchsource "../../Sources/%{prj.name}/Core/NexusFrameworkPch.cpp"

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp",
        "../../Sources/%{prj.name}/**.natvis",
    }

    includedirs
    {
        "../../Sources/%{prj.name}/"
    }

    defines
    {
        "NEXUS_FRAMEWORK_DLL_BUILD"
    }

    postbuildcommands
    {
        "../../Scripts/Build/PostBuild.bat %{cfg.buildtarget.directory}"
    }

project "NexusSandbox"
    location "../../Sources/%{prj.name}/"

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../builds/binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../builds/intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp"
    }

    includedirs
    {
        "../../Sources/%{prj.name}/",
        "../../Sources/NexusFramework/"
    }

    links
    {
        "NexusFramework",
    }

    postbuildcommands
    {
        "../../Scripts/Build/PostBuild.bat %{cfg.buildtarget.directory}"
    }

project "NexusTest"
    location "../../Sources/%{prj.name}/"

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../builds/binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../builds/intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    pchheader "Core/NexusTestPch.h"
	pchsource "../../Sources/%{prj.name}/Core/NexusTestPch.cpp"

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp",
    }

    includedirs
    {
        "../../Sources/%{prj.name}/",
        "../../Sources/NexusFramework/",

        "../../Libraries/googletest-1.14.0/include/"
    }

    links
    {
        "NexusFramework",
        "GoogleTest"
    }

    postbuildcommands
    {
        "../../Scripts/Build/PostBuild.bat %{cfg.buildtarget.directory}"
    }


project "GoogleTest"
    location "../../Libraries/googletest-1.14.0/"

    kind "StaticLib"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../builds/binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../builds/intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    disablewarnings { "26495", "26439" }

    files
    {
        "../../Libraries/googletest-1.14.0/**.h",
        "../../Libraries/googletest-1.14.0/**.cc"
    }

    removefiles
    {
        "../../Libraries/googletest-1.14.0/src/gtest-all.cc"
    }

    includedirs
    {
        "../../Libraries/googletest-1.14.0/",
        "../../Libraries/googletest-1.14.0/include/"
    }

	postbuildcommands
    {
        "../../Scripts/Build/PostBuild.bat %{cfg.buildtarget.directory}"
    }
