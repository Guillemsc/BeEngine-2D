#include "GridGuizmoRenderer.h"
#include "App.h"

GridGuizmoRenderer::GridGuizmoRenderer()
{
}

GridGuizmoRenderer::~GridGuizmoRenderer()
{
}

void GridGuizmoRenderer::Start()
{
	const char* vertex_code =
		"#version 330 core\n \
		layout(location = 0) in vec3 position;\n \
		layout(location = 1) in vec3 col;\n \
		layout(location = 2) in vec2 uv;\n \
		uniform mat4 Model; \
		uniform mat4 View; \
		uniform mat4 Projection; \
		out vec3 oColour;	\
		void main()\
		{\
			gl_Position = Projection * View * Model * vec4(position, 1);\
			oColour = col; \
		}";

	const char* fragment_code =
		"#version 330 core\n \
		in vec3 oColour;\
		out vec3 color; \
		void main()\
		{\
			color = oColour;\
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
	App->renderer->BindVertexArrayBuffer(vao);

	vbo = App->renderer->GenBuffer();
	App->renderer->BindArrayBuffer(vbo);

	VertexBuffer vb;
	vb.AddFloat3(float3(0, 0, 0));
	vb.AddFloat3(float3(1, 1, 1));
	vb.AddFloat2(float2(0, 0));

	vb.AddFloat3(float3(1, 0, 0));
	vb.AddFloat3(float3(1, 1, 1));
	vb.AddFloat2(float2(1, 0));

	vb.AddFloat3(float3(0, 1, 0));
	vb.AddFloat3(float3(1, 1, 1));
	vb.AddFloat2(float2(0, 1));

	vb.AddFloat3(float3(1, 0, 0));
	vb.AddFloat3(float3(1, 1, 1));
	vb.AddFloat2(float2(1, 0));

	vb.AddFloat3(float3(1, 1, 0));
	vb.AddFloat3(float3(1, 1, 1));
	vb.AddFloat2(float2(1, 1));

	vb.AddFloat3(float3(0, 1, 0));
	vb.AddFloat3(float3(1, 1, 1));
	vb.AddFloat2(float2(0, 1));

	App->renderer->LoadArrayToVRAM(vb.GetSize(), vb.GetBuffer(), GL_DYNAMIC_DRAW);
}

void GridGuizmoRenderer::CleanUp()
{
}

void GridGuizmoRenderer::Render(const float4x4 & view, const float4x4 & projection)
{
	App->renderer->BindArrayBuffer(vbo);

	float4x4 model = float4x4::FromTRS(float3::zero, Quat::identity, float3(100, 100, 1));

	program->UseProgram();

	GLint posAttrib = glGetAttribLocation(program->GetID(), "position");
	App->renderer->EnableVertexAttributeArray(posAttrib);
	App->renderer->SetVertexAttributePointer(posAttrib, 3, 8, 0);

	GLint posAttribCol = glGetAttribLocation(program->GetID(), "col");
	App->renderer->EnableVertexAttributeArray(posAttribCol);
	App->renderer->SetVertexAttributePointer(posAttribCol, 3, 8, 3);

	GLint posAttribUv = glGetAttribLocation(program->GetID(), "uv");
	App->renderer->EnableVertexAttributeArray(posAttribUv);
	App->renderer->SetVertexAttributePointer(posAttribUv, 2, 8, 6);

	App->renderer->SetUniformMatrix(program->GetID(), "Model", model.Transposed().ptr());
	App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
	App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
	}

	App->renderer->DisableVertexAttributeArray(posAttrib);
	App->renderer->DisableVertexAttributeArray(posAttribCol);
}

