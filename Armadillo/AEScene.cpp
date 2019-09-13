#include "AEScene.h"

template <class T>
void AEScene::Add(T object)
{
	AEObject cast_object = static_cast<AEObject>(object);
	
	switch (cast_object->GetObjType()) {
	case eAE_ObjectType_Camera:
		Cameras.push_back(object);
		break;
	case eAE_ObjectType_Light:
		Lights.push_back(object);
		break;
	case eAE_ObjectType_Primitive:
		Primitives.push_back(object);
		break;
	case eAE_ObjectType_Geometry:
		Geometry.push_back(object);
		break;
	default:
		break;
	}

	delete cast_object;
	cast_object = nullptr;
}

template <class T>
void AEScene::Destroy(T list)
{

}