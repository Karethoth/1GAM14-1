#include "testScene.hh"
#include "surface.hh"
#include "managers.hh"
#include "helpers.hh"
#include "joystick.hh"

#include <iostream>


extern std::unique_ptr<Joystick> joystick;
extern bool joystickYInverted;


struct WindowInfo
{
	int   width;
	int   height;
	float ratio;

	WindowInfo()
	{
		width  = 600;
		height = 400;
	}

	void UpdateRatio()
	{
		ratio = (float)width/(float)height;
	}
} windowInfo;



TestScene::TestScene()
{
	windowInfo.height = 400;
	windowInfo.width = 600;
	windowInfo.UpdateRatio();

	// Create the root node
	rootNode = Node( "RootNode" );

	// Create the player
	player = std::make_shared<Character>( "Player" );

	// Get the needed uniform locations
	auto shaderProgram = shaderManager.Get( "DefaultShader" );
	viewUniform          = shaderProgram->GetUniform( "V" );
	projUniform          = shaderProgram->GetUniform( "P" );
	worldCenterUniform   = shaderProgram->GetUniform( "worldCenter" );
	lightPositionUniform = shaderProgram->GetUniform( "lightPosition" );

	upKey = false;
	downKey = false;
	rightKey = false;
	leftKey = false;
}



TestScene::~TestScene()
{
}



