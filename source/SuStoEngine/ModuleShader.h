#ifndef __MODULE_SHADER_H__
#define __MODULE_SHADER_H__

#include <vector>
#include <map>

#include "Module.h"
#include "Globals.h"
#include "GeometryMath.h"

class ShaderProgramParameters;

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
	void SetProgramParameters(ShaderProgramParameters para);

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

class ShaderProgramParameters
{
public:
	ShaderProgramParameters();

	void SetFloat(const char* uniform_name, float value);
	void SetInt(const char* uniform_name, int value);
	void SetTexture(const char* uniform_name, int value);
	void SetVector2(const char* uniform_name, float2 value);
	void SetVector3(const char* uniform_name, float3 value);
	void SetVector4(const char* uniform_name, float4 value);

	std::map<std::string, float> GetFloatValues();
	std::map<std::string, int> GetIntValues();
	std::map<std::string, int> GetTextureValues();
	std::map<std::string, float2> GetVector2Values();
	std::map<std::string, float3> GetVector3Values();
	std::map<std::string, float4> GetVector4Values();

private:
	std::map<std::string, float> float_values;
	std::map<std::string, int> int_values;
	std::map<std::string, int> texture_values;
	std::map<std::string, float2> vector2_values;
	std::map<std::string, float3> vector3_values;
	std::map<std::string, float4> vector4_values;
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