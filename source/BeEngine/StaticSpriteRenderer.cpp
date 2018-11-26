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

	quad_vertex_buffer.AddFloat2(float2(-0.5f, -0.5f));
	quad_vertex_buffer.AddFloat2(float2(0.0f, 0.0f));

	quad_vertex_buffer.AddFloat2(float2(0.5f, -0.5f));
	quad_vertex_buffer.AddFloat2(float2(1.0f, 0.0f));

	quad_vertex_buffer.AddFloat2(float2(0.5f, 0.5f));
	quad_vertex_buffer.AddFloat2(float2(1.0f, 1.0f));

	quad_vertex_buffer.AddFloat2(float2(-0.5f, 0.5f));
	quad_vertex_buffer.AddFloat2(float2(0.0f, 1.0f));

	uint indices[] =
	{
		0, 1, 3,
		1, 2, 3,
	};

	quad_indices_buffer = indices;

	const char* vertex_code =
		"#version 330 core\n \
		layout(location = 0) in vec3 position; \n \
		layout(location = 1) in vec4 uvs; \n \
		uniform mat4 Model; \
		uniform mat4 View; \
		uniform mat4 Projection; \
		out vec4 oUvs; \
		void main()\
		{\
			oUvs = uvs;\
			gl_Position = Projection * View * Model * vec4(position, 1);\
		}";

	const char* fragment_code =
		"#version 330 core\n \
		uniform vec3 col; \
		uniform bool has_texture; \
		uniform sampler2D texture; \
		in vec4 oUvs; \
		void main()\
		{\
			if(has_texture == true)\
			{\
				gl_FragColor = texture2D(texture, oUvs);\
			}\
			else\
			{\
				gl_FragColor = col;\
			}\
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

	App->renderer->UnbindArraybuffer();
	App->renderer->UnbindVertexArrayBuffer();
}

void StaticSpriteRenderer::CleanUp()
{
}

void StaticSpriteRenderer::Render(const float4x4& view, const float4x4 & projection)
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	App->renderer->BindElementArrayBuffer(vao);

	program->UseProgram();

	GLint posAttrib = App->renderer->GetVertexAttributeArray(program->GetID(), "position");
	App->renderer->EnableVertexAttributeArray(posAttrib);
	App->renderer->SetVertexAttributePointer(posAttrib, 3, 5, 0);

	GLint posAttribCol = App->renderer->GetVertexAttributeArray(program->GetID(), "uvs");
	App->renderer->EnableVertexAttributeArray(posAttribCol);
	App->renderer->SetVertexAttributePointer(posAttribCol, 2, 5, 3);

	for (std::vector<ComponentSpriteRenderer*>::iterator it = sprite_renderers.begin(); it != sprite_renderers.end(); ++it)
	{
		ComponentSpriteRenderer* curr_sprite = (*it);

		ComponentTransform* transform = curr_sprite->GetOwner()->transform;

		ShaderProgramParameters par;
		par.SetVector3("col", float3(1.0f, 1.0f, 1.0f));
		par.SetBool("has_texture", true);
		par.SetTextures("texture", 0);
		program->SetProgramParameters(par);

		App->renderer->SetUniformMatrix(program->GetID(), "Model", transform->GetGlobalTransform().Transposed().ptr());
		App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
		App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

		App->renderer->BindTexture(curr_sprite->GetTextureId());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, quad_indices_buffer);

		App->renderer->UnbindTexture();
	}

	glDisable(GL_BLEND);
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
