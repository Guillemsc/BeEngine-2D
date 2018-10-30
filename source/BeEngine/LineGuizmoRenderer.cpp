#include "LineGuizmoRenderer.h"
#include "App.h"
#include "VertexBuffer.h"

LineGuizmoRenderer::LineGuizmoRenderer()
{
}

LineGuizmoRenderer::~LineGuizmoRenderer()
{
}

void LineGuizmoRenderer::Start()
{
	const char* user_vertex_code =
		"#version 330 core\n \
		layout(location = 1) in vec3 col;\n \
		out vec3 oColour;	\
		uniform vec3 Colour;\
		void UserShader()\
		{\
			oColour = col;\
		}";

	const char* user_fragment_code =
		"#version 330 core\n \
		in vec3 oColour;\
		out vec3 color; \
		void UserShader()\
		{\
			color = oColour;\
		}";

	const char* vertex_code =
		"#version 330 core\n \
		layout(location = 0) in vec3 position;\n \
		uniform mat4 Model; \
		uniform mat4 View; \
		uniform mat4 Projection; \
		void UserShader();\
		void main()\
		{\
			UserShader();\
			gl_Position = Projection * View * Model * vec4(position, 1);\
		}";

	const char* fragment_code =
		"#version 330 core\n \
		void UserShader();\
		void main()\
		{\
			UserShader();\
		}";

	Shader* vsh = App->shader->CreateShader(ShaderType::VERTEX);
	vsh->SetShaderCode(vertex_code);

	Shader* user_vsh = App->shader->CreateShader(ShaderType::VERTEX);
	user_vsh->SetShaderCode(user_vertex_code);

	Shader* fsh = App->shader->CreateShader(ShaderType::FRAGMENT);
	fsh->SetShaderCode(fragment_code);

	Shader* user_fsh = App->shader->CreateShader(ShaderType::FRAGMENT);
	user_fsh->SetShaderCode(user_fragment_code);

	program = App->shader->CreateShaderProgram();
	program->AddShader(user_vsh);
	program->AddShader(user_fsh);
	program->AddShader(vsh);
	program->AddShader(fsh);

	program->LinkProgram();

	vao = App->renderer->GenVertexArrayBuffer();
	App->renderer->BindVertexArrayBuffer(vao);

	vbo = App->renderer->GenBuffer();
	App->renderer->BindArrayBuffer(vbo);

	App->renderer->LoadArrayToVRAM(lines_vb.GetSize(), lines_vb.GetBuffer(), GL_STREAM_DRAW);
}

void LineGuizmoRenderer::CleanUp()
{

}

void LineGuizmoRenderer::Render(const float4x4& view, const float4x4& projection)
{
	App->renderer->BindArrayBuffer(vbo);
	App->renderer->LoadArrayToVRAM(lines_vb.GetSize(), lines_vb.GetBuffer(), GL_STREAM_DRAW);

	float4x4 model = float4x4::identity;
	model[0][3] = 1;
	model[1][3] = 1;
	model[2][3] = 1;

	program->UseProgram();

	ShaderProgramParameters par;
	par.SetVector3("Colour", float3(1.0f, 1.0f, 1.0f));
	program->SetProgramParameters(par);

	GLint posAttrib = glGetAttribLocation(program->GetID(), "position");
	App->renderer->EnableVertexAttributeArray(posAttrib);
	App->renderer->SetVertexAttributePointer(posAttrib, 3, 6, 0);

	GLint posAttribCol = glGetAttribLocation(program->GetID(), "col");
	App->renderer->EnableVertexAttributeArray(posAttribCol);
	App->renderer->SetVertexAttributePointer(posAttribCol, 3, 6, 3);

	App->renderer->SetUniformMatrix(program->GetID(), "Model", model.Transposed().ptr());
	App->renderer->SetUniformMatrix(program->GetID(), "View", view.ptr());
	App->renderer->SetUniformMatrix(program->GetID(), "Projection", projection.ptr());

	glLineWidth(25);

	glDrawArrays(GL_LINES, 0, lines_count * 2);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
	}

	App->renderer->DisableVertexAttributeArray(posAttrib);
	App->renderer->DisableVertexAttributeArray(posAttribCol);

	lines_vb.Clear();
	lines_count = 0;
}

void LineGuizmoRenderer::DrawLine(float2 start, float2 end, float3 colour)
{
	lines_vb.AddSpace(12);

	lines_vb.AddFloat3(float3(start.x, start.y, 0));
	lines_vb.AddFloat3(colour);
	lines_vb.AddFloat3(float3(end.x, end.y, 0));
	lines_vb.AddFloat3(colour);

	++lines_count;
}
