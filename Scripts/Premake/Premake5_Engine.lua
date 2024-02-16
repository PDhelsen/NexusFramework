workspace "NexusEngine"
    location "../../"
    startproject "NexusApp"
    debugcommand "../../builds/NexusApp.exe"

    configurations { "Debug", "Release", "Ditrib" }
    platforms { "Win64" }
    flags { "MultiProcessorCompile" }

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

group "Engine"
project "NexusEngine"
project "NexusEditor"
group ""
group "App"
project "NexusApp"
group ""
group "Test"
project "NexusTest"
group ""

project "NexusEngine"
    location "../../Sources/%{prj.name}/"
    
    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir ("../../binaries/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")
	objdir ("../../intermediates/%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}/")

    pchheader "Core/NexusPch.h"
	pchsource "../../Sources/%{prj.name}/Core/NexusPch.cpp"

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp"
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

    files
    {
        "../../Sources/%{prj.name}/**.h",
        "../../Sources/%{prj.name}/**.cpp",

        "../../Libraries/googletest-1.14.0/**.h",
        "../../Libraries/googletest-1.14.0/**.cc"
    }

    removefiles 
    { 
        "../../Libraries/googletest-1.14.0/src/gtest-all.cc" 
    }

    includedirs
    {
        "../../Sources/%{prj.name}/",
        "../../Sources/NexusEngine/",
        "../../Sources/NexusEditor/",
        "../../Sources/NexusApp/",

        "../../Libraries/googletest-1.14.0/",
        "../../Libraries/googletest-1.14.0/include/"
    }

    links
    {
        "NexusEngine",
        "NexusEditor",
        "NexusApp"
    }

    postbuildcommands 
    {
        "../../Scripts/PostBuildCommandsEngine.bat %{cfg.buildtarget.directory}"
    }
    