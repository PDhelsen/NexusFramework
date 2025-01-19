Name = "%{prj.name}"
Output = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"

Root = os.realpath(os.getcwd() .. "/../../"):gsub("\\", "/")

Framework = "NexusFramework"
Sandbox = "NexusSandbox"
Tests = "NexusTest"
GoogleTest = "googletest-1.14.0"

Builds = Root .. "builds/"
Configs = Root .. "Configs/"
Libraries = Root .. "Libraries/"
Saved = Root .. "saved/"
Scripts = Root .. "Scripts/"
Sources = Root .. "Sources/"

Artifacts = Builds .. "artifacts/"
Binaries = Builds .. "binaries/"
Intermediates = Builds .. "intermediates/"
Code = Sources .. Name .. "/"
External = Libraries .. Name .. "/"
Target = Binaries .. Output .. "/"
Object = Intermediates .. Output .. "/"

PostBuild = Scripts .. "Build/PostBuild.bat " .. Target

workspace (Framework)
    location (Root)

    platforms { "Win64" }
    configurations { "Debug", "Release", "Ditrib" }

	startproject "NexusSandbox"
    debugcommand (Artifacts .. "NexusSandbox.exe")
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

    filter "configurations:Ditrib"
        defines { "NEXUS_DISTRIB" }
        symbols "Off"
        optimize "On"

group "Libraries"
project (GoogleTest)
group "Tests"
project (Sandbox)
project (Tests)
group ""

project (Framework)
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    pchheader ("Core/NexusFrameworkPch.h")
	pchsource (Code .. "Core/NexusFrameworkPch.cpp")

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
        Code .. "**.natvis",
    }

    includedirs
    {
        Code
    }

    defines
    {
        "NEXUS_FRAMEWORK_DLL_BUILD"
    }

    postbuildcommands
    {
        PostBuild
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
        Code,
        Sources .. Framework .. "/"
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

    pchheader "Core/NexusTestPch.h"
	pchsource (Code .. "Core/NexusTestPch.cpp")

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
        Configs .. "UnitTest.runsettings"
    }

    includedirs
    {
        Code,
        Sources .. Framework .. "/",
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

    disablewarnings { "26495", "26439" }

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
        External .. "include/"
    }

	postbuildcommands
    {
        PostBuild
    }
