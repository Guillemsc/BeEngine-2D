#include "LineRenderer.h"
#include "App.h"
#include "VertexBuffer.h"
#include "Functions.h"

LineRenderer::LineRenderer()
{
}

LineRenderer::~LineRenderer()
{
}

void LineRenderer::Start()
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

	App->renderer->LoadArrayToVRAM(lines_vb.GetSize(), lines_vb.GetBuffer(), GL_DYNAMIC_DRAW);

	App->renderer->UnbindArraybuffer();
}

void LineRenderer::CleanUp()
{
	lines_vb.Clear();
	lines_count = 0;
}

void LineRenderer::Render(const float4x4& view, const float4x4& projection)
{
	//glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_ADD);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	program->UseProgram();

	App->renderer->BindArrayBuffer(vbo);
	App->renderer->LoadArrayToVRAM(lines_vb.GetSize(), lines_vb.GetBuffer(), GL_DYNAMIC_DRAW);

	float4x4 model = float4x4::identity;
	model[0][3] = 0;
	model[1][3] = 0;
	model[2][3] = 0;

	//ShaderProgramParameters par;
	//par.SetVector3("Colour", float3(1.0f, 1.0f, 1.0f));
	//program->SetProgramParameters(par);

	GLint posAttrib = App->renderer->GetVertexAttributeArray(program->GetID(), "position");
	App->renderer->EnableVertexAttributeArray(posAttrib);
	App->renderer->SetVertexAttributePointer(posAttrib, 3, 7, 0);

	GLint posAttribCol = App->renderer->GetVertexAttributeArray(program->GetID(), "col");
	App->renderer->EnableVertexAttributeArray(posAttribCol);
	App->renderer->SetVertexAttributePointer(posAttribCol, 4, 7, 3);

	App->renderer->SetUniformMatrix(program->GetID(), "Model", model.Transposed().ptr());
	App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
	App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

	glDrawArrays(GL_TRIANGLES, 0, lines_count * 6);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
	}

	App->renderer->DisableVertexAttributeArray(posAttrib);
	App->renderer->DisableVertexAttributeArray(posAttribCol);

	App->renderer->UnbindArraybuffer();

	lines_vb.Clear();
	lines_count = 0;

	//glDisable(GL_BLEND);
}

void LineRenderer::DrawLine(const float2& start, const float2& end, const float3& colour, float alpha, float tickness)
{
	float delta_y = end.y - start.y;
	float delta_x = end.x - start.x;

	float angle1 = atan2(delta_y, delta_x);
	float angle2 = angle1 + (180.0f * DEGTORAD);

	float half_width = tickness * 0.5;

	float new_p1_1_x = start.x - (sin(angle1) * half_width);
	float new_p1_1_y = start.y + (cos(angle1) * half_width);

	float new_p1_2_x = start.x - (sin(angle2) * half_width);
	float new_p1_2_y = start.y + (cos(angle2) * half_width);

	float new_p2_1_x = end.x - (sin(angle1) * half_width);
	float new_p2_1_y = end.y + (cos(angle1) * half_width);

	float new_p2_2_x = end.x - (sin(angle2) * half_width);
	float new_p2_2_y = end.y + (cos(angle2) * half_width);

	float3 final_1 = float3(new_p1_2_x, new_p1_2_y, z_layer);

	float3 final_2 = float3(new_p2_2_x, new_p2_2_y, z_layer);

	float3 final_3 = float3(new_p2_1_x, new_p2_1_y, z_layer);

	float3 final_4 = float3(new_p1_1_x, new_p1_1_y, z_layer);

	lines_vb.AddSpace(42);

	float4 full_colour = float4(colour.x, colour.y, alpha, colour.z);

	lines_vb.AddFloat3(final_2);
	lines_vb.AddFloat4(full_colour);
	lines_vb.AddFloat3(final_4);
	lines_vb.AddFloat4(full_colour);
	lines_vb.AddFloat3(final_3);
	lines_vb.AddFloat4(full_colour);

	lines_vb.AddFloat3(final_2);
	lines_vb.AddFloat4(full_colour);
	lines_vb.AddFloat3(final_4);
	lines_vb.AddFloat4(full_colour);
	lines_vb.AddFloat3(final_1);
	lines_vb.AddFloat4(full_colour);

	++lines_count;
}

void LineRenderer::DrawDashedLine(const float2 & start, const float2 & end, const float3 & colour, float alpha, float dash_lenght, float tickness)
{
}

void LineRenderer::SetZLayer(uint set)
{
	z_layer = set;
}

void LineRenderer::ResetZLayer()
{
	z_layer = 0;
}
