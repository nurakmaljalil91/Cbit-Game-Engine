workspace "Cbit"
    architecture "x64"

    configurations 
    {
        "Debug",
        "Relesae",
        "Dist"
    }
-- outpurdir = "%{cfg.buildcfg}-%{cfg.system}-%{}"
project "Cbit"
    location "src"
    kind "SharedLib"
    language "C++"

    files
    {
        "src/**.h"
        "src/**.cpp"
    }

    include
    {
        "vendors/"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    targerdir("bin")