#ifndef _SCENE_HH_
#define _SCENE_HH_

#include "templateStack.hh"
#include <memory>


typedef TemplateStack<std::shared_ptr<Scene>> SceneStack;
typedef std::shared_ptr<SceneStack>           SceneStackPtr;


class Scene
{
 public:
	virtual ~Scene();

	virtual void CreateScene() = 0;
	virtual void DestroyScene() = 0;

	virtual void Run() = 0;
	virtual void Stop() = 0;
	virtual void Tick( double deltaTime ) = 0;

	virtual void SetSceneStack( const SceneStackPtr& stack );


 protected:
	SceneStackPtr sceneStack;
};


#endif

