#include "App.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "Globals.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"
#include "ModuleShader.h"
#include "ModuleEditor.h"
#include "SDL/include/SDL_opengl.h"
#include "ImGuizmo.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/Lib/glew32.lib") 

ModuleRenderer3D::ModuleRenderer3D() : Module()
{

}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
}

// Called before render is available
bool ModuleRenderer3D::Awake()
{
	bool ret = true;

	INTERNAL_LOG("Creating 3D Renderer context");

	//Create context
	context = SDL_GL_CreateContext(App->window->GetWindow());

	if (context == NULL)
	{
		INTERNAL_LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// Glew
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		INTERNAL_LOG("Glew library could not init %s\n", glewGetErrorString(err));
		ret = false;
	}
	else 
	{
		INTERNAL_LOG("Glew library is being used correctly\n");
		INTERNAL_LOG("Using Glew %s\n", glewGetString(GLEW_VERSION));
	}

	// OpenGL
	if (ret == true)
	{
		// Get version info
		INTERNAL_LOG("Vendor: %s", glGetString(GL_VENDOR));
		INTERNAL_LOG("Renderer: %s", glGetString(GL_RENDERER));
		INTERNAL_LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		INTERNAL_LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		// Use Vsync
		if (SDL_GL_SetSwapInterval(App->window->GetVsync()) < 0)
			INTERNAL_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//fbo_texture = new FBO();
		//fbo_texture->Create(App->window->GetWindowSize().x, App->window->GetWindowSize().y);

		// Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		// Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

		glClearDepth(1.0f);

		// Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisable(GL_CULL_FACE);
	}

	return ret;
}

