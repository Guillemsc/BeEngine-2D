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

	for (std::vector<Shader*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
	{		
		(*it)->CleanUp();
		RELEASE(*it);
	}
	shaders.clear();

	for (std::vector<ShaderProgram*>::iterator it = programs.begin(); it != programs.end(); ++it)
	{		
		(*it)->CleanUp();
		RELEASE(*it);		
	}
	programs.clear();

	return ret;
}

Shader * ModuleShader::CreateShader(ShaderType type)
{
	Shader* shader = new Shader(type);

	shaders.push_back(shader);

	return shader;
}

void ModuleShader::DeleteShader(Shader * sh)
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

void ModuleShader::DeleteShaderProgram(ShaderProgram * sp)
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
		}

		UnlinkProgram();
	}
}

void ShaderProgram::RemoveShaders()
{
	vertex_shaders.clear();
	fragment_shaders.clear();

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

		if (all_compile)
		{
			id = App->renderer->CreateShaderProgram();

			for (std::vector<Shader*>::iterator it = vertex_shaders.begin(); it != vertex_shaders.end(); ++it)
			{
				App->renderer->AttachShaderToProgram(id, (*it)->GetID());
			}

			for (std::vector<Shader*>::iterator it = fragment_shaders.begin(); it != fragment_shaders.end(); ++it)
			{
				App->renderer->AttachShaderToProgram(id, (*it)->GetID());
			}

			linked = App->renderer->LinkProgram(id);

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
			case GL_FLOAT_VEC3:
			{
				std::map<std::string, float3> vector3_vals = para.GetVector3Values();

				float3 val = vector3_vals[uniform_name];

				for (std::map<std::string, float3>::iterator it = vector3_vals.begin(); it != vector3_vals.end(); ++it)
				{
					if (it->first.compare(uniform_name) == 0)
					{
						float3 val = vector3_vals[uniform_name];

						App->renderer->SetUniformVec3(id, uniform_name.c_str(), val);
					}
				}
			}
			break;

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

bool ShaderProgram::GetLinked() const
{
	return linked;
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

void ShaderProgramParameters::SetTexture(const char * uniform_name, int value)
{
	texture_values[uniform_name] = value;
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

std::map<std::string, float> ShaderProgramParameters::GetFloatValues()
{
	return float_values;
}

std::map<std::string, int> ShaderProgramParameters::GetIntValues()
{
	return int_values;
}

std::map<std::string, int> ShaderProgramParameters::GetTextureValues()
{
	return texture_values;
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

