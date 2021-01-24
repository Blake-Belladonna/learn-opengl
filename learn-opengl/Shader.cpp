#include "Shader.h"

Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath)
{
	std::stringstream vertexSS, framentSS;
	std::ifstream vertexFS, fragmentFS;
	vertexFS.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFS.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexFS.open(vertexShaderPath);
		fragmentFS.open(fragmentShaderPath);

		vertexSS << vertexFS.rdbuf();
		framentSS << fragmentFS.rdbuf();

		vertexFS.close();
		fragmentFS.close();
	}
	catch (const std::ifstream::failure&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	std::string vertexShaderString = vertexSS.str();
	std::string fragmentShaderString = framentSS.str();
	const char* vertexShaderCString = vertexShaderString.c_str();
	const char* fragmentShaderCString = fragmentShaderString.c_str();

	GLuint vs, fs;
	GLint success;
	char logBuffer[512];

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &(vertexShaderCString), NULL);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, sizeof(logBuffer), NULL, logBuffer);
		std::cout << "ERROR::SHADER_COMPILE_FAILDED:" << logBuffer << std::endl;
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderCString, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, sizeof(logBuffer), NULL, logBuffer);
		std::cout << "ERROR::SHADER_COMPILE_FAILED:" << logBuffer << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(id, sizeof(logBuffer), NULL, logBuffer);
		std::cout << "ERROR::SHADER_PROGRAM_LINK_FAILED:" << logBuffer << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::Use()
{
	glUseProgram(id);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	GLint i = glGetUniformLocation(id, name.c_str());
	glUniform1i(i, (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	GLint i = glGetUniformLocation(id, name.c_str());
	glUniform1i(i, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	GLint i = glGetUniformLocation(id, name.c_str());
	glUniform1f(i, value);
}

