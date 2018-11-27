#include "StaticSpriteRenderer.h"
#include "App.h"
#include "ModuleShader.h"
#include "ComponentTransfrom.h"
#include "GameObject.h"

StaticSpriteRenderer::StaticSpriteRenderer()
{
}

StaticSpriteRenderer::~StaticSpriteRenderer()
{
}

void StaticSpriteRenderer::Start()
{
	VertexBuffer quad_vertex_buffer;

	quad_vertex_buffer.AddFloat3(float3(-0.5f, -0.5f, 0));
	//quad_vertex_buffer.AddFloat2(float2(0.0f, 0.0f));

	quad_vertex_buffer.AddFloat3(float3(0.5f, -0.5f, 0));
	//quad_vertex_buffer.AddFloat2(float2(1.0f, 0.0f));

	quad_vertex_buffer.AddFloat3(float3(0.5f, 0.5f, 0));
	//quad_vertex_buffer.AddFloat2(float2(1.0f, 1.0f));

	//quad_vertex_buffer.AddFloat3(float3(-0.5f, 0.5f, 0));
	//quad_vertex_buffer.AddFloat2(float2(0.0f, 1.0f));

	uint indices[] =
	{
		0, 1, 3,
		1, 2, 3,
	};

	quad_indices_buffer = indices;

	const char* vertex_code =
		"#version 330 core\n \
		layout(location = 0) in vec3 position; \n \
		\
		uniform mat4 Model; \
		uniform mat4 View; \
		uniform mat4 Projection; \
		\
		uniform vec3 col; \
		out vec3 oCol; \
		out vec2 oUvs; \
		\
		void main()\
		{\
			oCol = col;\
			gl_Position = Projection * View * Model * vec4(position, 1);\
		}";

	const char* fragment_code =
		"#version 330 core\n \
		in vec3 oCol; \
		out vec3 finalColor; \
		void main()\
		{\
			finalColor = oCol;\
		}";

	Shader* vsh = App->shader->CreateShader(ShaderType::VERTEX);
	vsh->SetShaderCode(vertex_code);

	Shader* fsh = App->shader->CreateShader(ShaderType::FRAGMENT);
	fsh->SetShaderCode(fragment_code);

	program = App->shader->CreateShaderProgram();
	program->AddShader(vsh);
	program->AddShader(fsh);

	program->LinkProgram();

	vao = App->renderer->GenVertexArrayBuffer();
	vbo = App->renderer->GenBuffer();

	App->renderer->BindVertexArrayBuffer(vao);
	App->renderer->BindArrayBuffer(vbo);

	App->renderer->LoadArrayToVRAM(quad_vertex_buffer.GetSize(), quad_vertex_buffer.GetBuffer(), GL_STATIC_DRAW);

	GLint posAttrib = App->renderer->GetVertexAttributeArray(program->GetID(), "position");
	App->renderer->EnableVertexAttributeArray(posAttrib);
	App->renderer->SetVertexAttributePointer(posAttrib, 3, 3, 0);

	//GLint posAttribCol = App->renderer->GetVertexAttributeArray(program->GetID(), "uvs");
	//App->renderer->EnableVertexAttributeArray(posAttribCol);
	//App->renderer->SetVertexAttributePointer(posAttribCol, 2, 5, 3);

	App->renderer->UnbindArraybuffer();
	App->renderer->UnbindVertexArrayBuffer();
}

void StaticSpriteRenderer::CleanUp()
{
}

void StaticSpriteRenderer::Render(const float4x4& view, const float4x4 & projection)
{
	if (sprite_renderers.size() > 0)
	{
		//glEnable(GL_BLEND);
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		program->UseProgram();

		App->renderer->BindVertexArrayBuffer(vao);

		for (std::vector<ComponentSpriteRenderer*>::iterator it = sprite_renderers.begin(); it != sprite_renderers.end(); ++it)
		{
			ComponentSpriteRenderer* curr_sprite = (*it);

			ComponentTransform* transform = curr_sprite->GetOwner()->transform;

			ShaderProgramParameters par;
			par.SetVector3("col", float3(1.0f, 1.0f, 1.0f));
			//par.SetTextures("texture", 0);
			program->SetProgramParameters(par);

			float4x4 model = float4x4::FromTRS(float3(0, 0, 0), Quat::identity, float3(100, 100, 1));

			App->renderer->SetUniformMatrix(program->GetID(), "Model", model.Transposed().ptr());
			App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
			App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

			//App->renderer->BindTexture(curr_sprite->GetTextureId());

			glDrawArrays(GL_TRIANGLES, 0, 3);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
			}
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			//App->renderer->UnbindTexture();
		}

		App->renderer->UnbindElementArrayBuffer();

		//glDisable(GL_BLEND);
	}
}

void StaticSpriteRenderer::AddSpriteRenderer(ComponentSpriteRenderer * add)
{
	bool exists = false;
	for (std::vector<ComponentSpriteRenderer*>::iterator it = sprite_renderers.begin(); it != sprite_renderers.end(); ++it)
	{
		if ((*it) == add)
		{
			exists = true;
			break;
		}
	}

	if(!exists)
		sprite_renderers.push_back(add);
}

void StaticSpriteRenderer::RemoveSpriteRenderer(ComponentSpriteRenderer * remove)
{
	for (std::vector<ComponentSpriteRenderer*>::iterator it = sprite_renderers.begin(); it != sprite_renderers.end(); ++it)
	{
		if ((*it) == remove)
		{
			sprite_renderers.erase(it);
			break;
		}
	}
}
