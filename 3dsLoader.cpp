#include <stdio.h>
#include <stdlib.h>
#include "Commons.h"
#include "3dsLoader.h"
#include <sys\stat.h>

long filelength(int f)
{
	struct  stat buf;
	
	fstat(f, &buf);

	return(buf.st_size);
}

char Load3DS(obj_type_ptr p_object, char* p_filename)
{
	int i;																// Index Variable

	FILE* inFile;														// FILE pointer

	unsigned short chunkId, s_Temp;										// Chunk ID
	unsigned int chunkLength;											// Chunk Length
	unsigned char name;													// Char varibale
	unsigned short size;												// Number of elements in each chunk

	unsigned short faceFlag;											// Flag stores face info

	if ((inFile = fopen(p_filename, "rb")) == NULL) return 0;			// Open the file

	while (ftell(inFile) < filelength(fileno(inFile)))					// Loop to scan the whole file
	{
		fread(&chunkId, 2, 1, inFile);									// Read the chunk header
		fread(&chunkLength, 4, 1, inFile);								// Read the chunk length
		//getchar(); //Insert this command for debug (to wait for keypress for each chuck reading)

		switch (chunkId)
		{
			//--------------------- MAIN3DS --------------------------
			// Main chunk, contains all other chunks
			// Chunk ID: 4d4d
			// Chunk Length: 0 + sub chunks
			//--------------------------------------------------------
			case 0x4d4d:
			break;

			//--------------------- EDIT3DS --------------------------
			// 3D Editor chunk, contains object layout
			// Chunk ID: 3d3d (hex)
			// Chunk Length: 0 + sub chunks
			//--------------------------------------------------------
			case 0x3d3d:
			break;

			//--------------------- EDIT_OBJECT --------------------------
			// Object block, each object info
			// Chunk ID: 4000 (hex)
			// Chunk Length: len(object name) + sub chunks
			//------------------------------------------------------------
			case 0x4000:
				i = 0;
				do
				{
					fread(&name, 1, 1, inFile);
						 p_object->name[i] = name;
					i++;
				} 
				while (name != '\0' && i < 20);
			break;

			//--------------------- OBJ_TRIMESH --------------------------
			// Triangualar mesh, contains 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Length: 0 + sub chunks
			//------------------------------------------------------------
			case 0x4100:
			break;

			//--------------------- TRI_VERTEXL --------------------------
			// Vertices List
			// Chunk ID: 4110 (hex)
			// Chunk Length: 1 x unsigned short (number of vertices)
			//				+ 3 x float (vertex coordinates) x (number of vertices)
			//				+ sub chunks
			//------------------------------------------------------------
			case 0x4110:
				fread(&size, sizeof(unsigned short), 1, inFile);
				p_object->vertices_qty = size;

				for (i = 0; i < size; i++)
				{
					fread(&p_object->vertex[i].x, sizeof(float), 1, inFile);
					fread(&p_object->vertex[i].y, sizeof(float), 1, inFile);
					fread(&p_object->vertex[i].z, sizeof(float), 1, inFile);
				}
				break;

			//--------------------- TRI_FACEL1 --------------------------
			// Polygon list
			// Chunk ID: 4120 (hex)
			// Chunk Length: 1 x unsigned short (number of polygons)
			//				+ 3 x unsigned short (polygon points) x (number of polygons)
			//				+ sub chunks
			//-----------------------------------------------------------
			case 0x4120:
				fread(&size, sizeof(unsigned short), 1, inFile);
				p_object->polygons_qty = size;

				for (i = 0; i < size; i++)
				{
					fread(&s_Temp, sizeof(unsigned short), 1, inFile);
					p_object->polygon[i].a = s_Temp;

					fread(&s_Temp, sizeof(unsigned short), 1, inFile);
					p_object->polygon[i].b = s_Temp;

					fread(&s_Temp, sizeof(unsigned short), 1, inFile);
					p_object->polygon[i].c = s_Temp;

					fread(&s_Temp, sizeof(unsigned short), 1, inFile);
				}
			break;

			//--------------------- TRI_MAPPINGCOORDS --------------------------
			// Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Length: 1 x unsigned short (number of mapping points)
			//				+ 3 x unsigned short (mapping coords) x (number of mapping points)
			//				+ sub chunks
			//------------------------------------------------------------------
			case 0x4140:
				fread(&size, sizeof(unsigned short), 1, inFile);

				for (i = 0; i < size; i++)
				{
					fread(&p_object->mapcoord[i].u, sizeof(float), 1, inFile);
					fread(&p_object->mapcoord[i].v, sizeof(float), 1, inFile);
				}
			break;

			//--------------------- Skip unkown chunks --------------------------
			// Skip all chunks that aren't in use
			// Use chunk length info to set file pointer
			// to the same level next chunk
			//------------------------------------------------------------------
			default:
				fseek(inFile, chunkLength - 6, SEEK_CUR);
		}
	}

	fclose(inFile);
	return(1);
}