void TestScene::Create()
{
	player->SetPosition( glm::vec3( 0.0, 0.0, 0.0 ) );
	player->SetCollisionBox( AABB(
		glm::vec3( -1.f, 0.f, -1.f ),
		glm::vec3(  0.f, 5.f, 1.f )
	) );


	// Create the world node
	world = std::make_shared<Node>( "World" );
	rootNode.AddChild( world );

	// Create the camera
	camera = std::make_shared<Camera>();
	camera->SetPosition( glm::vec3( 0.f, 10.f, 15.f ) );
	camera->SetTarget( glm::vec3( 0.0, 0.0, 0.0 ) );
	camera->SetRatio( windowInfo.ratio );
	camera->SetFOV( 45.f );


	// Generate Node that acts as the "center" of the world.
	// (vertices that are affected by it, rotate around it)
	worldCenter = std::make_shared<Node>( "WorldCenter" );
	worldCenter->SetPosition( glm::vec3( 0.0, -20.0, 0.0 ) );


	// Generate ground surface
	Surface groundSurface( 40, 40 );
	glm::vec2 corners[] =
	{
		glm::vec2( 2.0, 3.0 ),
		glm::vec2( 0.0, 3.0 ),
		glm::vec2( 2.0, 0.0 ),
		glm::vec2( 0.0, 0.0 )
	};
	groundSurface.SetTextureCorners( corners );
	auto groundSurfaceMesh = groundSurface.GenerateMesh( 10, 10 );

	// Give name to the mesh and generate
	// the OpenGL arrays and buffers
	groundSurfaceMesh->name = "GroundSurfaceMesh";
	groundSurfaceMesh->GenerateGLBuffers( "DefaultShader" );

	// Give the mesh to the mesh manager
	meshManager.Add( "GroundSurfaceMesh", groundSurfaceMesh );

	// Generate entity and set it to use the ground surface mesh
	// and the wanted shader
	auto ground = std::make_shared<Entity>( "GroundSurfaceEntity" );
	ground->SetMeshName( "GroundSurfaceMesh" );
	ground->SetTextureName( "pebbles" );
	ground->SetShaderName( "DefaultShader" );
	ground->SetPosition( glm::vec3( -20.0, 0.0, -20.0 ) );


	// Give the entity to the world node
	world->AddChild( ground );


	// Repeat process for a wall
	Surface wallSurface( 40, 10 );
	corners[0] = glm::vec2( 0.0, 0.0 );
	corners[1] = glm::vec2( 5.0, 0.0 );
	corners[2] = glm::vec2( 0.0, 1.0 );
	corners[3] = glm::vec2( 5.0, 1.0 );
	wallSurface.SetTextureCorners( corners );
	auto wallMesh = wallSurface.GenerateMesh( 10, 5 );
	wallMesh->name = "WallMesh";
	wallMesh->GenerateGLBuffers( "DefaultShader" );
	meshManager.Add( "WallMesh", wallMesh );
	auto wall = std::make_shared<Entity>( "WallEntity" );
	wall->SetMeshName( "WallMesh" );
	wall->SetShaderName( "DefaultShader" );
	wall->SetTextureName( "bricks" );
	wall->SetPosition( glm::vec3( 20.0, 0.0, -20.0 ) );
	wall->SetOrientation( glm::quat( glm::vec3(
		ToRadians( -90.f ),
		ToRadians( 180.f ),
		0.0 )
	) );
	wall->SetCollisionBox( AABB(
		glm::vec3( -40.f, 0.f, -0.5f ),
		glm::vec3( 0.f, 10.f, 0.0f )
	) );
	world->AddChild( wall );


	// And another wall entity!
	auto secondWall = std::make_shared<Entity>( "WallEntity2" );
	secondWall->SetMeshName( "WallMesh" );
	secondWall->SetTextureName( "bricks" );
	secondWall->SetShaderName( "DefaultShader" );
	secondWall->SetPosition( glm::vec3( -20.0, 0.0, -20.0 ) );
	secondWall->SetOrientation( glm::quat( glm::vec3(
		ToRadians( -90.f ),
		ToRadians( -90.f ),
		0.0 )
	) );
	secondWall->SetCollisionBox( AABB(
		glm::vec3( -0.5f, 0.f, 0.f ),
		glm::vec3( 0.0f, 10.f, 40.f )
	) );
	world->AddChild( secondWall );


	// Create a tree to test textures with alpha layer
	Surface squareSurface( 5, 5 );
	auto squareMesh = squareSurface.GenerateMesh( 2, 2 );
	squareMesh->name = "SquareMesh";
	squareMesh->GenerateGLBuffers( "DefaultShader" );
	meshManager.Add( "SquareMesh", squareMesh );
	auto tree = std::make_shared<Entity>( "Tree" );
	tree->SetMeshName( "SquareMesh" );
	tree->SetTextureName( "tree" );
	tree->SetShaderName( "DefaultShader" );
	tree->SetPosition( glm::vec3( -15.0, 4.9, -10.0 ) );
	tree->SetOrientation( glm::quat( glm::vec3(
		ToRadians( 90.f ),
		ToRadians( 0.f ),
		0.0 )
	) );
	tree->SetCollisionBox( AABB(
		glm::vec3( 0.f, -4.9f, -1.f ),
		glm::vec3( 4.f, 0.f, 1.f )
	) );
	world->AddChild( tree );

	world->AddChild( tree );


	// Generate the head mesh
	Surface headSurface( 2, 2 );
	auto headMesh = headSurface.GenerateMesh( 1, 1 );
	headMesh->name = "HeadMesh";
	headMesh->GenerateGLBuffers( "DefaultShader" );
	meshManager.Add( "HeadMesh", headMesh );


	// Generate the torso mesh
	Surface torsoSurface( 2, 2 );
	auto torsoMesh = torsoSurface.GenerateMesh( 2, 2 );
	torsoMesh->name = "MediumTorsoMesh";
	torsoMesh->GenerateGLBuffers( "DefaultShader" );
	meshManager.Add( "MediumTorsoMesh", torsoMesh );

	// Create a Node to hold position of the light
	light = std::make_shared<Node>( "TestLight" );
	light->SetPosition( glm::vec3( -10.0, 8.0, -10.0 ) );
	world->AddChild( light );


	// Create the GUI elements.
	gui.SetSize( static_cast<float>( windowInfo.width ),
	             static_cast<float>( windowInfo.height ) );

	bar = std::make_shared<GUIBar>();
	bar->SetSize( 200.f, 20.f );
	bar->SetPosition( 0.f, 5.f );
	gui.AddChild( static_cast<std::shared_ptr<GUIElement>>( bar ) );


	// Generate list of objects player can bump into
	auto collidableObjects = std::make_shared<PhysicalList>();
	collidableObjects->push_back( tree );
	collidableObjects->push_back( wall );
	collidableObjects->push_back( secondWall );
	player->collidableObjects = collidableObjects;

	rootNode.AddChild( camera );
	player->AddChild( worldCenter );
	world->AddChild( player );
}


void TestScene::Destroy()
{
}



