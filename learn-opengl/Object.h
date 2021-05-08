#pragma once

#include "Transform.h"

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	Transform& transform()
	{
		return mTransform;
	}

private:
	Transform mTransform;
};

