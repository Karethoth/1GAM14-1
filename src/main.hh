#pragma once
#ifndef _MAIN_HH_
#define _MAIN_HH_


#ifdef _WIN32
	#pragma comment(lib, "GLFW3.lib")
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#pragma comment(lib, "glew32s.lib")

	#include <Windows.h>
#endif

#define GLEW_STATIC
#include <GL/glew.h>

#include "shaderProgram.hh"
#include "node.hh"
#include "mesh.hh"
#include "surface.hh"

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#endif
