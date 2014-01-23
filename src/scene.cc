#include "scene.hh"


Scene::~Scene()
{
}


void Scene::SetSceneStack( const SceneStackPtr& stack )
{
	sceneStack = stack;
}

