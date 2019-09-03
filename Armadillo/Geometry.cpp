#include "Geometry.h"
#include "AEPrimitive.h"

bool Geometry::Init()
{
	AEPrimitive plane(eAE_PrimitiveType_Plane);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), plane.GetVertexes(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	if (vbo != 0 && vao != 0)
		return true;
	else
		return false;
}

void Geometry::BindGeometry()
{
	glBindVertexArray(vao);
}

void Geometry::UnbindGeometry()
{
	glBindVertexArray(0);
}