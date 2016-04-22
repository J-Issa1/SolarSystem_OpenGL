#include <time.h>
#include <windows.h>
#include <GL\gl.h>
#include <vector>
#include <string>
#include <sstream>
#include "GameScreenLevel1.h"
#include "Constants.h"
#include "Commons.h"

using namespace std;


GameScreenLevel1::GameScreenLevel1() : GameScreen()
{
	mTerrain.Initialise();

	// enable lighting
	glEnable(GL_LIGHT0);

	orbitAngle = 0.5f;
	orbitVelocity = 1.0f;
	moonOrbitVelocity = 0.5f;

	pressed0 = false;
	pressed1 = false;
	pressed2 = false;
	pressed3 = false;
	pressed4 = false;
	pressed5 = false;
	pressed6 = false;
	pressed7 = false;
	pressed8 = false;
	pressedI = false;
	instructions = true;

	sunEmmisiveMaterial = {
							{ 1.0f, 1.0f, 1.0f, 1.0f },
							{ 0.4f, 0.4f, 0.4f, 1.0f },
							{ 1.0f, 1.0f, 1.0f, 1.0f },
							0.0f,
							{ 0.4f, 0.2f, 0.0f }
					   };

	// Skybox Texture 
	Texture* skyFront = new Texture();
	Texture* skyBack  = new Texture();
	Texture* skyLeft  = new Texture();
	Texture* skyRight = new Texture();
	Texture* skyUp	  = new Texture();
	Texture* skyDown  = new Texture();
	skyboxTexture[0]  = skyFront;
	skyboxTexture[1]  = skyBack;
	skyboxTexture[2]  = skyLeft;
	skyboxTexture[3]  = skyRight;
	skyboxTexture[4]  = skyUp;
	skyboxTexture[5]  = skyDown;

	skyFront->Load("sky-front.raw", 512, 512);
	skyBack->Load("sky-back.raw", 512, 512);
	skyLeft->Load("sky-left.raw", 512, 512);
	skyRight->Load("sky-right.raw", 512, 512);
	skyUp->Load("sky-up.raw", 512, 512);
	skyDown->Load("sky-down.raw", 512, 512);

	Texture* sunTexture = new Texture();
	Texture* mercuryTexture = new Texture();
	Texture* venusTexture = new Texture();
	Texture* earthTexture = new Texture();
	Texture* moonTexture = new Texture();
	Texture* marsTexture = new Texture();
	Texture* jupiterTexture = new Texture();
	Texture* saturnTexture = new Texture();
	Texture* uranusTexture = new Texture();
	Texture* neptuneTexture = new Texture();

	// Sun
	sunTexture->Load("sun.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, sunTexture->GetID());
	sun = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", sunTexture->GetID());

	// Planet - Mercury
	mercuryTexture->Load("texture-mercury.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, mercuryTexture->GetID());
	planet[0] = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", mercuryTexture->GetID());

	// Planet - Venus
	venusTexture->Load("texture-venus.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, venusTexture->GetID());
	planet[1] = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", venusTexture->GetID());

	// Planet - Earth
	earthTexture->Load("texture-earth.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, earthTexture->GetID());
	planet[2] = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", earthTexture->GetID());

	// Planet - Mars
	marsTexture->Load("texture-mars.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, marsTexture->GetID());
	planet[3] = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", marsTexture->GetID());

	// Planet - Jupiter
	jupiterTexture->Load("texture-jupiter.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, jupiterTexture->GetID());
	planet[4] = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", jupiterTexture->GetID());

	// Planet - Saturn
	saturnTexture->Load("texture-saturn.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, saturnTexture->GetID());
	planet[5] = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", saturnTexture->GetID());

	// Planet - Uranus
	uranusTexture->Load("texture-uranus.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, uranusTexture->GetID());
	planet[6] = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", uranusTexture->GetID());

	// Planet - Neptune
	neptuneTexture->Load("texture-neptune.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, neptuneTexture->GetID());
	planet[7] = new Object3DS(Vector3D(0.0f, -1.0f, 0.0f), "Sphere.3ds", neptuneTexture->GetID());

	//Moon - Earth
	moonTexture->Load("Texture-Moon.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, moonTexture->GetID());
	m_p3DSModel_2 = new Object3DS(Vector3D(0.0f, -1.0f, -50.0f), "Sphere.3ds", moonTexture->GetID());

}

GameScreenLevel1::~GameScreenLevel1()
{	
	delete[] planet;
	delete[] skyboxTexture;
}

void setLight()
{
	lighting light = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
	//	100.0f

			/*{ 1.0f, 0.5f, 0.5f, 1.0f },
			{ 0.7f, 0.7f, 0.7f, 1.0f },
			{ 0.8f, 0.9f, 0.1f, 1.0f }*/
	};

	// position of the light in homogeneous coordinates (x, y, z, w)
	// w should be 0 for directional lights, and 1 for spotlights
	float light_pos[] = { 20.0f, 0.0f, 1.0f, 0.0f };

	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light.diffuse);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light.specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

void GameScreenLevel1::Render()
{
	//Parent class deals with camera.
	GameScreen::Render();

	RenderText();

	DrawSkybox();

	DrawPlanetsWithRotation();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	//Call parent update for camera functionality.
	GameScreen::Update(deltaTime, e);

	switch (e.type)
	{
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
				case SDLK_RETURN:
					pressed0 = false;
					pressed1 = false;
					pressed2 = false;
					pressed3 = false;
					pressed4 = false;
					pressed5 = false;
					pressed6 = false;
					pressed7 = false;
					pressed8 = false;
					pressedI = false;
					instructions = true;

					orbitAngle = 0.5f;
					orbitVelocity = 1.0f;
					moonOrbitVelocity = 0.5f;

					glEnable(GL_LIGHTING);

					DrawPlanetsWithRotation();
					break;

				case SDLK_0:
					pressed0 = true;
					pressed1 = false;
					pressed2 = false;
					pressed3 = false;
					pressed4 = false;
					pressed5 = false;
					pressed6 = false;
					pressed7 = false;
					pressed8 = false;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;
		
					DrawPlanetsWithoutRotation();
				break;
		
				if (pressedI = !pressedI)
				{
					if (pressedI)
					{
						case SDLK_i:
							pressedI = true;
					}
							
					else if (!pressedI)
					{
						pressedI = false;
					}
						break;								
				}

				case SDLK_1:
					pressed0 = false;
					pressed1 = true;
					pressed2 = false;
					pressed3 = false;
					pressed4 = false;
					pressed5 = false;
					pressed6 = false;
					pressed7 = false;
					pressed8 = false;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;

					DrawPlanetsWithoutRotation();
				break;

				case SDLK_2:
					pressed0 = false;
					pressed1 = false;
					pressed2 = true;
					pressed3 = false;
					pressed4 = false;
					pressed5 = false;
					pressed6 = false;
					pressed7 = false;
					pressed8 = false;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;

					DrawPlanetsWithoutRotation();
				break;

				case SDLK_3:
					pressed0 = false;
					pressed1 = false;
					pressed2 = false;
					pressed3 = true;
					pressed4 = false;
					pressed5 = false;
					pressed6 = false;
					pressed7 = false;
					pressed8 = false;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;
					
					DrawPlanetsWithoutRotation();
				break;

				case SDLK_4:
					pressed0 = false;
					pressed1 = false;
					pressed2 = false;
					pressed3 = false;
					pressed4 = true;
					pressed5 = false;
					pressed6 = false;
					pressed7 = false;
					pressed8 = false;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;

					DrawPlanetsWithoutRotation();
				break;

				case SDLK_5:
					pressed0 = false;
					pressed1 = false;
					pressed2 = false;
					pressed3 = false;
					pressed4 = false;
					pressed5 = true;
					pressed6 = false;
					pressed7 = false;
					pressed8 = false;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;

					DrawPlanetsWithoutRotation();
				break;

				case SDLK_6:
					pressed0 = false;
					pressed1 = false;
					pressed2 = false;
					pressed3 = false;
					pressed4 = false;
					pressed5 = false;
					pressed6 = true;
					pressed7 = false;
					pressed8 = false;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;

					DrawPlanetsWithoutRotation();
				break;

				case SDLK_7:
					pressed0 = false;
					pressed1 = false;
					pressed2 = false;
					pressed3 = false;
					pressed4 = false;
					pressed5 = false;
					pressed6 = false;
					pressed7 = true;
					pressed8 = false;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;

					DrawPlanetsWithoutRotation();
				break;

				case SDLK_8:
					pressed0 = false;
					pressed1 = false;
					pressed2 = false;
					pressed3 = false;
					pressed4 = false;
					pressed5 = false;
					pressed6 = false;
					pressed7 = false;
					pressed8 = true;
					pressedI = false;
					instructions = false;

					orbitAngle = 0.0f;
					orbitVelocity = 0.0f;
					moonOrbitVelocity = 0.0f;

					DrawPlanetsWithoutRotation();
				break;

				case SDLK_EQUALS:
					orbitVelocity += 0.5f;

					if (orbitVelocity >= 5.0f)
						orbitVelocity = 5.0f;
					break;

				case SDLK_MINUS:
					orbitVelocity -= 0.5f;

					if (orbitVelocity <= -5.0f)
						orbitVelocity = -5.0f;
					break;
			}
		break;

		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
				case SDLK_i:
					pressedI = false;
					break;
			}
			break;
	}

	mCurrentTime += deltaTime;

	orbitAngle += orbitVelocity;

	sun->Update(deltaTime);
	m_p3DSModel_2->Update(deltaTime);

	int numPlanets = 8;
	
	for (int i = 0; i < numPlanets; i++)
	{
		planet[i]->Update(deltaTime);
	}
}

void GameScreenLevel1::OutputLine(float x, float y, string text)
{
	glRasterPos2f(x, y);  // where to start drawing
	for (int i = 0; i < text.size(); i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
	}

}

void GameScreenLevel1::RenderText()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);

		if (instructions == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glColor3f(1.0f, 1.0f, 0.0f);
				OutputLine(2, 95, "Use number keys to go to planets.");
				OutputLine(55, 95, "Press 'Enter' key to restart rotation.");
				OutputLine(60, 2, "Press '=' to increase rotation.");
				OutputLine(2, 2, "Press '-' to decrease rotation.");
			glPopMatrix();
		}

		if (pressed0 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f, 1.0f, 0.0f);
				OutputLine(5, 95, "Sun");
				glColor3f(0.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressed0 == true & pressedI == true)
			{
				float size = 1.392f;
				string sunSize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream sunSS;
				sunSS << "Size: " << sunSize << " million km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, sunSS.str());
				glPopMatrix();

				int _sunCoreTemp = 27;
				string sunCoreTemp = static_cast<ostringstream*>(&(ostringstream() << _sunCoreTemp))->str();
				stringstream sunSS1;
				sunSS1 << "Min Temp: " << sunCoreTemp << " million degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, sunSS1.str());
				glPopMatrix();

				int _sunSurfaceTemp = 5500;
				string sunSurfaceTemp = static_cast<ostringstream*>(&(ostringstream() << _sunSurfaceTemp))->str();
				stringstream sunSS2;
				sunSS2 << "Max Temp: " << sunSurfaceTemp << " degrees C";

				glPushMatrix();
				glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, sunSS2.str());
				glPopMatrix();

				int _sunSatellites = 8;
				string sunSatellites = static_cast<ostringstream*>(&(ostringstream() << _sunSatellites))->str();
				stringstream sunSS3;
				sunSS3 << "Natural Satellites: " << sunSatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, sunSS3.str());
				glPopMatrix();
			}
		}

		if (pressed1 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f, 0.0f, 0.0f);
				OutputLine(5, 95, "Mercury");
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressedI == true & pressed1 == true)
			{
				int size = 4879;
				string mercurySize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream mercurySS;
				mercurySS << "Size: " << mercurySize << " km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, mercurySS.str());
				glPopMatrix();

				int _mercuryMinTemp = 173;
				string mercuryMinTemp = static_cast<ostringstream*>(&(ostringstream() << _mercuryMinTemp))->str();
				stringstream mercurySS1;
				mercurySS1 << "Min Temp: -" << mercuryMinTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, mercurySS1.str());
				glPopMatrix();

				int _mercuryMaxTemp = 427;
				string mercuryMaxTemp = static_cast<ostringstream*>(&(ostringstream() << _mercuryMaxTemp))->str();
				stringstream mercurySS2;
				mercurySS2 << "Max Temp: " << mercuryMaxTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, mercurySS2.str());
				glPopMatrix();

				int _mercurySatellites = 0;
				string mercurySatellites = static_cast<ostringstream*>(&(ostringstream() << _mercurySatellites))->str();
				stringstream mercurySS3;
				mercurySS3 << "Natural Satellites: " << mercurySatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, mercurySS3.str());
				glPopMatrix();
			}
		}

		if (pressed2 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f, 0.0f, 0.0f);
				OutputLine(5, 95, "Venus");
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressedI == true & pressed2 == true)
			{
				int size = 6052;
				string venusSize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream venusSS;
				venusSS << "Size: " << venusSize << " km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, venusSS.str());
				glPopMatrix();

				int _venusMinTemp = 173;
				string venusMinTemp = static_cast<ostringstream*>(&(ostringstream() << _venusMinTemp))->str();
				stringstream venusSS1;
				venusSS1 << "Min Temp: -" << venusMinTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, venusSS1.str());
				glPopMatrix();

				int _venusMaxTemp = 467;
				string venusMaxTemp = static_cast<ostringstream*>(&(ostringstream() << _venusMaxTemp))->str();
				stringstream venusSS2;
				venusSS2 << "Max Temp: " << venusMaxTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, venusSS2.str());
				glPopMatrix();

				int _venusSatellites = 0;
				string venusSatellites = static_cast<ostringstream*>(&(ostringstream() << _venusSatellites))->str();
				stringstream venusSS3;
				venusSS3 << "Natural Satellites: " << venusSatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, venusSS3.str());
				glPopMatrix();
			}
		}

		if (pressed3 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(0.0f, 0.0f, 1.0f);
				OutputLine(5, 95, "Earth");
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressedI == true & pressed3 == true)
			{
				int size = 6371;
				string earthSize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream earthSS;
				earthSS << "Size: " << earthSize << " km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, earthSS.str());
				glPopMatrix();

				float _earthMinTemp = 89.2;
				string earthMinTemp = static_cast<ostringstream*>(&(ostringstream() << _earthMinTemp))->str();
				stringstream earthSS1;
				earthSS1 << "Min Temp: " << earthMinTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, earthSS1.str());
				glPopMatrix();

				int _earthMaxTemp = 58;
				string earthMaxTemp = static_cast<ostringstream*>(&(ostringstream() << _earthMaxTemp))->str();
				stringstream earthSS2;
				earthSS2 << "Max Temp: " << earthMaxTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, earthSS2.str());
				glPopMatrix();

				int _earthSatellites = 1;
				string earthSatellites = static_cast<ostringstream*>(&(ostringstream() << _earthSatellites))->str();
				stringstream earthSS3;
				earthSS3 << "Natural Satellites: " << earthSatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, earthSS3.str());
				glPopMatrix();
			}
		}

		if (pressed4 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f, 1.0f, 0.0f);
				OutputLine(5, 95, "Mars");
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressedI == true & pressed4 == true)
			{
				int size = 3390;
				string marsSize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream marsSS;
				marsSS << "Size: " << marsSize << " km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, marsSS.str());
				glPopMatrix();

				float _marsMinTemp = 153;
				string marsMinTemp = static_cast<ostringstream*>(&(ostringstream() << _marsMinTemp))->str();
				stringstream marsSS1;
				marsSS1 << "Min Temp: -" << marsMinTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, marsSS1.str());
				glPopMatrix();

				int _marsMaxTemp = 20;
				string marsMaxTemp = static_cast<ostringstream*>(&(ostringstream() << _marsMaxTemp))->str();
				stringstream marsSS2;
				marsSS2 << "Max Temp: " << marsMaxTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, marsSS2.str());
				glPopMatrix();

				int _marsSatellites = 2;
				string marsSatellites = static_cast<ostringstream*>(&(ostringstream() << _marsSatellites))->str();
				stringstream marsSS3;
				marsSS3 << "Natural Satellites: " << marsSatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, marsSS3.str());
				glPopMatrix();
			}
		}

		if (pressed5 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(0.5f, 1.0f, 0.0f);
				OutputLine(5, 95, "Jupiter");
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressedI == true & pressed5 == true)
			{
				int size = 69911;
				string jupiterSize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream jupiterSS;
				jupiterSS << "Size: " << jupiterSize << " km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, jupiterSS.str());
				glPopMatrix();

				float _jupiterMinTemp = 128;
				string jupiterMinTemp = static_cast<ostringstream*>(&(ostringstream() << _jupiterMinTemp))->str();
				stringstream jupiterSS1;
				jupiterSS1 << "Min Temp: -" << jupiterMinTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, jupiterSS1.str());
				glPopMatrix();

				int _jupiterMaxTemp = 4;
				string jupiterMaxTemp = static_cast<ostringstream*>(&(ostringstream() << _jupiterMaxTemp))->str();
				stringstream jupiterSS2;
				jupiterSS2 << "Max Temp: " << jupiterMaxTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, jupiterSS2.str());
				glPopMatrix();

				int _jupiterSatellites = 67;
				string jupiterSatellites = static_cast<ostringstream*>(&(ostringstream() << _jupiterSatellites))->str();
				stringstream jupiterSS3;
				jupiterSS3 << "Natural Satellites: " << jupiterSatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, jupiterSS3.str());
				glPopMatrix();
			}
		}

		if (pressed6 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f, 1.0f, 0.0f);
				OutputLine(5, 95, "Saturn");
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressedI == true & pressed6 == true)
			{
				int size = 58232;
				string saturnSize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream saturnSS;
				saturnSS << "Size: " << saturnSize << " km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, saturnSS.str());
				glPopMatrix();

				int _saturnMinTemp = 150;
				string saturnMinTemp = static_cast<ostringstream*>(&(ostringstream() << _saturnMinTemp))->str();
				stringstream saturnSS1;
				saturnSS1 << "Min Temp: -" << saturnMinTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, saturnSS1.str());
				glPopMatrix();

				int _saturnMaxTemp = 11700;
				string saturnMaxTemp = static_cast<ostringstream*>(&(ostringstream() << _saturnMaxTemp))->str();
				stringstream saturnSS2;
				saturnSS2 << "Max Temp: " << saturnMaxTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, saturnSS2.str());
				glPopMatrix();

				int _saturnSatellites = 62;
				string saturnSatellites = static_cast<ostringstream*>(&(ostringstream() << _saturnSatellites))->str();
				stringstream saturnSS3;
				saturnSS3 << "Natural Satellites: " << saturnSatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, saturnSS3.str());
				glPopMatrix();
			}
		}

		if (pressed7 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f, 1.0f, 0.4f);
				OutputLine(5, 95, "Uranus");
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressedI == true & pressed7 == true)
			{
				int size = 25362;
				string uranusSize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream uranusSS;
				uranusSS << "Size: " << uranusSize << " km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, uranusSS.str());
				glPopMatrix();

				int _uranusMinTemp = 224;
				string uranusMinTemp = static_cast<ostringstream*>(&(ostringstream() << _uranusMinTemp))->str();
				stringstream uranusSS1;
				uranusSS1 << "Min Temp: -" << uranusMinTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, uranusSS1.str());
				glPopMatrix();

				int _uranusMaxTemp = 197;
				string uranusMaxTemp = static_cast<ostringstream*>(&(ostringstream() << _uranusMaxTemp))->str();
				stringstream uranusSS2;
				uranusSS2 << "Max Temp: -" << uranusMaxTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, uranusSS2.str());
				glPopMatrix();

				int _uranusSatellites = 27;
				string uranusSatellites = static_cast<ostringstream*>(&(ostringstream() << _uranusSatellites))->str();
				stringstream uranusSS3;
				uranusSS3 << "Natural Satellites: " << uranusSatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, uranusSS3.str());
				glPopMatrix();
			}
		}

		if (pressed8 == true)
		{
			glPushMatrix();
				glLoadIdentity();
				gluOrtho2D(0, 100, 0, 100);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(5, 95, "Neptune");
				glColor3f(1.0f, 1.0f, 1.0f);
				OutputLine(65, 95, "Hold 'I' to toggle more info.");
			glPopMatrix();

			if (pressedI == true & pressed8 == true)
			{
				int size = 24622;
				string neptuneSize = static_cast<ostringstream*>(&(ostringstream() << size))->str();
				stringstream neptuneSS;
				neptuneSS << "Size: " << neptuneSize << " km";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 85, neptuneSS.str());
				glPopMatrix();

				int _neptuneMinTemp = 218;
				string neptuneMinTemp = static_cast<ostringstream*>(&(ostringstream() << _neptuneMinTemp))->str();
				stringstream neptuneSS1;
				neptuneSS1 << "Min Temp: -" << neptuneMinTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 80, neptuneSS1.str());
				glPopMatrix();

				int _neptuneMaxTemp = 200;
				string neptuneMaxTemp = static_cast<ostringstream*>(&(ostringstream() << _neptuneMaxTemp))->str();
				stringstream neptuneSS2;
				neptuneSS2 << "Max Temp: -" << neptuneMaxTemp << " degrees C";

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 75, neptuneSS2.str());
				glPopMatrix();

				int _neptuneSatellites = 14;
				string neptuneSatellites = static_cast<ostringstream*>(&(ostringstream() << _neptuneSatellites))->str();
				stringstream neptuneSS3;
				neptuneSS3 << "Natural Satellites: " << neptuneSatellites;

				glPushMatrix();
					glLoadIdentity();
					gluOrtho2D(0, 100, 0, 100);
					glColor3f(0.0f, 1.0f, 0.0f);
					OutputLine(2, 70, neptuneSS3.str());
				glPopMatrix();
			}
		}

		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GameScreenLevel1::DrawSkybox()
{
	// Skybox 
	// White vertices to allow different coloured text.
	glPushMatrix();
		glScalef(600.0f, 600.0f, 600.0f);
		glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]->GetID());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Top)	
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Top)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Top)
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Top)
		glEnd();

		glBindTexture(GL_TEXTURE_2D, skyboxTexture[1]->GetID());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Bottom)
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Bottom)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,  1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Bottom)
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,  1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Bottom)
		glEnd();

		glBindTexture(GL_TEXTURE_2D, skyboxTexture[2]->GetID());
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Front)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f,  -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,  -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
		glEnd();

		glBindTexture(GL_TEXTURE_2D, skyboxTexture[3]->GetID());
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Back)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Back)
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Back)
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Back)
		glEnd();

		glBindTexture(GL_TEXTURE_2D, skyboxTexture[4]->GetID());
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f,  1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Left)
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f,  1.0f,  1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Left)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f,  1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
		glEnd();

		glBindTexture(GL_TEXTURE_2D, skyboxTexture[5]->GetID());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Right)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Right)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	glColor3f(1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Right)
		glEnd();
		glPopMatrix();
}

