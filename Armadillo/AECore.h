#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <assimp/scene.h>

#define VAO_POSITION_LOCATION			0
#define	VAO_COLOR_LOCATION				1
#define VAO_NORMAL_LOCATION				2
#define VAO_TEXTURECOORD_LOCATION		3
#define VAO_DRAWID_LOCATION				4

#define SSBO_MODEL_MATRIX_LOCATION		0

#define UBO_GLOBAL_PARAMS_LOCATION		0


static uint32_t flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;