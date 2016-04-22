#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H
#include <Windows.h>
#include <gl\glu.h>

class Texture
{
private:
	GLuint _ID; // Texture ID
	int _width, _height;

public:
	Texture();
	~Texture();

	bool Load(char* path, int width, int height);

	GLuint GetID()			const { return _ID; }
	int GetWidth()			const { return _width; }
	int GetHeight()			const { return _height; }
};

#endif

