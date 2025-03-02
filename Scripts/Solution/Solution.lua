Root = os.realpath(os.getcwd() .. "/../../"):gsub("\\", "/")

Name = "%{prj.name}"
Output = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"

Framework = "NexusFramework"
Utility = "NexusUtility"
Sandbox = "NexusSandbox"
Tests = "NexusTests"
GoogleTest = "GoogleTest"

Builds = Root .. "builds/"
Configs = Root .. "Configs/"
Libraries = Root .. "Libraries/"
Scripts = Root .. "Scripts/"
Sources = Root .. "Sources/"

Artifacts = Builds .. "artifacts/"
Binaries = Builds .. "binaries/"
Intermediates = Builds .. "intermediates/"
Code = Sources .. Name .. "/"
External = Libraries .. Name .. "/"
Target = Binaries .. Output .. "/"
Object = Intermediates .. Output .. "/"

PostBuild = Scripts .. "Build/Steps/PostBuild.bat " .. Target

workspace (Framework)
    location (Root)

    platforms { "Win64" }
    configurations { "Debug", "Release", "Distrib" }

	startproject (Sandbox)
    debugcommand (Artifacts .. Sandbox .. ".exe")
	debugdir (Root)

	characterset "Unicode"
    flags { "MultiProcessorCompile" }

	filter "action:vs*"
        toolset "msc"

    filter "toolset:msc"
        defines { "NEXUS_MSVC" }

    filter "platforms:Win64"
        defines { "NEXUS_WINDOWS" }
        architecture "x64"
		system "windows"

    filter "configurations:Debug"
        defines { "NEXUS_DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NEXUS_RELEASE" }
        symbols "On"
        optimize "On"

    filter "configurations:Distrib"
        defines { "NEXUS_DISTRIB" }
        symbols "Off"
        optimize "On"

group "Libraries"
project (GoogleTest)
group "Tests"
project (Sandbox)
project (Tests)
group "Misc"
project (Utility)
group ""

project (Framework)
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    pchheader ("NexusFramework/Core/NexusFrameworkPch.h")
	pchsource (Code .. "Core/NexusFrameworkPch.cpp")

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
        Code .. "**.natvis",
    }

    includedirs
    {
		Sources,
		Libraries,
    }

	defines
	{
		"NEXUS_FRAMEWORK_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

project (Utility)
    location (Code)

    kind "Utility"

	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.natvis",
    }

project (Sandbox)
    location (Code)

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.h",
        Code .. "**.cpp"
    }

    includedirs
    {
        Sources
    }

    links
    {
        Framework,
    }

    postbuildcommands
    {
        PostBuild
    }

project (Tests)
    location (Code)

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
        Configs .. "UnitTest.runsettings"
    }

    includedirs
    {
        Sources,
		Libraries,

        Libraries .. GoogleTest .. "/include/"
    }

    links
    {
        Framework,
        GoogleTest
    }

    postbuildcommands
    {
        PostBuild
    }

project (GoogleTest)
    location (External)

    kind "StaticLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        External .. "**.h",
        External .. "**.cc"
    }

    removefiles
    {
        External .. "src/gtest-all.cc"
    }

    includedirs
    {
        External,
        External .. "include/",
    }

	postbuildcommands
    {
        PostBuild
    }
