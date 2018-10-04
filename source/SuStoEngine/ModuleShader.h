#ifndef __MODULE_SHADER_H__
#define __MODULE_SHADER_H__

#include <vector>

#include "Module.h"
#include "Globals.h"

enum ShaderType
{
	VERTEX,
	FRAGMENT
};

class Shader
{
public:
	Shader(ShaderType type);

	void CleanUp();

	bool SetShaderCode(const char* code);
	const char* GetShaderCode() const;

	bool GetCompiles() const;
	const char* GetCompilationError();
	uint GetID() const;

private:
	ShaderType type;
	std::string shader_code;
	bool compiles = false;
	std::string compilation_error;
	uint id = 0;
};

class ShaderProgram
{
public:
	ShaderProgram();

	void CleanUp();

	void SetVertexShader(Shader* set);
	void SetFragmentShader(Shader* set);

	void ClearVertexShader();
	void ClearFragmentShader();

	bool LinkProgram();
	void UseProgram();

	Shader* GetVertexShader() const;
	Shader* GetFragmentShader() const;

	bool GetLinked() const;
	uint GetID() const;

private:
	uint id = 0;

	Shader* vertex_shader = nullptr;
	Shader* fragment_shader = nullptr;

	bool linked = false;
};

class ModuleShader : public Module
{
public:
	ModuleShader();
	~ModuleShader();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	Shader* CreateShader(ShaderType type);
	void DeleteShader(Shader* sh);

	ShaderProgram* CreateShaderProgram();
	void DeleteShaderProgram(ShaderProgram* sp);

private:
	std::vector<Shader*> shaders;
	std::vector<ShaderProgram*> programs;
};

#endif // !__MODULE_SHADER_H__