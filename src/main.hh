#pragma once
#ifndef _MAIN_HH_
#define _MAIN_HH_


#ifdef _WIN32
	#pragma comment(lib, "GLFW3.lib")
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#ifdef _DEBUG
		#pragma comment(lib, "freetype252_D.lib")
		#pragma comment(lib, "libpng16d.lib")
		#pragma comment(lib, "glew32sd.lib")
	#else
		#pragma comment(lib, "freetype252.lib")
		#pragma comment(lib, "libpng16.lib")
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
#include "character.hh"
#include "joystick.hh"

#include "scene.hh"
#include "testScene.hh"

#include "gui.hh"
#include "guiBar.hh"

#include "managers.hh"
#include "helpers.hh"

#include "input.hh"
#include "windowEvent.hh"


#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <memory>
#include <future>

#endif
