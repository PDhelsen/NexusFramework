workspace "NexusSandbox"
    location "../../"
    startproject "NexusSandbox"
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

project "NexusSandbox"
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
        "../../../../Softwares/NexusEngine/Install/Sources/NexusEngine/",
        "../../../../Softwares/NexusEngine/Install/Sources/NexusEditor/"
    }

    postbuildcommands 
    {
        "../../Scripts/PostBuildCommandsProject.bat %{cfg.buildtarget.directory} %{cfg.buildtarget.name}"
    }