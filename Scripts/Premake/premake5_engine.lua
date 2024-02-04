workspace "NexusEngine"
    location "../../"
    startproject "NexusApp"
    debugcommand "../../builds/NexusApp.exe"

    configurations { "Debug", "Release", "Ditrib" }
    platforms { "Win64" }

    filter "platforms:Win64"
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

project "NexusEngine"
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