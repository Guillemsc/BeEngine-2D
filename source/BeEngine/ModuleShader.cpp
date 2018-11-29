#include "ModuleShader.h"
#include "App.h"
#include "ModuleRenderer.h"

ModuleShader::ModuleShader() : Module()
{
}

ModuleShader::~ModuleShader()
{
}

bool ModuleShader::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleShader::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleShader::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleShader::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleShader::CleanUp()
{
	bool ret = true;

	DestroyAllShaders();
	DestroyAllShaderPrograms();

	return ret;
}

Shader * ModuleShader::CreateShader(ShaderType type)
{
	Shader* shader = new Shader(type);

	shaders.push_back(shader);

	return shader;
}

void ModuleShader::DestroyShader(Shader * sh)
{
	if (sh != nullptr)
	{
		for (std::vector<Shader*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
		{
			if ((*it) == sh)
			{
				for (std::vector<ShaderProgram*>::iterator pro = programs.begin(); pro != programs.end(); ++pro)
				{
					(*pro)->RemoveShader((*it));
				}

				(*it)->CleanUp();
				RELEASE(*it);
				shaders.erase(it);
				break;
			}
		}
	}
}

ShaderProgram * ModuleShader::CreateShaderProgram()
{
	ShaderProgram* program = new ShaderProgram();

	programs.push_back(program);

	return program;
}

void ModuleShader::DestroyShaderProgram(ShaderProgram * sp)
{
	if (sp != nullptr)
	{
		for (std::vector<ShaderProgram*>::iterator it = programs.begin(); it != programs.end(); ++it)
		{
			if ((*it) == sp)
			{
				(*it)->CleanUp();

				RELEASE(*it);
				programs.erase(it);
				break;
			}
		}
	}
}

void ModuleShader::DestroyAllShaders()
{
	for (std::vector<Shader*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}
	shaders.clear();
}

void ModuleShader::DestroyAllShaderPrograms()
{
	for (std::vector<ShaderProgram*>::iterator it = programs.begin(); it != programs.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}
	programs.clear();
}

Shader::Shader(ShaderType _type)
{
	type = _type;
}

void Shader::CleanUp()
{
	if (id > 0)
		App->renderer->DeleteShader(id);
}

bool Shader::SetShaderCode(const char * code)
{
	bool ret = false;

	if (id > 0)
		App->renderer->DeleteShader(id);

	id = 0;
	compiles = false;

	switch (type)
	{
	case ShaderType::VERTEX:
		id = App->renderer->CreateVertexShader(code, compilation_error);
		break;
	case ShaderType::FRAGMENT:
		id = App->renderer->CreateFragmentShader(code, compilation_error);
		break;
	case ShaderType::GEOMETRY:
		id = App->renderer->CreateGeometryShader(code, compilation_error);
	}

	if (id > 0)
	{
		compiles = true;
		ret = true;
	}

	return ret;
}

const char * Shader::GetShaderCode() const
{
	return shader_code.c_str();
}

bool Shader::GetCompiles() const
{
	return compiles;
}

const char * Shader::GetCompilationError()
{
	return compilation_error.c_str();
}

uint Shader::GetID() const
{
	return id;
}

ShaderType Shader::GetType() const
{
	return type;
}

ShaderProgram::ShaderProgram()
{
}

void ShaderProgram::CleanUp()
{
	if (id > 0)
		App->renderer->DeleteProgram(id);
}

void ShaderProgram::AddShader(Shader * set)
{
	if (set != nullptr)
	{
		switch (set->GetType())
		{
		case ShaderType::VERTEX:
			vertex_shaders.push_back(set);
			break;
		case ShaderType::FRAGMENT:
			fragment_shaders.push_back(set);
			break;
		case ShaderType::GEOMETRY:
			geometry_shaders.push_back(set);
			break;
		}

		UnlinkProgram();
	}
}

void ShaderProgram::RemoveShader(Shader * sh)
{
	if (sh != nullptr)
	{
		switch (sh->GetType())
		{
		case ShaderType::VERTEX:
			for (std::vector<Shader*>::iterator it = vertex_shaders.begin(); it != vertex_shaders.end(); ++it)
			{
				if ((*it) == sh)
				{
					vertex_shaders.erase(it);
					break;
				}
			}
			break;
		case ShaderType::FRAGMENT:
			for (std::vector<Shader*>::iterator it = fragment_shaders.begin(); it != fragment_shaders.end(); ++it)
			{
				if ((*it) == sh)
				{
					fragment_shaders.erase(it);
					break;
				}
			}
			break;
		case ShaderType::GEOMETRY:
			for (std::vector<Shader*>::iterator it = geometry_shaders.begin(); it != geometry_shaders.end(); ++it)
			{
				if ((*it) == sh)
				{
					geometry_shaders.erase(it);
					break;
				}
			}
			break;
		}

		UnlinkProgram();
	}
}

void ShaderProgram::RemoveShaders()
{
	vertex_shaders.clear();
	fragment_shaders.clear();
	geometry_shaders.clear();

	UnlinkProgram();
}

bool ShaderProgram::LinkProgram()
{
	bool ret = false;

	UnlinkProgram();

	if (vertex_shaders.size() > 0 && fragment_shaders.size() > 0)
	{
		bool all_compile = true;

		for (std::vector<Shader*>::iterator it = vertex_shaders.begin(); it != vertex_shaders.end(); ++it)
		{
			if (!(*it)->GetCompiles())
			{
				all_compile = false;
				break;
			}
		}

		for (std::vector<Shader*>::iterator it = fragment_shaders.begin(); it != fragment_shaders.end(); ++it)
		{
			if (!(*it)->GetCompiles())
			{
				all_compile = false;
				break;
			}
		}

		for (std::vector<Shader*>::iterator it = geometry_shaders.begin(); it != geometry_shaders.end(); ++it)
		{
			if (!(*it)->GetCompiles())
			{
				all_compile = false;
				break;
			}
		}

		if (all_compile)
		{
			id = App->renderer->CreateShaderProgram();

			for (std::vector<Shader*>::iterator it = vertex_shaders.begin(); it != vertex_shaders.end(); ++it)
			{
				App->renderer->AttachShaderToProgram(id, (*it)->GetID());
			}

			for (std::vector<Shader*>::iterator it = geometry_shaders.begin(); it != geometry_shaders.end(); ++it)
			{
				App->renderer->AttachShaderToProgram(id, (*it)->GetID());
			}

			for (std::vector<Shader*>::iterator it = fragment_shaders.begin(); it != fragment_shaders.end(); ++it)
			{
				App->renderer->AttachShaderToProgram(id, (*it)->GetID());
			}

			linked = App->renderer->LinkProgram(id, link_error);

			if (linked)
				ret = true;
		}
	}

	return ret;
}

void ShaderProgram::UseProgram()
{
	if (id > 0 && linked)
	{
		App->renderer->UseShaderProgram(id);
	}
}

void ShaderProgram::SetProgramParameters(ShaderProgramParameters para)
{
	if (id > 0 && linked)
	{
		int count = App->renderer->GetUniformsCount(id);

		for (int i = 0; i < count; ++i)
		{
			std::string uniform_name;
			GLenum uniform_type;

			App->renderer->GetUniformInfo(id, i, uniform_name, uniform_type);

			switch (uniform_type)
			{
			case GL_INT:
			{
				std::map<std::string, int> int_vals = para.GetIntValues();

				for (std::map<std::string, int>::iterator it = int_vals.begin(); it != int_vals.end(); ++it)
				{
					if (it->first.compare(uniform_name) == 0)
					{
						int val = int_vals[uniform_name];

						App->renderer->SetUniformInt(id, uniform_name.c_str(), val);
					}
				}

				break;
			}
			case GL_FLOAT_VEC3:
			{
				std::map<std::string, float3> vector3_vals = para.GetVector3Values();

				for (std::map<std::string, float3>::iterator it = vector3_vals.begin(); it != vector3_vals.end(); ++it)
				{
					if (it->first.compare(uniform_name) == 0)
					{
						float3 val = vector3_vals[uniform_name];

						App->renderer->SetUniformVec3(id, uniform_name.c_str(), val);
					}
				}

				break;
			}
			case GL_FLOAT_VEC4:
			{
				std::map<std::string, float4> vector4_vals = para.GetVector4Values();

				for (std::map<std::string, float4>::iterator it = vector4_vals.begin(); it != vector4_vals.end(); ++it)
				{
					if (it->first.compare(uniform_name) == 0)
					{
						float4 val = vector4_vals[uniform_name];

						App->renderer->SetUniformVec4(id, uniform_name.c_str(), val);
					}
				}

				break;
			}
			case GL_BOOL:
			{
				std::map<std::string, bool> bool_vals = para.GetBoolValues();

				for (std::map<std::string, bool>::iterator it = bool_vals.begin(); it != bool_vals.end(); ++it)
				{
					if (it->first.compare(uniform_name) == 0)
					{
						bool val = bool_vals[uniform_name];

						App->renderer->SetUniformBool(id, uniform_name.c_str(), val);
					}
				}
				break;
			}
			case GL_SAMPLER_2D:
			{
				std::map<std::string, uint> text_vals = para.GetTextureValues();

				for (std::map<std::string, uint>::iterator it = text_vals.begin(); it != text_vals.end(); ++it)
				{
					if (it->first.compare(uniform_name) == 0)
					{
						uint index = text_vals[uniform_name];

						App->renderer->SetUniformInt(id, uniform_name.c_str(), index);
					}
				}
				break;
			}

			default:
				break;
			}
		}
	}
}

std::vector<Shader*> ShaderProgram::GetVertexShaders() const
{
	return vertex_shaders;
}

std::vector<Shader*> ShaderProgram::GetFragmentShaders() const
{
	return fragment_shaders;
}

std::vector<Shader*> ShaderProgram::GetGeometryShaders() const
{
	return geometry_shaders;
}

bool ShaderProgram::GetLinked() const
{
	return linked;
}

const char * ShaderProgram::GetLinkError() const
{
	return link_error.c_str();
}

uint ShaderProgram::GetID() const
{
	return id;
}

void ShaderProgram::UnlinkProgram()
{
	if (id > 0)
		App->renderer->DeleteProgram(id);

	id = 0;
	linked = false;
}

ShaderProgramParameters::ShaderProgramParameters()
{
}

void ShaderProgramParameters::SetFloat(const char * uniform_name, float value)
{
	float_values[uniform_name] = value;
}

void ShaderProgramParameters::SetInt(const char * uniform_name, int value)
{
	int_values[uniform_name] = value;
}

void ShaderProgramParameters::SetVector2(const char * uniform_name, float2 value)
{
	vector2_values[uniform_name] = value;
}

void ShaderProgramParameters::SetVector3(const char * uniform_name, float3 value)
{
	vector3_values[uniform_name] = value;
}

void ShaderProgramParameters::SetVector4(const char * uniform_name, float4 value)
{
	vector4_values[uniform_name] = value;
}

void ShaderProgramParameters::SetBool(const char * uniform_name, bool value)
{
	bool_values[uniform_name] = value;
}

void ShaderProgramParameters::SetTextures(const char * uniform_name, uint texture_index)
{
	textures_values[uniform_name] = texture_index;
}

std::map<std::string, float> ShaderProgramParameters::GetFloatValues()
{
	return float_values;
}

std::map<std::string, int> ShaderProgramParameters::GetIntValues()
{
	return int_values;
}

std::map<std::string, float2> ShaderProgramParameters::GetVector2Values()
{
	return vector2_values;
}

std::map<std::string, float3> ShaderProgramParameters::GetVector3Values()
{
	return vector3_values;
}

std::map<std::string, float4> ShaderProgramParameters::GetVector4Values()
{
	return vector4_values;
}

std::map<std::string, bool> ShaderProgramParameters::GetBoolValues()
{
	return bool_values;
}

std::map<std::string, uint> ShaderProgramParameters::GetTextureValues()
{
	return textures_values;
}

