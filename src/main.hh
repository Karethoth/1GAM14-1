#pragma once
#ifndef _MAIN_HH_
#define _MAIN_HH_


#ifdef _WIN32
	#pragma comment(lib, "GLFW3.lib")
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#ifdef _DEBUG
		#pragma comment(lib, "glew32sd.lib")
	#else
		#pragma comment(lib, "glew32s.lib")
	#endif

	#include <Windows.h>
#endif

#define GLEW_STATIC
#include <GL/glew.h>


#include "shaderProgram.hh"
#include "camera.hh"
#include "mesh.hh"
#include "surface.hh"
#include "entity.hh"

#include "managers.hh"


#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>


#endif
