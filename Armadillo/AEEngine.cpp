#include "AEEngine.h"

void AEEngine::CompileVAO()
{
	// Generate geometry buffers
	glGenVertexArrays(1, &VAO_Static);
	glGenBuffers(1, &VBO_Static);
	glGenBuffers(1, &IBO_Static);
	glGenBuffers(1, &DCBO_Static);
	glGenBuffers(1, &DIBO_Static);

	glBindVertexArray(VAO_Static);
	glObjectLabel(GL_BUFFER, VAO_Static, -1, "Vertex Array Buffer");

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Static);

	glEnableVertexAttribArray(VAO_POSITION_LOCATION);
	glVertexAttribPointer(VAO_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, DrawList.stride_size, 0);

	glEnableVertexAttribArray(VAO_COLOR_LOCATION);
	glVertexAttribPointer(VAO_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, DrawList.stride_size, (void*)(4 * sizeof(float)));

	glEnableVertexAttribArray(VAO_NORMAL_LOCATION);
	glVertexAttribPointer(VAO_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, DrawList.stride_size, (void*)(8 * sizeof(float)));

	glEnableVertexAttribArray(VAO_TEXTURECOORD_LOCATION);
	glVertexAttribPointer(VAO_TEXTURECOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, DrawList.stride_size, (void*)(12 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Static);
	glBufferData(GL_ARRAY_BUFFER, DrawList.vertex_count * DrawList.stride_size, DrawList.vertex_data, GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, VBO_Static, -1, "Vertex Buffer");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Static);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, DrawList.indices_count * sizeof(unsigned int), DrawList.indices_data, GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, IBO_Static, -1, "Indices Buffer");

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, DCBO_Static);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, DrawList.CommandList.size() * sizeof(AEDrawElementsCommand), &DrawList.CommandList[0], GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, DCBO_Static, -1, "Indirect Draw Buffer");

	glBindBuffer(GL_ARRAY_BUFFER, DIBO_Static);
	glBufferData(GL_ARRAY_BUFFER, DrawList.CommandList.size() * sizeof(unsigned int), &DrawList.IndexList[0], GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, DIBO_Static, -1, "Matrix ID Buffer");

	glEnableVertexAttribArray(VAO_DRAWID_LOCATION);
	glVertexAttribIPointer(VAO_DRAWID_LOCATION, 1, GL_UNSIGNED_INT, 0, (void*)0);
	glVertexAttribDivisor(VAO_DRAWID_LOCATION, 1);
}

void AEEngine::Idle()
{
	// Sleeping time
	SleepTime = (1000 / FpsCap) - RenderTime;
	SleepTime = SleepTime < 0 ? 0 : SleepTime;
	Sleep(SleepTime);
}

void AEEngine::BindVAO()
{
	glBindVertexArray(VAO_Static);
}

void AEEngine::UnbindVAO()
{
	glBindVertexArray(0);
}