bool ModuleRenderer3D::Start()
{
	bool ret = true;

	// Shaders testing -----------------------

	//uint vao = GenVertexArrayBuffer();
	//BindVertexArrayBuffer(vao);

	//uint vbo = GenBuffer();
	//BindArrayBuffer(vbo);

	//float g_vertex_buffer_data[] =
	//{
	//	0, 0, 0,
	//	1000.0f, 0, 0.0f,
	//	1000.0f, 1000.0f, 0.0f,
	//};

	//LoadArrayToVRAM(sizeof(g_vertex_buffer_data), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

	//const char* user_vertex_code = 
	//	"#version 330 core\n \
	//	out vec3 oColour;	\
	//	uniform vec3 Colour;\
	//	void UserShader()\
	//	{\
	//		oColour = Colour;\
	//	}";

	//const char* user_fragment_code = 		
	//	"#version 330 core\n \
	//	in vec3 oColour;\
	//	out vec3 color; \
	//	void UserShader()\
	//	{\
	//		color = oColour;\
	//	}";

	//const char* vertex_code =
	//	"#version 330 core\n \
	//	layout(location = 0) in vec3 position;\n \
	//	uniform mat4 Model; \
	//	uniform mat4 View; \
	//	uniform mat4 Projection; \
	//	void UserShader();\
	//	void main()\
	//	{\
	//		UserShader();\
	//		gl_Position = Projection * View * Model * vec4(position, 1);\
	//	}";

	//const char* fragment_code =
	//	"#version 330 core\n \
	//	void UserShader();\
	//	void main()\
	//	{\
	//		UserShader();\
	//	}";

	//Shader* vsh = App->shader->CreateShader(ShaderType::VERTEX);
	//vsh->SetShaderCode(vertex_code);

	//Shader* user_vsh = App->shader->CreateShader(ShaderType::VERTEX);
	//user_vsh->SetShaderCode(user_vertex_code);

	//Shader* fsh = App->shader->CreateShader(ShaderType::FRAGMENT);
	//fsh->SetShaderCode(fragment_code);

	//Shader* user_fsh = App->shader->CreateShader(ShaderType::FRAGMENT);
	//user_fsh->SetShaderCode(user_fragment_code);

	//sp = App->shader->CreateShaderProgram();
	//sp->AddShader(user_vsh);
	//sp->AddShader(user_fsh);
	//sp->AddShader(vsh);
	//sp->AddShader(fsh);

	//sp->LinkProgram();
	// ----------------------------------------

	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer3D::PreUpdate()
{
	bool ret = true;

	//fbo_texture->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return ret;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	bool ret = true;

	// Draw scene
	//RenderScene();

	// Shaders testing -----------------------

	/*float4x4 model = float4x4::identity;
	model[0][3] = 100;
	model[1][3] = 100;
	model[2][3] = 100;

	sp->UseProgram();

	ShaderProgramParameters par;
	par.SetVector3("Colour", float3(1, 1, 1));
	sp->SetProgramParameters(par);

	GLint posAttrib = glGetAttribLocation(sp->GetID(), "position");
	EnableVertexAttributeArray(posAttrib);
	SetVertexAttributePointer(posAttrib, 3, 0, 0);

	SetUniformMatrix(sp->GetID(), "Model", model.Transposed().ptr());
	SetUniformMatrix(sp->GetID(), "View", App->camera->GetEditorCamera()->GetOpenGLViewMatrix().ptr());
	SetUniformMatrix(sp->GetID(), "Projection", App->camera->GetEditorCamera()->GetOpenGLProjectionMatrix().ptr());

	BindArrayBuffer(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error drawing %s\n", gluErrorString(error));
	}

	DisableVertexAttributeArray(posAttrib);*/

	App->editor->RenderEditor();

	// ----------------------------------------

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// Disable light
	//SetLightingState(false);

	//// Draw editor
	////App->editorUI->DrawEditor();

	//// Enable light
	//SetLightingState(true);

	// Swap
	SDL_GL_SwapWindow(App->window->GetWindow());

	return ret;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return ret;
}

SDL_GLContext ModuleRenderer3D::GetSDLGLContext() const
{
	return context;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	App->camera->GetEditorCamera()->SetAspectRatio((float)width / (float)height);
}

void ModuleRenderer3D::RenderScene()
{
	//std::vector<Camera3D*> cameras = App->camera->GetCameras();

	//float2 window_size = App->window->GetWindowSize();

	//for (std::vector<Camera3D*>::iterator it = cameras.begin(); it != cameras.end(); ++it)
	//{
	//	Camera3D* curr_camera = (*it);

	//	curr_camera->Bind(0, 0, window_size.x, window_size.y);

	//	const std::vector<GameObject*> game_object = App->gameobj->GetListGameObjects();

	//	for (std::vector<GameObject*>::const_iterator go = game_object.begin(); go != game_object.end(); ++go)
	//	{
	//		(*go)->Draw();
	//	}

	//	curr_camera->Unbind();
	//}
}

void ModuleRenderer3D::SetPoligonModeWireframe() const
{
	bool wireframe = true;
	bool points = false;
	bool fill = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void ModuleRenderer3D::SetPoligonModePoints(float point_size) const
{
	bool wireframe = false;
	bool points = true;
	bool fill = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glPointSize(point_size);
}

void ModuleRenderer3D::SetPoligonModeFill() const
{
	bool wireframe = false;
	bool points = false;
	bool fill = true;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::SetDepthTest(const bool& set) const
{
	if (set)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::SetCullFace(const bool& set) const
{
	if (set)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::SetLightingState(const bool& set) const
{
	if (set)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::SetTexture2D(const bool& set) const
{
	if (set)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SetColorMaterial(const bool& set) const
{
	if (set)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::SetAmbientLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, color);

	if (enabled)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}

void ModuleRenderer3D::SetDiffuseLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color);

	if (enabled)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
}

void ModuleRenderer3D::SetSpecularLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT2, GL_SPECULAR, color);

	if (enabled)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);
}

void ModuleRenderer3D::SetViewport(uint start_x, uint start_y, uint width, uint height)
{
	glViewport(start_x, start_y, width, height);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error setting viewport: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::GetViewport(uint & start_x, uint & start_y, uint & width, uint & height)
{
	GLint viewport[4]; glGetIntegerv(GL_VIEWPORT, viewport);

	start_x = viewport[0];
	start_y = viewport[1];
	width = viewport[2];
	height = viewport[3];

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error getting viewport: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::Clear(uint buffer)
{
	glClear(buffer);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error clearing buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::GenBuffer() const
{
	uint ret = 0;

	glGenBuffers(1, (GLuint*)&ret);

	return ret;
}

void ModuleRenderer3D::UnloadBuffer(uint& id)
{
	if(id > 0)
		glDeleteBuffers(1, &id);
}

void ModuleRenderer3D::BindArrayBuffer(uint id) const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::BindElementArrayBuffer(uint id) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::RenderElement(uint num_indices) const
{
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error draw elements: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindArraybuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindElementArrayBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::EnableState(GLenum type) const
{
	glEnableClientState(type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error enable state: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::DisableState(GLenum type) const
{
	glDisableClientState(type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error disable state: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetVertexPointer() const
{
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error set vertex pointer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetNormalsPointer() const
{
	glNormalPointer(GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error set normals pointer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetTexCoordPointer()
{
	glTexCoordPointer(3, GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error set texcoord pointer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::GenTexture() const
{
	uint ret = 0;

	glGenTextures(1, &ret);

	return ret;
}

void ModuleRenderer3D::BindTexture(uint id) const
{
	glBindTexture(GL_TEXTURE_2D, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::BindTexture(uint target, uint id) const
{
	glBindTexture((GLenum)target, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindTexture(uint target) const
{
	glBindTexture((GLenum)target, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::DeleteTexture(uint& id) const
{
	if (id > 0)
	{
		glDeleteTextures(1, &id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error deleting texture: %s\n", gluErrorString(error));
		}
	}
}

uint ModuleRenderer3D::GenRenderBuffer() const
{
	uint ret = 0;

	glGenRenderbuffers(1, &ret);

	return ret;
}

void ModuleRenderer3D::BindRenderBuffer(uint id) const
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error binding render buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindRenderBuffer() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind render buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::Set2DMultisample(uint samples, uint width, uint height)
{
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error setting 2D multisample texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetFrameBufferTexture2D(uint target, uint id)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, id, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error setting frame buffer texture2D: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::RenderStorageMultisample(uint samples, uint width, uint height)
{
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error rendering storage multisample: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::LoadArrayToVRAM(uint size, float * values, GLenum type) const
{
	glBufferData(GL_ARRAY_BUFFER, size, values, type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error load array to vram: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::LoadArrayToVRAM(uint size, uint * values, GLenum type) const
{
	glBufferData(GL_ARRAY_BUFFER, size, values, type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error load array to vram: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::LoadTextureToVRAM(uint w, uint h, GLubyte * tex_data, GLint format) const
{
	uint buff_id = 0;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error load texture to vram e1: %s\n", gluErrorString(error));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &buff_id);
	glBindTexture(GL_TEXTURE_2D, buff_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, tex_data);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error load texture to vram e2: %s\n", gluErrorString(error));
	}

	return buff_id; uint();
}

void ModuleRenderer3D::PushMatrix()
{
	glPushMatrix();
}

void ModuleRenderer3D::PopMatrix()
{
	glPopMatrix();
}

void ModuleRenderer3D::MultMatrix(float * matrix)
{
	glMultMatrixf(matrix);
}

uint ModuleRenderer3D::GenVertexArrayBuffer() const
{
	uint ret = 0;

	glGenVertexArrays(1, (GLuint*)&ret);

	return ret;
}

void ModuleRenderer3D::BindVertexArrayBuffer(uint id) const
{
	glBindVertexArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind vertex array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindVertexArrayBuffer() const
{
	glBindVertexArray(0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind array buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::GenFrameBuffer() const
{
	uint ret = 0;

	glGenFramebuffers(1, (GLuint*)&ret);

	return ret;
}

void ModuleRenderer3D::BindFrameBuffer(uint id) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error binding frame buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::RenderFrameBuffer(uint id) const
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error rendering frame buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbinding frame buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::CheckFrameBufferStatus()
{
	uint ret = 0;

	ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error checking frame buffer status: %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer3D::DeleteFrameBuffer(uint & id)
{
	if (id > 0)
	{
		glDeleteFramebuffers(1, &id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error deleting frame buffer: %s\n", gluErrorString(error));
		}
	}
}

uint ModuleRenderer3D::CreateVertexShader(const char * source, std::string& compilation_error_msg)
{
	GLuint vertexShader = 0;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (success == 0)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		INTERNAL_LOG("Shader compilation error:\n %s", infoLog);
		glDeleteShader(vertexShader);

		compilation_error_msg = infoLog;
		vertexShader = 0;
	}

	return vertexShader;
}

uint ModuleRenderer3D::CreateFragmentShader(const char * source, std::string& compilation_error_msg)
{
	GLuint fragmentShader = 0;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (success == 0)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		INTERNAL_LOG("Shader compilation error:\n %s", infoLog);
		glDeleteShader(fragmentShader);

		compilation_error_msg = infoLog;
		fragmentShader = 0;
	}

	return fragmentShader;
}

void ModuleRenderer3D::DeleteShader(uint shader_id)
{
	if (shader_id > 0)
	{
		glDeleteShader(shader_id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error deleting shader %s\n", gluErrorString(error));
		}
	}
}

uint ModuleRenderer3D::GetProgramBinary(uint program_id, uint buff_size, char * buff) const
{
	uint ret = 0;

	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	GLint *binaryFormats = new GLint[formats];
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

	glGetProgramBinary(program_id, buff_size, (GLsizei*)&ret, (GLenum*)binaryFormats, buff);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error geting shader program binary %s\n", gluErrorString(error));
	}

	RELEASE_ARRAY(binaryFormats);

	return ret;
}

int ModuleRenderer3D::GetProgramSize(uint program_id) const
{
	int ret = 0;

	glGetProgramiv(program_id, GL_PROGRAM_BINARY_LENGTH, &ret);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error geting shader program size %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer3D::LoadProgramFromBinary(uint program_id, uint buff_size, const char * buff)
{
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	GLint *binaryFormats = new GLint[formats];
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

	glProgramBinary(program_id, (GLenum)binaryFormats, buff, buff_size);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error loading shader program binary %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::EnableVertexAttributeArray(uint id)
{
	glEnableVertexAttribArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error enabling vertex attribute Pointer %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::DisableVertexAttributeArray(uint id)
{
	glDisableVertexAttribArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error disabling vertex attributePointer %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetVertexAttributePointer(uint id, uint element_size, uint elements_gap, uint infogap)
{
	glVertexAttribPointer(id, element_size, GL_FLOAT, GL_FALSE, elements_gap * sizeof(GLfloat), (void*)(infogap * sizeof(GLfloat)));

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error Setting vertex attributePointer %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::GetAttributesCount(uint program)
{
	uint ret = 0;

	GLint count = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_ERROR("Error getting attributes count on program %d: %s\n", program, gluErrorString(error));
	}

	ret = count;

	return ret;
}

void ModuleRenderer3D::GetAttributesInfo(uint program, uint index, std::string & name, GLenum & type)
{
	const GLsizei bufSize = 100;
	GLchar c_name[bufSize];
	GLsizei length;
	GLint size;

	glGetActiveAttrib(program, (GLuint)index, bufSize, &length, &size, &type, c_name);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_ERROR("Error getting attribute info on program %d: %s\n", program, gluErrorString(error));
	}

	name = c_name;
}

void ModuleRenderer3D::SetUniformMatrix(uint program, const char * name, const float * data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, data);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error Setting uniform matrix %s: %s\n", name, gluErrorString(error));
	}
}

void ModuleRenderer3D::SetUniformFloat(uint program, const char * name, float data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniform1f(modelLoc, data);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error Setting uniform float %s: %s\n", name, gluErrorString(error));
	}
}

void ModuleRenderer3D::SetUniformBool(uint program, const char * name, bool data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniform1i(modelLoc, data);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_ERROR("Error Setting uniform float %s: %s\n", name, gluErrorString(error));
	}
}

void ModuleRenderer3D::SetUniformVec3(uint program, const char * name, float3 data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniform3f(modelLoc, data.x, data.y, data.z);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error Setting uniform float %s: %s\n", name, gluErrorString(error));
	}
}

uint ModuleRenderer3D::GetUniformsCount(uint program)
{
	uint ret = 0;

	GLint count = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_ERROR("Error getting uniforms count on program %d: %s\n", program, gluErrorString(error));
	}

	ret = count;

	return ret;
}

void ModuleRenderer3D::GetUniformInfo(uint program, uint index, std::string& name, GLenum& type)
{
	const GLsizei bufSize = 100;
	GLchar c_name[bufSize];
	GLsizei length;
	GLint size;

	glGetActiveUniform(program, (GLuint)index, bufSize, &length, &size, &type, c_name);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_ERROR("Error getting uniforms info on program %d: %s\n", program, gluErrorString(error));
	}

	name = c_name;
}

uint ModuleRenderer3D::CreateShaderProgram()
{
	uint ret = glCreateProgram();

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error creating shader program %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer3D::UseShaderProgram(uint id)
{
	glUseProgram(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error at use shader program: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::AttachShaderToProgram(uint program_id, uint shader_id)
{
	if (program_id > 0 && shader_id > 0)
	{
		glAttachShader(program_id, shader_id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error attaching shader %s\n", gluErrorString(error));
		}
	}
}

bool ModuleRenderer3D::LinkProgram(uint program_id)
{
	bool ret = false;

	if (program_id != 0)
	{
		glLinkProgram(program_id);

		GLint success;
		GLint valid;
		glGetProgramiv(program_id, GL_LINK_STATUS, &success);
		glGetProgramiv(program_id, GL_VALIDATE_STATUS, &valid);

		ret = true;

		if (!success || !valid) 
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(program_id, 512, NULL, infoLog);
			INTERNAL_LOG("Shader link error: %s", infoLog);

			ret = false;
		}
	}

	return ret;
}

void ModuleRenderer3D::DeleteProgram(uint program_id)
{
	if (program_id > 0)
	{
		glDeleteProgram(program_id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			INTERNAL_LOG("Error deleting shader program %s\n", gluErrorString(error));
		}
	}
}