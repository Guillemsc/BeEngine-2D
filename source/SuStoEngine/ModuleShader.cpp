#include "ModuleShader.h"
#include "App.h"
#include "ModuleRenderer3D.h"

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
					if ((*pro)->GetVertexShader() == sh)
						(*pro)->ClearVertexShader();

					if ((*pro)->GetFragmentShader() == sh)
						(*pro)->ClearFragmentShader();
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
		App->renderer3D->DeleteShader(id);
}

bool Shader::SetShaderCode(const char * code)
{
	bool ret = false;

	if (id > 0)
		App->renderer3D->DeleteShader(id);

	id = 0;
	compiles = false;

	switch (type)
	{
	case ShaderType::VERTEX:
		id = App->renderer3D->CreateVertexShader(code, compilation_error);
		break;
	case ShaderType::FRAGMENT:
		id = App->renderer3D->CreateFragmentShader(code, compilation_error);
		break;
	}

	if (id > 0)
		compiles = true;

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

ShaderProgram::ShaderProgram()
{
}

void ShaderProgram::CleanUp()
{
	if (id > 0)
		App->renderer3D->DeleteProgram(id);
}

void ShaderProgram::SetVertexShader(Shader * set)
{
	if (set != nullptr)
	{
		vertex_shader = set;
	}
}

void ShaderProgram::SetFragmentShader(Shader * set)
{
	if (set != nullptr)
	{
		fragment_shader = set;
	}
}

void ShaderProgram::ClearVertexShader()
{
	vertex_shader = nullptr;

	LinkProgram();
}

void ShaderProgram::ClearFragmentShader()
{
	fragment_shader = nullptr;

	LinkProgram();
}

bool ShaderProgram::LinkProgram()
{
	bool ret = false;

	if (id > 0)
		App->renderer3D->DeleteProgram(id);

	id = 0;
	linked = false;

	if (vertex_shader != nullptr && fragment_shader != nullptr)
	{
		if (vertex_shader->GetCompiles() && fragment_shader->GetCompiles())
		{
			id = App->renderer3D->CreateShaderProgram();

			App->renderer3D->AttachShaderToProgram(id, vertex_shader->GetID());
			App->renderer3D->AttachShaderToProgram(id, fragment_shader->GetID());

			linked = App->renderer3D->LinkProgram(id);

			if (linked)
				ret = true;
		}
	}

	return ret;
}

void ShaderProgram::UseProgram()
{
	if(id > 0 && linked)
		App->renderer3D->UseShaderProgram(id);
}

Shader* ShaderProgram::GetVertexShader() const
{
	return vertex_shader;
}

Shader* ShaderProgram::GetFragmentShader() const
{
	return fragment_shader;
}

bool ShaderProgram::GetLinked() const
{
	return linked;
}

uint ShaderProgram::GetID() const
{
	return id;
}
