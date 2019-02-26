#include "QuadRenderer.h"
#include "App.h"
#include "ModuleShader.h"

QuadRenderer::QuadRenderer()
{
}

QuadRenderer::~QuadRenderer()
{
}

void QuadRenderer::Start()
{
	const char* vertex_code =
		"#version 330 core\n \
		layout(location = 0) in vec3 position; \n \
		layout(location = 1) in vec4 col; \n \
		uniform mat4 Model; \
		uniform mat4 View; \
		uniform mat4 Projection; \
		out vec4 oColour;	\
		void main()\
		{\
			oColour = col; \
			gl_Position = Projection * View * Model * vec4(position, 1);\
		}";

	const char* fragment_code =
		"#version 330 core\n \
		in vec4 oColour;\
		void main()\
		{\
			gl_FragColor = oColour;\
		}";


	Shader* vsh = App->shader->CreateShader(ShaderType::VERTEX);
	vsh->SetShaderCode(vertex_code);

	Shader* fsh = App->shader->CreateShader(ShaderType::FRAGMENT);
	fsh->SetShaderCode(fragment_code);

	program = App->shader->CreateShaderProgram();
	program->AddShader(vsh);
	program->AddShader(fsh);

	program->LinkProgram();

	vbo = App->renderer->GenBuffer();
	App->renderer->BindArrayBuffer(vbo);

	App->renderer->LoadArrayToVRAM(quads_vb.GetSize(), quads_vb.GetBuffer(), GL_DYNAMIC_DRAW);

	App->renderer->UnbindArraybuffer();
}

void QuadRenderer::CleanUp()
{
	quads_vb.Clear();
	quads_count = 0;
}

void QuadRenderer::Render(const float4x4 & view, const float4x4 & projection)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	App->renderer->BindArrayBuffer(vbo);
	App->renderer->LoadArrayToVRAM(quads_vb.GetSize(), quads_vb.GetBuffer(), GL_DYNAMIC_DRAW);

	float4x4 model = float4x4::identity;
	model[0][3] = 0;
	model[1][3] = 0;
	model[2][3] = 0;

	program->UseProgram();

	GLint posAttrib = App->renderer->GetVertexAttributeArray(program->GetID(), "position");
	App->renderer->EnableVertexAttributeArray(posAttrib);
	App->renderer->SetVertexAttributePointer(posAttrib, 3, 7, 0);

	GLint posAttribCol = App->renderer->GetVertexAttributeArray(program->GetID(), "col");
	App->renderer->EnableVertexAttributeArray(posAttribCol);
	App->renderer->SetVertexAttributePointer(posAttribCol, 4, 7, 3);

	App->renderer->SetUniformMatrix(program->GetID(), "Model", model.Transposed().ptr());
	App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
	App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

	glDrawArrays(GL_TRIANGLES, 0, quads_count * 6);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
	}

	App->renderer->DisableVertexAttributeArray(posAttrib);
	App->renderer->DisableVertexAttributeArray(posAttribCol);

	App->renderer->UnbindArraybuffer();

	quads_vb.Clear();
	quads_count = 0;

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}


void QuadRenderer::DrawQuad(const float2 & pos, const float2 & size, const float3 & colour, float alpha)
{
	float2 half_size = float2(size.x * 0.5f, size.y * 0.5f);

	float3 final_1 = float3(pos.x - half_size.x, pos.y - half_size.y, 0);
	float3 final_2 = float3(pos.x + half_size.x, pos.y - half_size.y, 0);
	float3 final_3 = float3(pos.x + half_size.x, pos.y + half_size.y, 0);
	float3 final_4 = float3(pos.x - half_size.x, pos.y + half_size.y, 0);

	quads_vb.AddSpace(42);

	float4 full_colour = float4(colour.x, colour.y, alpha, colour.z);

	quads_vb.AddFloat3(final_1);
	quads_vb.AddFloat4(full_colour);
	quads_vb.AddFloat3(final_2);
	quads_vb.AddFloat4(full_colour);
	quads_vb.AddFloat3(final_3);
	quads_vb.AddFloat4(full_colour);

	quads_vb.AddFloat3(final_1);
	quads_vb.AddFloat4(full_colour);
	quads_vb.AddFloat3(final_3);
	quads_vb.AddFloat4(full_colour);
	quads_vb.AddFloat3(final_4);
	quads_vb.AddFloat4(full_colour);

	++quads_count;
}
