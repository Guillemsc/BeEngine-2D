#include "DynamicGridRenderer.h"
#include "ModuleShader.h"
#include "App.h"
#include "ModuleSceneRenderer.h"

DynamicGridRenderer::DynamicGridRenderer()
{
}

DynamicGridRenderer::~DynamicGridRenderer()
{
}

void DynamicGridRenderer::Start()
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
		out vec4 oCol; \
		out vec2 oUvs; \
		\
		void main()\
		{\
			oCol = col;\
			oUvs = uvs; \
			gl_Position = Projection * View * Model * vec4(vec3(position.x, position.y, z_pos), 1);\
		}";

	const char* fragment_code =
		"#version 330 core\n \
		in vec4 oCol; \
		in vec2 oUvs; \
		out vec4 finalColor; \
		\
		float grid(vec2 st, float res) \
		{\
			vec2 grid = fract(st*res); \
			return (step(res, grid.x) * step(res, grid.y)); \
		}\
		\
		void main()\
		{\
			vec2 coord = oUvs.xy * 10000;\
			\
			vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);\
			float line = min(grid.x, grid.y);\
			\
			float alpha = 0.6;\
			\
			if(1.0 - min(line, 1.0) < 0.7)\
			{\
				alpha = 0;\
			}\
			\
			vec3 col = vec3(0.8, 0.8, 0.8);\
			finalColor = vec4(col.x, col.y, col.z, 1 - min(line, 1.0)); \
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

void DynamicGridRenderer::CleanUp()
{
}

void DynamicGridRenderer::Render(const float4x4 & view, const float4x4 & projection)
{
	if (needs_to_render)
	{
		needs_to_render = false;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LESS);

		program->UseProgram();

		App->renderer->BindVertexArrayBuffer(vao);

		App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
		App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

		float4x4 world_transform = float4x4::FromTRS(float3::zero, Quat::identity, float3(99999, 99999, 1));

		float z_pos = App->scene_renderer->layer_space_grid.GetLayerValue(0);

		App->renderer->SetUniformFloat(program->GetID(), "z_pos", z_pos);

		App->renderer->SetUniformVec4(program->GetID(), "col", float4(1.0f, 0.0f, 1.0f, 1.0f));

		App->renderer->SetUniformMatrix(program->GetID(), "Model", world_transform.Transposed().ptr());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
		}

		App->renderer->UnbindVertexArrayBuffer();

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}
}

void DynamicGridRenderer::DrawGrid()
{
	needs_to_render = true;
}
