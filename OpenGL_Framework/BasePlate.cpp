#include "BasePlate.h"

BasePlate::BasePlate()
{
	TT = TransformType::TYPE_BasePlate;
}

BasePlate::~BasePlate()
{
}

void BasePlate::setBoundingRadius(float boundingRadius)
{
	_BR = boundingRadius;
}

float BasePlate::getBoundingRadius()
{
	return _BR;
}
