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
	FRAGMENT,
	GEOMETRY,
};

class Shader
{
	friend class ModuleShader;

private:
	void operator delete(void *) {}

public:
	Shader(ShaderType type);

	void CleanUp();

	bool SetShaderCode(const char* code);
	const char* GetShaderCode() const;

	bool GetCompiles() const;
	const char* GetCompilationError();
	uint GetID() const;
	ShaderType GetType() const;

private:
	ShaderType type;
	std::string shader_code;
	bool compiles = false;
	std::string compilation_error;
	uint id = 0;
};

class ShaderProgram
{
	friend class ModuleShader;

private:
	void operator delete(void *) {}

public:
	ShaderProgram();

	void CleanUp();

	void AddShader(Shader* set);
	void RemoveShader(Shader* sh);
	void RemoveShaders();

	bool LinkProgram();
	void UseProgram();
	void SetProgramParameters(ShaderProgramParameters para);

	std::vector<Shader*> GetVertexShaders() const;
	std::vector<Shader*> GetFragmentShaders() const;
	std::vector<Shader*> GetGeometryShaders() const;

	bool GetLinked() const;
	const char* GetLinkError() const;

	uint GetID() const;

private:
	void UnlinkProgram();


private:
	uint id = 0;

	std::vector<Shader*> vertex_shaders;
	std::vector<Shader*> fragment_shaders;
	std::vector<Shader*> geometry_shaders;

	bool linked = false;
	std::string link_error;
};

class ShaderProgramParameters
{
public:
	ShaderProgramParameters();

	void SetFloat(const char* uniform_name, float value);
	void SetInt(const char* uniform_name, int value);
	void SetVector2(const char* uniform_name, float2 value);
	void SetVector3(const char* uniform_name, float3 value);
	void SetVector4(const char* uniform_name, float4 value);
	void SetBool(const char* uniform_name, bool value);
	void SetTextures(const char* uniform_name, uint texture_index);

	std::map<std::string, float> GetFloatValues();
	std::map<std::string, int> GetIntValues();
	std::map<std::string, float2> GetVector2Values();
	std::map<std::string, float3> GetVector3Values();
	std::map<std::string, float4> GetVector4Values();
	std::map<std::string, bool> GetBoolValues();
	std::map<std::string, uint> GetTextureValues();

private:
	std::map<std::string, float> float_values;
	std::map<std::string, int> int_values;
	std::map<std::string, float2> vector2_values;
	std::map<std::string, float3> vector3_values;
	std::map<std::string, float4> vector4_values;
	std::map<std::string, bool> bool_values;
	std::map<std::string, uint> textures_values;
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
	void DestroyShader(Shader* sh);

	ShaderProgram* CreateShaderProgram();
	void DestroyShaderProgram(ShaderProgram* sp);

private:
	void DestroyAllShaders();
	void DestroyAllShaderPrograms();

private:
	std::vector<Shader*> shaders;
	std::vector<ShaderProgram*> programs;
};

#endif // !__MODULE_SHADER_H__