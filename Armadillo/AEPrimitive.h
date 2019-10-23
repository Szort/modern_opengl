#pragma once

#include "AEObject.h"

const float AEPlane_Vertex[] {
	-1.0, -1.0, 0.0,	0.0, 0.0,
	-1.0,  1.0, 0.0,	0.0, 1.0,
	 1.0,  1.0, 0.0,	1.0, 1.0,
	 1.0, -1.0, 0.0,	1.0, 0.0
};

const uint32_t AEPlane_Indices[]{
	0, 1, 2, 0, 2, 3
};

const float AECube_Vertex[] {
	-1.0, -1.0,  1.0,
	-1.0,  1.0,  1.0,
	 1.0,  1.0,  1.0,
	 1.0, -1.0,  1.0,
	-1.0, -1.0, -1.0,
	-1.0,  1.0, -1.0,
	 1.0,  1.0, -1.0,
	 1.0, -1.0, -1.0
};

const uint32_t AECube_Indices[]{
	0, 1, 2, 0, 2, 3,
	4, 5, 6, 4, 6, 7
};

enum AEPrimitiveType
{
	eAE_PrimitiveType_Plane,
	eAE_PrimitiveType_Cube,
	eAE_PrimitiveType_Null
};

class AEPrimitive : public AEObject
{
	float*		Vertexes;
	uint32_t*	Indices;
	uint32_t	VertexCount;
	uint32_t	IndicesCount;

public:
	AEPrimitive(AEPrimitiveType prim_type = eAE_PrimitiveType_Null) {
		SetObjType(eAE_ObjectType_Primitive);

		switch (prim_type)
		{
		case eAE_PrimitiveType_Plane:
			SetName("Plane");
			Vertexes = (float*)AEPlane_Vertex;
			Indices = (uint32_t*)AEPlane_Indices;
			VertexCount = sizeof(AEPlane_Vertex) / 20;
			IndicesCount = sizeof(AEPlane_Indices) / 4;
			break;
		case eAE_PrimitiveType_Cube:
			SetName("Cube");
			Vertexes = (float*)AECube_Vertex;
			Indices = (uint32_t*)AECube_Indices;
			VertexCount = sizeof(AECube_Vertex) / 20;
			IndicesCount = sizeof(AECube_Indices) / 4;
			break;
		default:
			SetName("NullObject");
			Vertexes = nullptr;
			Indices = nullptr;
			VertexCount = 0;
			IndicesCount = 0;
			break;
		}

		Selected = false;
		Visible = true;
	};
	~AEPrimitive() {};

	float*		GetVertexes() { return Vertexes; };
	uint32_t*	GetIndices() { return Indices; };
	uint32_t	GetVertexCount() { return VertexCount; };
	uint32_t	GetIndicesCount() { return IndicesCount; };

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