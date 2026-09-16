Root = os.realpath(os.getcwd() .. "/../../"):gsub("\\", "/")

Name = "%{prj.name}"
OutputDirectory = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"
OutputName = "%{cfg.buildtarget.basename}%{cfg.buildtarget.extension}"

Framework = "NexusFramework"
Utility = "NexusUtility"
Sandbox = "NexusSandbox"
Tests = "NexusTests"
GTest = "Gtest"
Yaml = "Yaml-Cpp"

Builds = Root .. "builds/"
Configs = Root .. "Configs/"
Libraries = Root .. "Libraries/"
Scripts = Root .. "Scripts/"
Sources = Root .. "Sources/"

Artifacts = Builds .. "artifacts/"
Binaries = Builds .. "binaries/"
Intermediates = Builds .. "intermediates/"
Code = Sources .. Name .. "/"
Lib = Libraries .. Name .. "/"
Target = Binaries .. OutputDirectory .. "/"
Object = Intermediates .. OutputDirectory .. "/"

PostBuild = Scripts .. "Build/Steps/PostBuild.bat " .. Target .. " " .. OutputName

workspace (Framework)
    location (Root)

    platforms { "Win64" }
    configurations { "Debug", "Release", "Distrib" }

	startproject (Sandbox)
    debugcommand (Artifacts .. Sandbox .. ".exe")
	debugdir (Root)

	characterset "Unicode"
    flags { "MultiProcessorCompile" }
    staticruntime "off"

	filter "action:vs*"
        toolset "msc"

    filter "toolset:msc"
        defines { "NX_MSVC" }
    	disablewarnings { "4244", "4267", "4251" }

    filter "platforms:Win64"
        defines { "NX_WINDOWS" }
        architecture "x64"
		system "windows"

    filter "configurations:Debug"
        defines { "NX_DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NX_RELEASE" }
        symbols "On"
        optimize "On"

    filter "configurations:Distrib"
        defines { "NX_DISTRIB" }
        symbols "Off"
        optimize "On"

group "Libraries"
project (GTest)
project (Yaml)
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

	links
	{
		Yaml
	}

	defines
	{
		"NX_FRAMEWORK_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (Utility)
    location (Code)

    kind "Utility"

	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.natvis",
    }

-- ----------------------------------------------------------------------------------
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
        Sources,
		Libraries,
    }

    links
    {
        Framework
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
    }

    links
    {
        Framework,
		GTest,
		Yaml
    }

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (GTest)
    location (Lib)

    kind "StaticLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        Lib .. "**.h",
        Lib .. "**.cc"
    }

    removefiles
    {
        Lib .. "**/gtest-all.cc",
        Lib .. "**/gtest_main.cc"
    }

    includedirs
    {
        Libraries,
        Lib,
    }

	postbuildcommands
    {
        PostBuild
    }

	filter "toolset:msc"
		disablewarnings { "26439", "26495" }

project (Yaml)
    location (Lib)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        Lib .. "**.h",
        Lib .. "**.cpp"
    }

    includedirs
    {
        Libraries,
    }

	defines
	{
		"yaml_cpp_EXPORTS"
	}

	postbuildcommands
    {
        PostBuild
    }

	filter "toolset:msc"
		disablewarnings { "4267", "4251", "4275" }
