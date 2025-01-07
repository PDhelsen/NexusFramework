workspace "NexusEngine"
    location "../../"
    startproject "NexusApp"
    debugcommand "../../builds/NexusApp.exe"
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

group "Engine"
project "NexusEngine"
project "NexusEditor"
group ""
group "App"
project "NexusApp"
group ""
group "UnitTest"
project "NexusTest"
group ""
group "Libraries"
project "GoogleTest"
group ""

project "NexusEngine"
    location "../../Sources/%{prj.name}/"

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    pchheader "Core/NexusEnginePch.h"
	pchsource "../../Sources/%{prj.name}/Core/NexusEnginePch.cpp"

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp",
        "../../Config/NexusEngine.natvis",
    }

    includedirs
    {
        "../../Sources/%{prj.name}/"
    }

    defines
    {
        "NEXUS_ENGINE_DLL_BUILD"
    }

    postbuildcommands
    {
        "../../Scripts/PostBuildCommandsEngine.bat %{cfg.buildtarget.directory}"
    }

project "NexusEditor"
    location "../../Sources/%{prj.name}/"

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp"
    }

    defines
    {
        "NEXUS_EDITOR_DLL_BUILD"
    }

    includedirs
    {
        "../../Sources/%{prj.name}/",
        "../../Sources/NexusEngine/"
    }

    links
    {
        "NexusEngine"
    }

    postbuildcommands
    {
        "../../Scripts/PostBuildCommandsEngine.bat %{cfg.buildtarget.directory}"
    }

project "NexusApp"
    location "../../Sources/%{prj.name}/"

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp"
    }

    includedirs
    {
        "../../Sources/%{prj.name}/",
        "../../Sources/NexusEngine/",
        "../../Sources/NexusEditor/"
    }

    links
    {
        "NexusEngine",
        "NexusEditor"
    }

    postbuildcommands
    {
        "../../Scripts/PostBuildCommandsEngine.bat %{cfg.buildtarget.directory}"
    }

project "NexusTest"
    location "../../Sources/%{prj.name}/"

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

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
        "../../Sources/NexusEngine/",
        "../../Sources/NexusEditor/",
        "../../Sources/NexusApp/",

        "../../Libraries/googletest-1.14.0/include/"
    }

    links
    {
        "NexusEngine",
        "NexusEditor",
        "NexusApp",
        "GoogleTest"
    }

    postbuildcommands
    {
        "../../Scripts/PostBuildCommandsEngine.bat %{cfg.buildtarget.directory}"
    }


project "GoogleTest"
    location "../../Libraries/googletest-1.14.0/"

    kind "StaticLib"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../Libraries/googletest-1.14.0/binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../Libraries/googletest-1.14.0/intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

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
