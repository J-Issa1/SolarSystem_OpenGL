#include "Texture.h"
#include <iostream>
#include <fstream>

using namespace std;

Texture::Texture()
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &_ID);
}

bool Texture::Load(char* path, int width, int height)
{
	char* tempTextureData; int fileSize; ifstream inFile;
	int _width = width; int _height = height;
	inFile.open(path, ios::binary);

	if (!inFile.good())
	{
		cerr << "Can't open texture file " << path << endl;
		return false;
	}

	inFile.seekg(0, ios::end);					// Go to end of file.
	fileSize = (int)inFile.tellg();				// Get the current position. Gives us the end as we set that in the above line of code.
	tempTextureData = new char[fileSize];		// Create a new array.
	inFile.seekg(0, ios::beg);					// Go to beginning of file.
	inFile.read(tempTextureData, fileSize);		// Read the texture data and the filesize.
	inFile.close();								// Close the file

	cout << path << " loaded  \n" << endl;

	glGenTextures(1, &_ID);						// Get 1 texure of the Texture ID
	glBindTexture(GL_TEXTURE_2D, _ID);			// Bind the Texture to the ID set
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete [] tempTextureData;
	return true;
}
