project "Glad"
    kind "StaticLib"
    language "C"
	staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
    }

	includedirs
	{
		"include"
	}
    
    filter "system:windows"
        systemversion "latest"
 
     defines
	 {
	 	 "MC_PLATFORM_WINDOWS"
	 }
 
     filter "configurations:Debug"
        runtime "Debug"
        symbols "on"             -- Deixamos on os debug symbols
        
     filter "configurations:Release"
        runtime "Release"
        optimize "on"            -- Ripamos todas as configura��es de debug e otimizamos o projeto