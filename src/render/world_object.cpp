#include <world_object.h>

using namespace so;

WorldObject::WorldObject(Model *model) :
	m_transform(),
	m_model(model)
{
}

WorldObject::~WorldObject()
{
}
