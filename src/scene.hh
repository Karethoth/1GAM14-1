#ifndef _SCENE_HH_
#define _SCENE_HH_

#include "templateStack.hh"
#include "input.hh"
#include <memory>


class Scene;

typedef TemplateStack<Scene>        SceneStack;
typedef std::shared_ptr<SceneStack> SceneStackPtr;


class Scene
{
 public:
	virtual ~Scene();

	virtual void Create() = 0;
	virtual void Destroy() = 0;

	virtual void Tick( double deltaTime ) = 0;

	virtual void HandleInputEvent( const InputEvent& event ) = 0;

	virtual void SetSceneStack( const SceneStackPtr& stack );


 protected:
	SceneStackPtr sceneStack;
};


#endif

