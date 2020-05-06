#pragma once

#include "AEObject.h"

enum AEPrimitiveType
{
	eAE_PrimitiveType_Plane,
	eAE_PrimitiveType_Cube,
	eAE_PrimitiveType_Null
};

class AEPrimitive : public AEObject
{
	std::vector<float> AEPlane_Vertex = {
	-1.0, -1.0, 0.0,	0.0, 0.0,
	-1.0,  1.0, 0.0,	0.0, 1.0,
	 1.0,  1.0, 0.0,	1.0, 1.0,
	 1.0, -1.0, 0.0,	1.0, 0.0
	};

	std::vector<int> AEPlane_Indices{
		0, 1, 2, 0, 2, 3
	};

	std::vector<float> AECube_Vertex{
		-0.5, -0.5, -0.5,  0.0,  0.0,	 0.5, -0.5, -0.5,  0.0,  0.0,
		 0.5,  0.5, -0.5,  0.0,  0.0,	 0.5,  0.5, -0.5,  0.0,  0.0,
		-0.5,  0.5, -0.5,  0.0,  0.0,	-0.5, -0.5, -0.5,  0.0,  0.0,
		-0.5, -0.5,  0.5,  0.0,  0.0,	 0.5, -0.5,  0.5,  0.0,  0.0,
		 0.5,  0.5,  0.5,  0.0,  0.0,	 0.5,  0.5,  0.5,  0.0,  0.0,
		-0.5,  0.5,  0.5,  0.0,  0.0,	-0.5, -0.5,  0.5,  0.0,  0.0,
		-0.5,  0.5,  0.5, -1.0,  0.0,	-0.5,  0.5, -0.5, -1.0,  0.0,
		-0.5, -0.5, -0.5, -1.0,  0.0,	-0.5, -0.5, -0.5, -1.0,  0.0,
		-0.5, -0.5,  0.5, -1.0,  0.0,	-0.5,  0.5,  0.5, -1.0,  0.0,
		 0.5,  0.5,  0.5,  1.0,  0.0,	 0.5,  0.5, -0.5,  1.0,  0.0,
		 0.5, -0.5, -0.5,  1.0,  0.0,	 0.5, -0.5, -0.5,  1.0,  0.0,
		 0.5, -0.5,  0.5,  1.0,  0.0,	 0.5,  0.5,  0.5,  1.0,  0.0,
		-0.5, -0.5, -0.5,  0.0, -1.0,	 0.5, -0.5, -0.5,  0.0, -1.0,
		 0.5, -0.5,  0.5,  0.0, -1.0,	 0.5, -0.5,  0.5,  0.0, -1.0,
		-0.5, -0.5,  0.5,  0.0, -1.0,	-0.5, -0.5, -0.5,  0.0, -1.0,
		-0.5,  0.5, -0.5,  0.0,  1.0,	 0.5,  0.5, -0.5,  0.0,  1.0,
		 0.5,  0.5,  0.5,  0.0,  1.0,	 0.5,  0.5,  0.5,  0.0,  1.0,
		-0.5,  0.5,  0.5,  0.0,  1.0,	-0.5,  0.5, -0.5,  0.0,  1.0
	};

	std::vector<int> AECube_Indices{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7
	};

	AEPrimitiveType			ShapeType;
	std::vector<float>		Vertexes;
	std::vector<int> 		Indices;
	uint32_t				VertexCount;
	uint32_t				IndicesCount;

	uint32_t				VAO;
	uint32_t				VBO;
	uint32_t				IBO;

public:
	AEPrimitive(AEPrimitiveType prim_type = eAE_PrimitiveType_Null) {
		SetObjType(eAE_ObjectType_Primitive);
		ShapeType = prim_type;

		switch (prim_type)
		{
		case eAE_PrimitiveType_Plane:
			SetName("Plane");
			Vertexes = AEPlane_Vertex;
			Indices = AEPlane_Indices;
			VertexCount = sizeof(AEPlane_Vertex) / 20;
			IndicesCount = sizeof(AEPlane_Indices) / 4;
			break;
		case eAE_PrimitiveType_Cube:
			SetName("Cube");
			Vertexes = AECube_Vertex;
			Indices = AECube_Indices;
			VertexCount = sizeof(AECube_Vertex) / 20;
			IndicesCount = sizeof(AECube_Indices) / 4;
			break;
		default:
			SetName("NullObject");
			VertexCount = 0;
			IndicesCount = 0;
			break;
		}

		glCreateBuffers(1, &VBO);
		glNamedBufferStorage(VBO, 20 * sizeof(float), 0, GL_DYNAMIC_STORAGE_BIT);
		glCreateBuffers(1, &IBO);
		glNamedBufferStorage(IBO, 6 * sizeof(int), 0, GL_DYNAMIC_STORAGE_BIT);
		
		glCreateVertexArrays(1, &VAO);
		glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));
		glVertexArrayElementBuffer(VAO, IBO);
		
		glEnableVertexArrayAttrib(VAO, 0);
		glEnableVertexArrayAttrib(VAO, 1);
		glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_TRUE, 0);
		glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_TRUE, 3 * sizeof(float));
		glVertexArrayAttribBinding(VAO, 0, 0);
		glVertexArrayAttribBinding(VAO, 1, 0);

		glNamedBufferSubData(VBO, 0, 20 * sizeof(float), AEPlane_Vertex.data());
		glNamedBufferSubData(IBO, 0, 6 * sizeof(int), AEPlane_Indices.data());

		Selected = false;
		Visible = true;
	};
	~AEPrimitive() {};

	std::vector<float> GetVertexes() { return Vertexes; };
	std::vector<int> GetIndices() { return Indices; };
	uint32_t GetVertexCount() { return VertexCount; };
	uint32_t GetIndicesCount() { return IndicesCount; };

	void RenderShape()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	};

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