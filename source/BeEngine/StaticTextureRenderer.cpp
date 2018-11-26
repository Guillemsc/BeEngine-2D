#include "StaticTextureRenderer.h"
#include "App.h"
#include "ModuleShader.h"

StaticTextureRenderer::StaticTextureRenderer()
{
}

StaticTextureRenderer::~StaticTextureRenderer()
{
}

void StaticTextureRenderer::Start()
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


	VertexBuffer quad_indices_buffer;

	quad_indices_buffer.AddFloat3(float3(0, 1, 3));
	quad_indices_buffer.AddFloat3(float3(1, 2, 3));


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
		in vec4 col; \
		in bool has_texture; \
		uniform sampler2D Texture; \
		in vec4 oUvs; \
		void main()\
		{\
			if(has_texture == true)\
			{\
				gl_FragColor = texture2D(Texture, oUvs);\
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

	vao = App->renderer->GenVertexArrayBuffer();
	vbo = App->renderer->GenBuffer();

	App->renderer->BindArrayBuffer(vbo);
	App->renderer->LoadArrayToVRAM(quad_indices_buffer.GetSize(), quad_indices_buffer.GetBuffer(), GL_STATIC_DRAW);

	App->renderer->UnbindArraybuffer();
	App->renderer->UnbindVertexArrayBuffer();
}

void StaticTextureRenderer::CleanUp()
{
}

void StaticTextureRenderer::Render(const float4x4 & view, const float4x4 & projection)
{
}
