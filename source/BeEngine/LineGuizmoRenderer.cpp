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
	const char* geo_code
		= "#version 330 core\
			layout (lines) in;\
			void main() {\
			gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);\
			}";

	Shader* geo_shader = App->shader->CreateShader(ShaderType::GEOMETRY);

	VertexBuffer vb;
	vb.AddFloat(43);
	vb.AddFloat2(float2(3, 4));
	vb.AddFloat3(float3(4, 2, 3));
}

void LineGuizmoRenderer::CleanUp()
{

}
