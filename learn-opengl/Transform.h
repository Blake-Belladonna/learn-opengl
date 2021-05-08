#pragma once

#include <glm/glm.hpp>

class Transform
{
public:

	Transform()
	{
		mPosition = glm::vec3(0.0f);
		mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		mForward = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	virtual ~Transform() {}

	glm::vec3& position()
	{
		return mPosition;
	}

	glm::vec3& up()
	{
		return mUp;
	}

	glm::vec3& forward()
	{
		return mForward;
	}

private:
	glm::vec3 mPosition;
	glm::vec3 mUp;
	glm::vec3 mForward;
};

