#pragma once

#include "Glew/include/glew.h"

typedef unsigned int uint;

class FBO
{
public:
	FBO();
	~FBO();

	bool Create(uint width, uint height);
	void Resize(uint width, uint height);
	void Bind();
	void Unbind();
	void Draw();
	const GLuint GetTexture() const;

private:
	void Destroy();

private:
	GLuint fbo;
	GLuint color_texture;
	GLuint depth_texture;
	int width;
	int height;
};