workspace "NexusEngine"
    location "../../"
    startproject "NexusSandbox"
    debugcommand "../../build/package/NexusSandbox.exe"
	debugdir "../../build/package/"

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

project "NexusEngine"
    location "../../Sources/%{prj.name}/"

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../build/binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../build/intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

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
        "../../Scripts/Build/PostBuild.bat %{cfg.buildtarget.directory}"
    }

project "NexusSandbox"
    location "../../Sources/%{prj.name}/"

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../build/binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../build/intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp"
    }

    includedirs
    {
        "../../Sources/%{prj.name}/",
        "../../Sources/NexusEngine/"
    }

    links
    {
        "NexusEngine",
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

	targetdir ("../../build/binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../build/intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

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

        "../../Libraries/googletest-1.14.0/include/"
    }

    links
    {
        "NexusEngine",
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
