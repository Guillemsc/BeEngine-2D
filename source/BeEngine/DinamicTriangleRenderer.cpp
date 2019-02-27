#include "DinamicTriangleRenderer.h"
#include "App.h"
#include "ModuleShader.h"

DinamicTriangleRenderer::DinamicTriangleRenderer()
{
}

DinamicTriangleRenderer::~DinamicTriangleRenderer()
{
}

void DinamicTriangleRenderer::Start()
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
			gl_FragColor  = oColour;\
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

	App->renderer->LoadArrayToVRAM(triangles_vb.GetSize(), triangles_vb.GetBuffer(), GL_DYNAMIC_DRAW);

	App->renderer->UnbindArraybuffer();
}

void DinamicTriangleRenderer::CleanUp()
{
	triangles_vb.Clear();
	triangles_count = 0;
}

void DinamicTriangleRenderer::Render(const float4x4 & view, const float4x4 & projection)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);

	App->renderer->BindArrayBuffer(vbo);
	App->renderer->LoadArrayToVRAM(triangles_vb.GetSize(), triangles_vb.GetBuffer(), GL_DYNAMIC_DRAW);

	float4x4 model = float4x4::identity;

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

	glDrawArrays(GL_TRIANGLES, 0, triangles_count * 3);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
	}

	App->renderer->DisableVertexAttributeArray(posAttrib);
	App->renderer->DisableVertexAttributeArray(posAttribCol);

	App->renderer->UnbindArraybuffer();

	triangles_vb.Clear();
	triangles_count = 0;

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void DinamicTriangleRenderer::DrawTriangle(const float2 & pos, const float2 & size, float rotation_degrees, const float3 & colour, float alpha)
{
	float2 half_size = float2(size.x * 0.5f, size.y * 0.5f);

	float rotation = rotation_degrees * DEGTORAD;

	float2 point1 = float2(pos.x - half_size.x, pos.y - half_size.y);
	float2 point2 = float2(pos.x + half_size.x, pos.y - half_size.y);
	float2 point3 = float2(pos.x, pos.y + half_size.y);

	float new_pos_1_x = ((point1.x - pos.x) * cos(rotation)) - ((point1.y - pos.y) * sin(rotation)) + pos.x;
	float new_pos_1_y = ((point1.x - pos.x) * sin(rotation)) + ((point1.y - pos.y) * cos(rotation)) + pos.y;

	float new_pos_2_x = ((point2.x - pos.x) * cos(rotation)) - ((point2.y - pos.y) * sin(rotation)) + pos.x;
	float new_pos_2_y = ((point2.x - pos.x) * sin(rotation)) + ((point2.y - pos.y) * cos(rotation)) + pos.y;

	float new_pos_3_x = ((point3.x - pos.x) * cos(rotation)) - ((point3.y - pos.y) * sin(rotation)) + pos.x;
	float new_pos_3_y = ((point3.x - pos.x) * sin(rotation)) + ((point3.y - pos.y) * cos(rotation)) + pos.y;

	float3 final_1 = float3(new_pos_1_x, new_pos_1_y, z_pos);
	float3 final_2 = float3(new_pos_2_x, new_pos_2_y, z_pos);
	float3 final_3 = float3(new_pos_3_x, new_pos_3_y, z_pos);

	triangles_vb.AddSpace(21);

	float4 full_colour = float4(colour.x, colour.y, alpha, colour.z);

	triangles_vb.AddFloat3(final_1);
	triangles_vb.AddFloat4(full_colour);
	triangles_vb.AddFloat3(final_2);
	triangles_vb.AddFloat4(full_colour);
	triangles_vb.AddFloat3(final_3);
	triangles_vb.AddFloat4(full_colour);

	++triangles_count;
}
