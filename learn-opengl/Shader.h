#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Shader
{
public:
	Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);
	virtual ~Shader();

	void Use();

	inline GLuint GetId() { return id; }

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMatrix4(const std::string& name,  float const* const& value) const;

private:
	GLuint id = 0;
};

