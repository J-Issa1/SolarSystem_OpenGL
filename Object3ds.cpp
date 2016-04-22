#include "Object3ds.h"
#include "./gl/glut.h"
#include "3dsLoader.h"
#include "Texture.h"
#include <iostream>
#include <fstream>

using namespace std;

Object3DS::Object3DS(Vector3D startPosition, string modelFileName, int texID)
{
	mPosition = startPosition;
	mRotation = Vector3D(0.0f, 0.0f, 0.0f);
	mScale = Vector3D(1.0f, 1.0f, 1.0f);

	rotateAngle = 0.0f;

	//3ds file to load
	std::strcpy(fileName, modelFileName.c_str());
	LoadModel();
	object.id_texture = texID;
}

void Object3DS::Update(float deltaTime)
{
}

void Object3DS::SetMaterial(material newMaterial)
{
	material material = {
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.4f, 0.4f, 0.4f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			0.0f,
			{ 0.4f, 0.2f, 0.0f }
	};

	glMaterialfv(GL_FRONT, GL_AMBIENT, material.diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material.emmisive);
}

void Object3DS::Render()
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(mScale.x, mScale.y, mScale.z);
		glRotatef(rotateAngle, mRotation.x, mRotation.y, mRotation.z);
		glTranslatef(mPosition.x, mPosition.y, mPosition.z);

		// Set the active texture.
		glBindTexture(GL_TEXTURE_2D, object.id_texture);

		glBegin(GL_TRIANGLES);
			for (int l_index = 0; l_index < object.polygons_qty; l_index++)
			{
				// ----------------- FIRST VERTEX ----------------------------------
				// Texture Coordinates
				 glTexCoord2f(object.mapcoord[object.polygon[l_index].a].u, object.mapcoord[object.polygon[l_index].a].v);

				// Coordinates of the first Vertex
				glVertex3f(object.vertex[object.polygon[l_index].a].x, object.vertex[object.polygon[l_index].a].y, object.vertex[object.polygon[l_index].a].z);

				// ----------------- SECOND VERTEX ----------------------------------
				// Texture Coordinates 
				 glTexCoord2f(object.mapcoord[object.polygon[l_index].b].u, object.mapcoord[object.polygon[l_index].b].v);

				// Coordinates of the second Vertex
				glVertex3f(object.vertex[object.polygon[l_index].b].x, object.vertex[object.polygon[l_index].b].y, object.vertex[object.polygon[l_index].b].z);

				// ----------------- THIRD VERTEX ----------------------------------
				// Texture Coordinates
				 glTexCoord2f(object.mapcoord[object.polygon[l_index].c].u, object.mapcoord[object.polygon[l_index].c].v);

				// Coordinates of the third Vertex
				glVertex3f(object.vertex[object.polygon[l_index].c].x, object.vertex[object.polygon[l_index].c].y, object.vertex[object.polygon[l_index].c].z);
			}
		glEnd();
	glPopMatrix();
}

void Object3DS::LoadModel()
{
	if (fileName[0] != '-')
		Load3DS(&object, fileName);
}

void Object3DS::LoadTexture()
{	
}

void Object3DS::SetPosition(Vector3D mChangePosition)
{
	mPosition = mChangePosition;
}

void Object3DS::SetRotation(Vector3D mChangeRotation, float angle)
{
	mRotation = mChangeRotation;
	rotateAngle += angle;
}

void Object3DS::SetScale(Vector3D mChangeScale)
{
	mScale = mChangeScale;
}