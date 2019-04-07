#include "StaticTextRenderer.h"
#include "ComponentText.h"
#include "App.h"
#include "ModuleShader.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "ModuleSceneRenderer.h"
#include "ModuleText.h"

StaticTextRenderer::StaticTextRenderer()
{
}

StaticTextRenderer::~StaticTextRenderer()
{
}

void StaticTextRenderer::Start()
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
		uniform float z_pos; \
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
			gl_Position = Projection * View * Model * vec4(vec3(position.x, position.y, z_pos), 1);\
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
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, oUvs).r); \
				finalColor = oCol * sampled;\
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

void StaticTextRenderer::CleanUp()
{
}

void StaticTextRenderer::Render(const float4x4 & view, const float4x4 & projection)
{
	if (text_renderers.size() > 0)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LESS);

		program->UseProgram();

		App->renderer->BindVertexArrayBuffer(vao);

		App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
		App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

		std::vector<ComponentText*> texts = text_renderers;

		for (std::vector<ComponentText*>::iterator it = texts.begin(); it != texts.end(); ++it)
		{
			ComponentText* curr_text = (*it);
			Font* font = curr_text->GetCurrentFont();

			if (font != nullptr)
			{
				TextData text_data = curr_text->GetTextData();

				float scale = (float)15 / (float)text_data.GetFontSize();

				float4 colour = float4(1, 1, 1, 1);

				ComponentTransform* transform = curr_text->GetOwner()->transform;

				float z_pos = App->scene_renderer->layer_space_component_sprite.GetLayerValue(curr_text->GetLayer());

				std::vector<Glyph> glyphs = text_data.GetGlyphs();

				float curr_x = -text_data.GetFullSize().x * 0.5f * scale;
				float curr_y = -text_data.GetFullSize().y * 0.5f * scale;

				int counter = 0;
				for (std::vector<Glyph>::iterator gl = glyphs.begin(); gl != glyphs.end(); ++gl, ++counter)
				{
					float4x4 size_mat = float4x4::identity;

					float2 glyph_size = (*gl).GetSize();
					float2 bearing = (*gl).GetBearing();

					float2 final_size = glyph_size * scale;

					float2 pos = float2::zero;

					pos.x = (curr_x + (bearing.x * scale)) + (final_size.x * 0.5f);
					pos.y = (-(glyph_size.y - bearing.y) * scale) + (final_size.y * 0.5f) + curr_y;

					size_mat = float4x4::FromTRS(float3(pos.x, pos.y, 0), Quat::identity, float3(final_size.x, final_size.y, 1));

					float4x4 world_transform = transform->GetWorldTransform() * size_mat;

					world_transform[1][1] = -world_transform[1][1];

					App->renderer->SetUniformInt(program->GetID(), "hasTexture", !curr_text->GetRenderQuads());

					App->renderer->SetUniformFloat(program->GetID(), "z_pos", z_pos);

					App->renderer->SetUniformVec4(program->GetID(), "col", curr_text->GetColour());

					App->renderer->SetUniformMatrix(program->GetID(), "Model", world_transform.Transposed().ptr());

					App->renderer->BindTexture((*gl).GetTextureId());

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

					GLenum error = glGetError();
					if (error != GL_NO_ERROR)
					{
						INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
					}

					App->renderer->UnbindTexture();

					curr_x += (*gl).GetAdvance() * scale;
				}
			}
		}

		App->renderer->UnbindVertexArrayBuffer();

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}
}

void StaticTextRenderer::AddTextRenderer(ComponentText * add)
{
	if (add != nullptr)
	{
		bool exists = false;
		for (std::vector<ComponentText*>::iterator it = text_renderers.begin(); it != text_renderers.end(); ++it)
		{
			if ((*it) == add)
			{
				exists = true;
				break;
			}
		}

		if (!exists)
			text_renderers.push_back(add);
	}
}

void StaticTextRenderer::RemoveTextRenderer(ComponentText * remove)
{
	if (remove != nullptr)
	{
		for (std::vector<ComponentText*>::iterator it = text_renderers.begin(); it != text_renderers.end(); ++it)
		{
			if ((*it) == remove)
			{
				text_renderers.erase(it);
				break;
			}
		}
	}
}
