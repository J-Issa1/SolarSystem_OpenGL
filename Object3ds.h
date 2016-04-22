#ifndef _OBJECT3DS_H
#define _OBJECT3DS_H

#include "Commons.h"
#include <string>
using std::string;

class Object3DS
{
public:
	Object3DS(Vector3D startPosition, string modelFileName, int texID);
	~Object3DS(){}

	void Update(float deltaTime);
	void Render();

	// Load 3DS File
	void LoadModel();

	// Load texture for model
	void LoadTexture();

	void SetRotation(Vector3D mChangeRotation, float angle);
	void SetPosition(Vector3D mChangePosition);
	void SetScale(Vector3D mChangeScale);

	void SetMaterial(material newMaterial);

private:
	Vector3D mPosition, mScale, mRotation;

	char fileName[20];
	char textureNme[20];

	obj_type object;

	float mVelocity, mRotVelocity, rotateAngle;
};

#endif