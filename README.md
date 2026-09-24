# Nexus Framework

Nexus Framework is a small C++ Framework made on my spare time to learn and improve my knowledge of low-level concept.
It will serve as the base for the others Nexus Project

# Supported Platform, Compiler,  Requirements

Nexus Framework is supported only Windows and MSCV compiler (it uses a few MSCV instrics functions).
The project generates the visual studio files with Premake.
Both Premake and Visual Studio build tools are expected to be available through environment variables in order to run the Scripts.

# How to build ?

Run the Scripts/Solution/Solution.bat to generate the Visual Studio files, then run the Scripts/Build/Command/Build* to build the solution.
Finally in order to use the framework dll for another project (i.e. NexusEngine), run the Scripts/Deploy <path>. This last step will copy the binaries and the includes to the provided path and ask the user to store that path into the environment variable, that will allows the other projects to find the corresponding dlls and includes.
