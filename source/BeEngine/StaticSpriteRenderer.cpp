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
	quad_vertex_buffer.AddFloat2(float2(0.0f, 0.0f));

	quad_vertex_buffer.AddFloat3(float3(0.5f, -0.5f, 0));
	quad_vertex_buffer.AddFloat2(float2(1.0f, 0.0f));

	quad_vertex_buffer.AddFloat3(float3(0.5f, 0.5f, 0));
	quad_vertex_buffer.AddFloat2(float2(1.0f, 1.0f));

	quad_vertex_buffer.AddFloat3(float3(-0.5f, 0.5f, 0));
	quad_vertex_buffer.AddFloat2(float2(0.0f, 1.0f));

	uint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	const char* vertex_code =
		"#version 330 core\n \
		layout(location = 0) in vec3 position; \n \
		layout(location = 1) in vec2 uvs; \n \
		\
		uniform mat4 Model; \
		uniform mat4 View; \
		uniform mat4 Projection; \
		\
		uniform vec4 col; \
		uniform int hasTexture; \
		out vec4 oCol; \
		flat out int oHasTexture; \
		out vec2 oUvs; \
		\
		void main()\
		{\
			oCol = col;\
			oHasTexture = hasTexture;\
			oUvs = uvs; \
			gl_Position = Projection * View * Model * vec4(position, 1);\
		}";

	const char* fragment_code =
		"#version 330 core\n \
		uniform sampler2D tex; \
		in vec4 oCol; \
		flat in int oHasTexture; \
		in vec2 oUvs; \
		out vec4 finalColor; \
		void main()\
		{\
			if(oHasTexture == 1)\
			{\
				finalColor = texture(tex, oUvs);\
			}\
			else\
			{\
				finalColor = oCol;\
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

	// VAO
	vao = App->renderer->GenVertexArrayBuffer();
	App->renderer->BindVertexArrayBuffer(vao);

	// VBO
	uint vbo = App->renderer->GenBuffer();

	App->renderer->BindArrayBuffer(vbo);

	App->renderer->LoadArrayToVRAM(quad_vertex_buffer.GetSize(), quad_vertex_buffer.GetBuffer(), GL_STATIC_DRAW);

	// Set info
	GLint posAttrib = App->renderer->GetVertexAttributeArray(program->GetID(), "position");
	App->renderer->EnableVertexAttributeArray(posAttrib);
	App->renderer->SetVertexAttributePointer(posAttrib, 3, 5, 0);

	GLint uvsAttrib = App->renderer->GetVertexAttributeArray(program->GetID(), "uvs");
	App->renderer->EnableVertexAttributeArray(uvsAttrib);
	App->renderer->SetVertexAttributePointer(uvsAttrib, 2, 5, 3);

	// VBIO
	uint vbio = App->renderer->GenBuffer();

	App->renderer->BindElementArrayBuffer(vbio);

	App->renderer->LoadElementArrayToVRAM(sizeof(indices), &indices[0], GL_STATIC_DRAW);


	// Clear
	App->renderer->UnbindVertexArrayBuffer();

	App->renderer->DisableVertexAttributeArray(posAttrib);
	App->renderer->DisableVertexAttributeArray(uvsAttrib);

	quad_vertex_buffer.Clear();
}

void StaticSpriteRenderer::CleanUp()
{
}

void StaticSpriteRenderer::Render(const float4x4& view, const float4x4 & projection)
{
	if (sprite_renderers.size() > 0)
	{
		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_BLEND);
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		program->UseProgram();

		App->renderer->BindVertexArrayBuffer(vao);

		App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
		App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

		std::vector<ComponentSpriteRenderer*> sprites = sprite_renderers;

		for (std::vector<ComponentSpriteRenderer*>::iterator it = sprites.begin(); it != sprites.end(); ++it)
		{
			ComponentSpriteRenderer* curr_sprite = (*it);

			ComponentTransform* transform = curr_sprite->GetOwner()->transform;

			App->renderer->SetUniformVec4(program->GetID(), "col", float4(1.0f, 0.0f, 1.0f, 1.0f));
			App->renderer->SetUniformInt(program->GetID(), "hasTexture", curr_sprite->GetHasTexture());

			//ShaderProgramParameters par;
			//par.SetVector4("col", float4(1.0f, 0.0f, 1.0f, 1.0f));
			//par.SetInt("hasTexture", curr_sprite->GetHasTexture());
			////par.SetTextures("texture", 0);
			//program->SetProgramParameters(par);

			App->renderer->SetUniformMatrix(program->GetID(), "Model", transform->GetWorldTransform().Transposed().ptr());

			if(curr_sprite->GetHasTexture())
				App->renderer->BindTexture(curr_sprite->GetTextureId());

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
			}
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			//App->renderer->UnbindTexture();
		}

		App->renderer->UnbindVertexArrayBuffer();

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