void TestScene::Tick( double deltaTime )
{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( shaderManager.Get( "DefaultShader" )->Get() );

		// Move the character, for now I'll let this snippet be here
		glm::vec3 move;
		glm::vec3 newVelocity;

		// Calculate the slowdown from previous velocity
		glm::vec3 slowDown = player->GetVelocity() * ((float)deltaTime*10);

		if( upKey )
			move += glm::vec3( 0.0, 0.0, -2.0/2.5 );
		if( downKey )
			move += glm::vec3( 0.0, 0.0, 2.0/2.5 );
		if( leftKey )
			move += glm::vec3( -1, 0.0, 0.0 );
		if( rightKey )
			move += glm::vec3( 1, 0.0, 0.0 );

		// Bool which we use to check if we have any input from joystick
		bool joystickInput = false;

		// 2nd joystick stick
		float xLook = 0.f;
		float yLook = 0.f;

		// If we have a joystick
		if( joystick )
		{
			// Update joystick and get the axes that count
			joystick->Update();
			float xAxis = joystick->GetAxis( 0 );
			float yAxis = joystick->GetAxis( 1 );

			if( joystickYInverted )
			{
				yAxis *= -1.f;
			}

			float triggerAxis = joystick->GetAxis( 2 );
			triggerAxis = triggerAxis*-1.f + 1.f;
			GUIMeasure w( triggerAxis * 50.f, PERCENTAGE );
			bar->SetSize(
				w,
				20.f
			);

			xLook =  joystick->GetAxis( 4 );
			yLook = -joystick->GetAxis( 3 );

			// Cap the input to zero if it's within the threshold
			const float threshold = 0.2f;
			if( xAxis < threshold && xAxis > -threshold )
				xAxis = 0.f;
			if( yAxis < threshold && yAxis > -threshold )
				yAxis = 0.f;

			// If any of the values is outside of the threshold(!0),
			// we got real input from joystick. For now this only is
			// affected by movement. "Looking" doesn't care about it
			if( xAxis != 0.f || yAxis != 0.f )
			{
				joystickInput = true;
				move = glm::vec3( xAxis, 0.f, yAxis );
			}
		}

		// If we got movement, we'll normalize it and multiply
		// it by how hastely we want the character to speed up
		if( glm::length( move ) > 0.f )
		{
			// If we have joystick input we need to check that
			// the movement vector has length under 1.f.
			// This is because some joysticks have square movement
			// area, which corners could otherwise be used to move
			// faster than supposed. Same goes for the arrow keys.
			if( joystickInput && glm::length( move ) <= 1.f )
			{
				move *= 200.f * deltaTime;
			}
			else
			{
				move = glm::normalize( move ) * 200.f * (float)deltaTime;
			}
		}

		// Calculate the new velocity
		newVelocity = player->GetVelocity() + move - slowDown;

		// If the velocity is above the maximum speed,
		// we'll cap it to it
		if( glm::length( newVelocity ) > 15.f )
		{
			newVelocity = glm::normalize( newVelocity );
			newVelocity *= 15.f;
		}

		// Set the velocity
		player->SetVelocity( newVelocity );
		player->Update( deltaTime );

		// Update the node tree
		rootNode.UpdateWorldInfo();

		glm::vec3 cameraTarget = player->GetWorldPosition();
		cameraTarget.x += xLook * 5.f;
		cameraTarget.y += yLook * 5.f;

		camera->SetTarget( cameraTarget );
		camera->SetPosition( player->GetWorldPosition() + glm::vec3( 0.f, 10.f, 15.f ) );

		// Camera handling and matrix stuff
		camera->SetRatio( windowInfo.ratio );
		auto projectionMat = camera->GetProjectionMatrix();
		auto viewMat = camera->GetViewMatrix();

		// Upload uniforms to GPU
		glUniformMatrix4fv( viewUniform,  1, GL_FALSE, &(*viewMat)[0][0] );
		glUniformMatrix4fv( projUniform,  1, GL_FALSE, &(*projectionMat)[0][0] );
		glUniform3fv( worldCenterUniform, 1, &worldCenter->GetWorldPosition()[0] );
		glUniform3fv( lightPositionUniform, 1, &light->GetWorldPosition()[0] );

		// Render the current scene
		rootNode.Render();

		// Render the user interface
		gui.Render();
}



void TestScene::HandleInputEvent( const InputEvent& event )
{
	if( event.type == KEYBOARD_INPUT )
	{
		switch( event.index )
		{
		 case GLFW_KEY_UP:
			upKey = event.subType == KEY_DOWN;
			break;

		 case GLFW_KEY_DOWN:
			downKey = event.subType == KEY_DOWN;
			break;

		 case GLFW_KEY_RIGHT:
			rightKey = event.subType == KEY_DOWN;
			break;

		 case GLFW_KEY_LEFT:
			leftKey = event.subType == KEY_DOWN;
			break;
		}
	}
	else if( event.type == JOYSTICK_INPUT )
	{
		switch( event.subType )
		{
		 case JOYSTICK_CONNECTED:
			std::cout << "Joystick connected: " << joystick->GetName() << "\n";
			break;

		 case JOYSTICK_DISCONNECTED:
			std::cout << "Joystick disconnected: " << joystick->GetName() << "\n";
			break;
		}
	}

	gui.HandleInputEvent( event );
}



void TestScene::HandleWindowEvent( const WindowEvent& event )
{
}

