#include "PhysicsShape.h"

PhysicsShape::PhysicsShape(PhysicsShapeType type)
{
	shape_type = type;
}

PhysicsShapeType PhysicsShape::GetBodyShape() const
{
	return shape_type;
}
