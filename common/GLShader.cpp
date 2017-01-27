
#include "GLShader.h"

#include <iostream>
#include <fstream>

#include "GL/glew.h"

bool GLShader::LoadShader(uint32_t type, const char* path)
{
	// on presuppose type = GL_VERTEX_SHADER ou 
	// GL_FRAGMENT_SHADER
	uint32_t shaderID;
	shaderID = glCreateShader(type);
	switch (type) {
	case GL_VERTEX_SHADER:
		_VertexShader = shaderID;
		break;
	case GL_FRAGMENT_SHADER:
		_FragmentShader = shaderID;
		break;
	case GL_GEOMETRY_SHADER:
		_GeometryShader = shaderID;
			break;
	default:
		break;
	}

	std::ifstream fileStream;
	fileStream.open(path, std::ios::binary);
	if (!fileStream.is_open())
		return false;
	auto begin = fileStream.tellg();
	fileStream.seekg(0, std::ios::end);
	auto file_len = fileStream.tellg() - begin;
	char* buffer = new char[file_len + 1];
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(buffer, file_len);
	fileStream.close();
	
	buffer[file_len] = '\0';
	glShaderSource(shaderID, 1, &buffer, nullptr);
	delete[] buffer;
	
	glCompileShader(shaderID);
	// check le resultat de la compilation
	int compileSuccess = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess)
	{
		char errorBuffer[4096];
		int error_len;
		glGetShaderInfoLog(shaderID, 4096, &error_len, errorBuffer);
		std::cout << "erreur dans le" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader: " << errorBuffer << std::endl;
		return false;
	}
	return true;
}

bool GLShader::Create()
{
	_Program = glCreateProgram();
	glAttachShader(_Program, _VertexShader);
	glAttachShader(_Program, _FragmentShader);
	glAttachShader(_Program, _GeometryShader);
	glLinkProgram(_Program);
	// checker le resultat du linkage
	int linkSuccess = 0;
	glGetProgramiv(_Program, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess)
	{
		char errorBuffer[4096];
		int error_len;
		glGetProgramInfoLog(_Program, 4096, &error_len, 
			errorBuffer);
		std::cout << "erreur de linkage: " 
			<< errorBuffer << std::endl;
		return false;
	}
	return true;
}

void GLShader::Destroy()
{
	glDeleteShader(_VertexShader);
	glDeleteShader(_FragmentShader);
	glDeleteProgram(_Program);
}

void GLShader::Bind() { glUseProgram(_Program); }

void GLShader::Unbind() { glUseProgram(0); }