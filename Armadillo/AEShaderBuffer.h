#pragma once

#include "AECore.h"

enum AEShaderBufferType
{
	eAE_ShaderBufferType_UBO,
	eAE_ShaderBufferType_SSBO,
	eAE_ShaderBufferType_UnDefined
};

class AEShaderBuffer
{
	uint32_t			ID;
	void*				Pointer;
	AEShaderBufferType	Type;
	size_t				Size;

public:
	AEShaderBuffer() : 
		ID(0), 
		Pointer(NULL), 
		Type(eAE_ShaderBufferType_UnDefined), 
		Size(0) 
	{};

	~AEShaderBuffer() { 
		if (Pointer) {
			// Do cleanup from GPU
			delete Pointer; 
			Pointer = NULL;
		}
	};

	uint32_t GetId() const { return ID; };
	void* GetPointer() { return Pointer; };
	AEShaderBufferType GetType() { return Type; };
	size_t GetSize() { return Size; };

	void CreateBuffer(AEShaderBufferType type, uint32_t binding, size_t size, const char* name)
	{
		uint32_t bufferType = 0;

		switch (type)
		{
		case eAE_ShaderBufferType_UBO:
			bufferType = GL_UNIFORM_BUFFER;
			break;
		case eAE_ShaderBufferType_SSBO:
			bufferType = GL_SHADER_STORAGE_BUFFER;
			break;
		case eAE_ShaderBufferType_UnDefined:
			return;
			break;
		}

		glGenBuffers(1, &ID);
		glBindBuffer(bufferType, ID);
		glBufferStorage(bufferType, size, 0, flags);
		Pointer = glMapBufferRange(bufferType, 0, size, flags);
		glBindBufferBase(bufferType, binding, ID);
		glBindBuffer(bufferType, 0);

		glObjectLabel(GL_BUFFER, ID, -1, name);
		Size = size;
	}

	void CopyDataToGPU(const void* data)
	{
		std::memcpy(Pointer, data, Size);
	}
};