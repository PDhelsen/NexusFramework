Root = "../../"

Builds = Root .. "builds/"
Configs = Root .. "Configs/"
Libraries = Root .. "Libraries/"
Scripts = Root .. "Scripts/"
Sources = Root .. "Sources/"

Artifacts = Builds .. "artifacts/"
Binaries = Builds .. "binaries/"
Intermediates = Builds .. "intermediates/"

GoogleTest = Libraries .. "googletest-1.14.0/"

ProjectToken = "%{prj.name}"
OutputToken = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"

SourceCodeToken = Sources .. ProjectToken .. "/"
TargetToken = Binaries .. OutputToken .. "/"
ObjectToken = Intermediates .. OutputToken .. "/"

PostBuildCommandToken = Scripts .. "Build/PostBuild.bat " .. TargetToken

workspace "NexusFramework"
    location (Root)
    startproject "NexusSandbox"
    debugcommand (Artifacts .. "NexusSandbox.exe")
	debugdir (Root)

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
    location (SourceCodeToken)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (TargetToken)
	objdir (ObjectToken)

    pchheader ("Core/NexusFrameworkPch.h")
	pchsource (SourceCodeToken .. "Core/NexusFrameworkPch.cpp")

    files
    {
        SourceCodeToken .. "**.h",
        SourceCodeToken .. "**.cpp",
        SourceCodeToken .. "**.natvis",
    }

    includedirs
    {
        SourceCodeToken
    }

    defines
    {
        "NEXUS_FRAMEWORK_DLL_BUILD"
    }

    postbuildcommands
    {
        PostBuildCommandToken
    }

project "NexusSandbox"
    location (SourceCodeToken)

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir (TargetToken)
	objdir (ObjectToken)

    files
    {
        SourceCodeToken .. "**.h",
        SourceCodeToken .. "**.cpp"
    }

    includedirs
    {
        SourceCodeToken,
        Sources .. "NexusFramework/"
    }

    links
    {
        "NexusFramework",
    }

    postbuildcommands
    {
        PostBuildCommandToken
    }

project "NexusTest"
    location (SourceCodeToken)

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir (TargetToken)
	objdir (ObjectToken)

    pchheader "Core/NexusTestPch.h"
	pchsource (SourceCodeToken .. "Core/NexusTestPch.cpp")

    files
    {
        SourceCodeToken .. "**.h",
        SourceCodeToken .. "**.cpp"
    }

    includedirs
    {
        SourceCodeToken,
        Sources .. "NexusFramework/",

        GoogleTest .. "include/"
    }

    links
    {
        "NexusFramework",
        "GoogleTest"
    }

    postbuildcommands
    {
        PostBuildCommandToken
    }


project "GoogleTest"
    location (GoogleTest)

    kind "StaticLib"
    language "C++"
	cppdialect "C++20"

	targetdir (TargetToken)
	objdir (ObjectToken)

    disablewarnings { "26495", "26439" }

    files
    {
        GoogleTest .. "**.h",
        GoogleTest .. "**.cc"
    }

    removefiles
    {
        GoogleTest .. "src/gtest-all.cc"
    }

    includedirs
    {
		GoogleTest,
        GoogleTest .. "include/"
    }

	postbuildcommands
    {
        PostBuildCommandToken
    }
