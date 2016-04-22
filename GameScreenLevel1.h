#ifndef _GAMESCREENLEVEL1_H
#define _GAMESCREENLEVEL1_H

#include <SDL.h>
#include "GameScreen.h"
#include "Terrain.h"
#include "Object3ds.h"
#include "Texture.h"
#include <string>
#include <sstream>

class GameScreenLevel1 : GameScreen
{

public:
	GameScreenLevel1();
	~GameScreenLevel1();

	bool		SetUpLevel();
	void		Render();
	void		Update(float deltaTime, SDL_Event e);

	void OutputLine(float x, float y, string text);

	void RenderText();

	void DrawSkybox();

	void DrawPlanetsWithRotation();
	void DrawPlanetsWithoutRotation();

private:	
	float   mCurrentTime;
	
	float	orbitAngle;
	float	orbitVelocity;
	float	moonOrbitVelocity;

	Terrain mTerrain;

	Texture* skyboxTexture[6];

	material sunEmmisiveMaterial;

	Object3DS* sun;
	Object3DS* m_p3DSModel_2;
	Object3DS* planet[7];

	string text;

	bool pressed0, pressed1, pressed2, pressed3, pressed4, pressed5, pressed6, pressed7, pressed8, pressedI, instructions;
};


#endif //_GAMESCREENLEVEL1_H