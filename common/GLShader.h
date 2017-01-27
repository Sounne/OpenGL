#pragma once

#include <cstdint>
#include <algorithm>

class GLShader
{
public:
	GLShader() = default;
	~GLShader() = default;

	bool LoadShader(uint32_t type, const char* path);
	bool Create();
	void Destroy();
	void Bind();
	void Unbind();

	inline 
	uint32_t Get() {
		return _Program;
	}
	int* ptr;
private:
	
	uint32_t _Program;
	uint32_t _VertexShader;
	uint32_t _FragmentShader;
	uint32_t _GeometryShader;
};