void GameScreenLevel1::DrawPlanetsWithRotation()
{
	// Sun
	glPushMatrix();
		sun->SetScale(Vector3D(3.0f, 3.0f, 3.0f));
		sun->SetRotation(Vector3D(0.0f, 5.0f, 0.0f), 0.2f);
		sun->SetMaterial(sunEmmisiveMaterial);
		sun->Render();

	glPushMatrix();
		setLight();
		glRotatef(10.0f, 0.0f, 5.0f, 0.0f);
	glPopMatrix();

	// Mercury
	glPushMatrix();
		glRotatef(orbitAngle, 0.0f, 5.0f, 0.0f);
		planet[0]->SetRotation(Vector3D(0.0f, 5.0f, 0.0f), 0.2f);
		planet[0]->SetScale(Vector3D(0.2f, 0.2f, 0.2f));
		glTranslatef(50.0f, 0.0f, 0.0f);
		planet[0]->Render();

	// Venus
	glPushMatrix();
		planet[1]->SetRotation(Vector3D(0.0f, 5.0f, 0.0f), 0.6f);
		planet[1]->SetScale(Vector3D(0.7f, 0.7f, 0.7f));
		glTranslatef(20.0f, 0.0f, 0.0f);
		planet[1]->Render();

	// Earth
	glPushMatrix();
		planet[2]->SetRotation(Vector3D(2.0f, 5.0f, 0.0f), 0.8f);
		planet[2]->SetScale(Vector3D(0.5f, 0.5f, 0.5f));
		glTranslatef(30.0f, 0.0f, 0.0f);
		planet[2]->Render();

	// Earth - Moon
	glPushMatrix();
		glRotatef(orbitAngle, 0.4f, 0.4f, 0.0f);
		glScalef(0.3f, 0.3f, 0.3f);
		glTranslatef(5.0f, 0.0f, 0.0f);
		m_p3DSModel_2->Render();
	glPopMatrix();

	// Mars
	glPushMatrix();
		planet[3]->SetRotation(Vector3D(0.0f, 5.0f, 0.0f), 0.5f);
		planet[3]->SetScale(Vector3D(0.4f, 0.4f, 0.4f));
		glTranslatef(20.0f, 0.0f, 0.0f);
		planet[3]->Render();

	// Jupiter
	glPushMatrix();
		planet[4]->SetRotation(Vector3D(0.0f, 5.0f, 0.0f), 1.0f);
		planet[4]->SetScale(Vector3D(2.0f, 2.0f, 2.0f));
		glTranslatef(35.0f, 0.0f, 0.0f);
		planet[4]->Render();

	// Saturn
	glPushMatrix();
		planet[5]->SetRotation(Vector3D(0.0f, 5.0f, 0.0f), 0.5f);
		planet[5]->SetScale(Vector3D(1.7f, 1.7f, 1.7f));
		glTranslatef(57.0f, 0.0f, 0.0f);
		planet[5]->Render();

	// Uranus
	glPushMatrix();
		planet[6]->SetRotation(Vector3D(0.0f, 5.0f, 0.0f), 0.5f);
		planet[6]->SetScale(Vector3D(1.2f, 1.2f, 1.2f));
		glTranslatef(50.0f, 0.0f, 0.0f);
		planet[6]->Render();

	// Neptune
	glPushMatrix();
		planet[7]->SetRotation(Vector3D(0.0f, 5.0f, 0.0f), 0.5f);
		planet[7]->SetScale(Vector3D(1.1f, 1.1f, 1.1f));
		glTranslatef(50.0f, 0.0f, 0.0f);
		planet[7]->Render();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void GameScreenLevel1::DrawPlanetsWithoutRotation()
{
	// Sun
	glPushMatrix();
		sun->SetScale(Vector3D(3.0f, 3.0f, 3.0f));
		sun->SetRotation(Vector3D(0.0f, 0.0f, 0.0f), 0.0f);
		sun->SetMaterial(sunEmmisiveMaterial);
		sun->Render();

	// Mercury
	glPushMatrix();
		planet[0]->SetScale(Vector3D(0.2f, 0.2f, 0.2f));
		glTranslatef(50.0f, 0.0f, 0.0f);
		planet[0]->Render();

	// Venus
	glPushMatrix();
	planet[1]->SetScale(Vector3D(0.7f, 0.7f, 0.7f));
	glTranslatef(20.0f, 0.0f, 0.0f);
	planet[1]->Render();

	// Earth
	glPushMatrix();
		planet[2]->SetScale(Vector3D(0.5f, 0.5f, 0.5f));
		glTranslatef(30.0f, 0.0f, 0.0f);
		planet[2]->Render();

	// Earth - Moon
	glPushMatrix();
		glScalef(0.3f, 0.3f, 0.3f);
		glTranslatef(35.0f, 0.0f, 0.0f);
		m_p3DSModel_2->Render();
	glPopMatrix();

	// Mars
	glPushMatrix();
		planet[3]->SetScale(Vector3D(0.4f, 0.4f, 0.4f));
		glTranslatef(20.0f, 0.0f, 0.0f);
		planet[3]->Render();

	// Jupiter
	glPushMatrix();
		planet[4]->SetScale(Vector3D(2.0f, 2.0f, 2.0f));
		glTranslatef(35.0f, 0.0f, 0.0f);
		planet[4]->Render();

	// Saturn
	glPushMatrix();
		planet[5]->SetScale(Vector3D(1.7f, 1.7f, 1.7f));
		glTranslatef(57.0f, 0.0f, 0.0f);
		planet[5]->Render();

	// Uranus
	glPushMatrix();
		planet[6]->SetScale(Vector3D(1.2f, 1.2f, 1.2f));
		glTranslatef(50.0f, 0.0f, 0.0f);
		planet[6]->Render();

	// Neptune
	glPushMatrix();
		planet[7]->SetScale(Vector3D(1.1f, 1.1f, 1.1f));
		glTranslatef(50.0f, 0.0f, 0.0f);
		planet[7]->Render();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}