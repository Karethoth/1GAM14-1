#ifndef _TESTSCENE_HH_
#define _TESTSCENE_HH_

#include "scene.hh"
#include "node.hh"
#include "camera.hh"
#include "character.hh"
#include "gui.hh"
#include "guiBar.hh"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class TestScene : public Scene
{
 public:
	TestScene();
	virtual ~TestScene();

	virtual void Create();
	virtual void Destroy();

	virtual void Tick( double deltaTime );

	virtual void HandleInputEvent( const InputEvent& event );


 protected:
	// Nodes and entities
	Node                       rootNode;
	std::shared_ptr<Node>      light;
	std::shared_ptr<Node>      world;
	std::shared_ptr<Node>      worldCenter;
	std::shared_ptr<Camera>    camera;
	std::shared_ptr<Character> player;

	// GUI stuff
	GUI gui;
	std::shared_ptr<GUIBar> bar;

	// Uniforms
	GLint viewUniform;
	GLint projUniform;
	GLint worldCenterUniform;
	GLint lightPositionUniform;

	bool upKey, downKey, rightKey, leftKey;
};


#endif

