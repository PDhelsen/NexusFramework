Framework = "NexusFramework"
Utility = "NexusUtility"
Sandbox = "NexusSandbox"
Tests = "NexusTests"
GTest = "Gtest"
YamlCpp = "Yaml-Cpp"

ProjectName = "%{prj.name}"
OutputName = "%{cfg.buildtarget.basename}%{cfg.buildtarget.extension}"
OutputDirectory = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"
Language = "C++"
LanguageVersion = "C++20"
Systems = 
{
    Windows = "windows"
}
Platforms = 
{
    Win64 = "Win64"
}
Configurations = 
{
    Debug  = "Debug",
    Release = "Release",
    Distrib = "Distrib"
}
Compilers = 
{
    Msvc = "msc"
}
Tools = 
{
    VisualStudio = "vs*"
}
Warnings = 
{
    "4244", "4251", "4267", "4275"
}

Root = os.realpath(os.getcwd() .. "/../../"):gsub("\\", "/")

Builds = Root .. "builds/"
Configs = Root .. "Configs/"
Libraries = Root .. "Libraries/"
Scripts = Root .. "Scripts/"
Sources = Root .. "Sources/"

Artifacts = Builds .. "artifacts/"
Binaries = Builds .. "binaries/"
Intermediates = Builds .. "intermediates/"
FirstParty = Sources .. ProjectName .. "/"
ThirdParty = Libraries .. ProjectName .. "/"
Target = Binaries .. OutputDirectory .. "/"
Object = Intermediates .. OutputDirectory .. "/"

Includes = 
{
    Sources,
    Libraries
}

LibrariesIncludes = 
{

}

Links = 
{
    YamlCpp,
    GTest
}

Defines = 
{
    "GTEST_LINKED_AS_SHARED_LIBRARY"
}

PostBuild = Scripts .. "Build/Steps/PostBuild.bat " .. OutputDirectory .. " " .. OutputName

-- ----------------------------------------------------------------------------------
workspace (Framework)
    location (Root)

    platforms { Platforms.Win64 }
    configurations { Configurations.Debug, Configurations.Release, Configurations.Distrib }

	startproject (Sandbox)
    debugcommand (Artifacts .. Sandbox .. ".exe")
	debugdir (Root)

	characterset "Unicode"
    flags { "MultiProcessorCompile" }
    staticruntime "off"

	filter ("action:" .. Tools.VisualStudio)
        toolset (Compilers.Msvc)

    filter ("toolset:" .. Compilers.Msvc)
        defines { "NX_MSVC", "_CRT_SECURE_NO_WARNINGS" }
    	disablewarnings (Warnings)

    filter ("platforms:" .. Platforms.Win64)
        defines { "NX_WINDOWS" }
		system (Systems.Windows)
        architecture "x64"

    filter ("configurations:" .. Configurations.Debug)
        defines { "NX_DEBUG" }
        symbols "On"
        optimize "Off"

    filter ("configurations:" .. Configurations.Release)
        defines { "NX_RELEASE" }
        symbols "On"
        optimize "On"

    filter ("configurations:" .. Configurations.Distrib)
        defines { "NX_DISTRIB" }
        symbols "Off"
        optimize "On"

    filter ""

group "Libraries"
project (GTest)
project (YamlCpp)
group "Tests"
project (Sandbox)
project (Tests)
group "Misc"
project (Utility)
group ""

-- ----------------------------------------------------------------------------------
project (Framework)
    location (FirstParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

    pchheader ("NexusFramework/Core/NexusFrameworkPch.h")
	pchsource (FirstParty .. "Core/NexusFrameworkPch.cpp")

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp"
    }

    includedirs
    {
		Includes
    }

	links
	{
		Links
	}

	defines
	{
        Defines,
		"NX_FRAMEWORK_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (Utility)
    location (FirstParty)

    kind "Utility"

	targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.natvis",
    }

-- ----------------------------------------------------------------------------------
project (Sandbox)
    location (FirstParty)

    kind "ConsoleApp"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp"
    }

    includedirs
    {
		Includes
    }

	links
	{
        Links,
        Framework
	}

    defines
	{
		Defines
	}

    postbuildcommands
    {
        PostBuild
    }

project (Tests)
    location (FirstParty)

    kind "ConsoleApp"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp"
    }

    includedirs
    {
		Includes
    }

	links
	{
        Links,
        Framework
	}

    defines
	{
		Defines
	}

    postbuildcommands
    {
        PostBuild
    }

    filter ("action:" .. Tools.VisualStudio)
        vsprops 
        {
            RunSettingsFilePath = Configs .. "UnitTest.runsettings"
        }

    filter ""

-- ----------------------------------------------------------------------------------
project (GTest)
    location (ThirdParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

    files
    {
        ThirdParty .. "**.h",
        ThirdParty .. "**.cc"
    }

    removefiles
    {
        ThirdParty .. "**/gtest-all.cc",
        ThirdParty .. "**/gtest_main.cc"
    }

    includedirs
    {
        Libraries,
        ThirdParty
    }

    defines
	{
		"GTEST_CREATE_SHARED_LIBRARY"
	}

	postbuildcommands
    {
        PostBuild
    }

project (YamlCpp)
    location (ThirdParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

    files
    {
        ThirdParty .. "**.h",
        ThirdParty .. "**.cpp"
    }

    includedirs
    {
        Libraries
    }

	defines
	{
		"yaml_cpp_EXPORTS"
	}

	postbuildcommands
    {
        PostBuild
    }
