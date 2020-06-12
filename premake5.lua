-- DIRECTORIES
BUILD_DIR = "build"
OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.platform}"
OBJECT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.platform}"

-- DELETE OBJECT AND BINARY FILES
if _ACTION == "clean" then
    os.rmdir(BUILD_DIR .. "/bin")
    os.rmdir(BUILD_DIR .. "/obj")
end

-- WORKSPACE SETUP
workspace "RBLibs"
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }
    
    defaultplatform "x86"
    startproject "Main"
    location "build"

    filter "system:windows"
        systemversion "latest"

    filter "platforms:x86"
        architecture "x86"
        
    filter "platforms:x64"
        architecture "x86_64"
        

    project "Libs"
        kind "StaticLib"
        location (BUILD_DIR .. "/%{prj.name}")
        language "C++"
        cppdialect "C++17"

        vectorextensions "AVX2"

        pchheader "pch.h"
        pchsource (BUILD_DIR .. "/%{prj.name}/src/pch.cpp")

        forceincludes { "pch.h" }

        files { BUILD_DIR .. "/%{prj.name}/src/**.h", BUILD_DIR .. "/%{prj.name}/src/**.cpp" }

        targetdir (BUILD_DIR .. "/bin/" .. OUTPUT_DIR .. "/%{prj.name}/")
        objdir (BUILD_DIR .. "/obj/" .. OBJECT_DIR .. "/%{prj.name}/")

        includedirs { BUILD_DIR .. "/%{prj.name}/src/" }

        filter "configurations:Debug"
            defines { "_DEBUG" }
            symbols "On"
            runtime "Debug"
            optimize "Debug"

        filter "configurations:Release"
            symbols "Off"
            runtime "Release"
            optimize "On"


    project "Tests"
        kind "SharedLib"
        location (BUILD_DIR .. "/%{prj.name}")
        language "C++"
        cppdialect "C++17"

        files { BUILD_DIR .. "/%{prj.name}/src/**.h", BUILD_DIR .. "/%{prj.name}/src/**.cpp" }

        targetdir (BUILD_DIR .. "/bin/" .. OUTPUT_DIR .. "/%{prj.name}/")
        objdir (BUILD_DIR .. "/obj/" .. OBJECT_DIR .. "/%{prj.name}/")

        includedirs { BUILD_DIR .. "/Libs/src/", BUILD_DIR .. "/%{prj.name}/src/" }

        links { "Libs" }

        filter "configurations:Debug"
            defines { "_DEBUG" }
            symbols "On"
            runtime "Debug"
            optimize "Debug"

        filter "configurations:Release"
            symbols "Off"
            runtime "Release"
            optimize "On"

            
    project "Main"
        kind "ConsoleApp"
        location (BUILD_DIR .. "/%{prj.name}")
        language "C++"
        cppdialect "C++17"

        files { BUILD_DIR .. "/%{prj.name}/src/**.h", BUILD_DIR .. "/%{prj.name}/src/**.cpp" }

        targetdir (BUILD_DIR .. "/bin/" .. OUTPUT_DIR .. "/%{prj.name}/")
        objdir (BUILD_DIR .. "/obj/" .. OBJECT_DIR .. "/%{prj.name}/")

        includedirs { BUILD_DIR .. "/Libs/src/", BUILD_DIR .. "/%{prj.name}/src/" }

        links { "Libs" }

        filter "configurations:Debug"
            defines { "_DEBUG" }
            symbols "On"
            runtime "Debug"
            optimize "Debug"

        filter "configurations:Release"
            symbols "Off"
            runtime "Release"
            optimize "On"
