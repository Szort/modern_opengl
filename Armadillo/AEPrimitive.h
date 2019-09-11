#pragma once

#include "AEObject.h"

const float AEPlane_Vertex[] {
	-1.0, -1.0, 0.0,
	-1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, -1.0, 0.0
};

const int AEPlane_Indices[]{
	1, 2, 3, 1, 3, 4
};

const float AECube_Vertex[] {
	-1.0, -1.0, 1.0,
	-1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, -1.0, 1.0,
	-1.0, -1.0, -1.0,
	-1.0, 1.0, -1.0,
	1.0, 1.0, -1.0,
	1.0, -1.0, -1.0
};

const int AECube_Indices[]{
	1, 2, 3, 1, 3, 4,
	5, 6, 7, 5, 7, 8
};

enum AEPrimitiveType
{
	eAE_PrimitiveType_Plane,
	eAE_PrimitiveType_Cube,
	eAE_PrimitiveType_Null
};

class AEPrimitive : public AEObject
{
	float* Vertexes;
	int* Indices;

public:
	AEPrimitive(AEPrimitiveType prim_type = eAE_PrimitiveType_Null) {
		SetObjType(eAE_ObjectType_Primitive);

		switch (prim_type)
		{
		case eAE_PrimitiveType_Plane:
			Name = "Plane";
			Vertexes = (float*)AEPlane_Vertex;
			Indices = (int*)AEPlane_Indices;
			break;
		case eAE_PrimitiveType_Cube:
			Name = "Cube";
			Vertexes = (float*)AECube_Vertex;
			Indices = (int*)AECube_Indices;
			break;
		default:
			Name = "NullObject";
			Vertexes = nullptr;
			Indices = nullptr;
			break;
		}
	};
	~AEPrimitive() {};

	float* GetVertexes() { return Vertexes; };
	int* GetIndices() { return Indices; };

private:
	void ComputeGrid(unsigned int grid_size)
	{
		float* grid_vert = new float[grid_size * grid_size];
		for (uint16_t y = 0; y < grid_size; y++)
		{
			for (uint16_t x = 0; x < grid_size; x++)
			{

			}
		}
	}
};