workspace "PPMA Viewer"
    architecture "x64"
    configurations {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "PPMA Viewer"
    kind "WindowedApp"
    language "C++"
    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp"
    }

    includedirs {
        "include/",
        "src/"
    }

    libdirs {
        "lib/%{cfg.architecture}/"
    }

    links {
        "SDL2",
        "SDL2main",
    }

    postbuildcommands {
        "{COPY} lib/%{cfg.architecture}/SDL2.dll bin/"..outputdir,
    }

--    filter { "system:windows" }
--      systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
  
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
