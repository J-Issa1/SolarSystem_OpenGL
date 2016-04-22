#include "GameScreen.h"
#include "./gl/glut.h"
#include "Camera.h"
#include "Constants.h"

//--------------------------------------------------------------------------------------------------

GameScreen::GameScreen()
{
	aAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	aNear = 0.1f;
	aFar = 1000.0f;
	aFieldOfView = 60.0f;

	aBottom = 10.0f;
	aTop = 100.0f;
	aLeft = 10.0f;
	aRight = 100.0f;

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	gluPerspective(aFieldOfView, aAspect, aNear, aFar);
	
	//Set up the camera.
	mCamera = Camera::GetInstance();
	mCamera->SetAsPerspective(Vector3D(0.0f, 0.0f, -380.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
								aNear, aFar, aFieldOfView, aAspect);


	//OpenGL set up.
	glEnable(GL_CULL_FACE);								//Stop calculation of inside faces
	glEnable(GL_DEPTH_TEST);							//Hidden surface removal
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);								// Enable Lighting
	glShadeModel(GL_SMOOTH);

	//clear background colour.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

GameScreen::~GameScreen()
{
}

void GameScreen::Render()
{
	//Clear the screen.
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	mCamera->Render();
}

void GameScreen::Update(float deltaTime, SDL_Event e)
{
	mCamera->Update(deltaTime, e);

	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
			case SDLK_RETURN:
				mCamera->SetAsPerspective(Vector3D(0.0f, 0.0f, -380.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_0:
				mCamera->SetAsPerspective(Vector3D(0.0f, 5.0f, -90.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_1:
				mCamera->SetAsPerspective(Vector3D(-50.0f, 0.0f, -10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;
			
			case SDLK_2:
				mCamera->SetAsPerspective(Vector3D(-70.0f, 0.0f, -20.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_3:
				mCamera->SetAsPerspective(Vector3D(-100.0f, 0.0f, -20.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_4:
				mCamera->SetAsPerspective(Vector3D(-120.0f, 0.0f, -20.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_5:
				mCamera->SetAsPerspective(Vector3D(-150.0f, 0.0f, -60.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_6:
				mCamera->SetAsPerspective(Vector3D(-210.0f, 0.0f, -50.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_7 :
				mCamera->SetAsPerspective(Vector3D(-260.0f, 0.0f, -40.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_8:
				mCamera->SetAsPerspective(Vector3D(-310.0f, 0.0f, -35.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;

			case SDLK_SPACE:
				mCamera->SetAsPerspective(Vector3D(0.0f, -100.0f, -380.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f),
					aNear, aFar, aFieldOfView, aAspect);
				break;
		}
		break;
	}
}