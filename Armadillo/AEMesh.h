#pragma once

#include "AEObject.h"

class AEMesh : public AEObject
{
	AEObjectMinMax			BoundBoxMinMax;
	AEDrawObjectsCommand	DrawCommand;

public:
	AEMesh() {};
	~AEMesh() {};

	void SetPosition(glm::vec3 new_pos) { ModelMatrix = glm::translate(ModelMatrix, new_pos); };
	void SetBoundBox(AEObjectMinMax bbox) { BoundBoxMinMax = bbox; };
	void SetDrawCommand(AEDrawObjectsCommand draw_cmd) { DrawCommand = draw_cmd; };

	glm::vec3 GetPosition() { return Position; };
	AEObjectMinMax GetBoundBox() const { return BoundBoxMinMax; };
	AEDrawObjectsCommand GetDrawCommand() const { return DrawCommand; };
};