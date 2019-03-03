#include "App.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "Globals.h"
#include "App.h"
#include "ModuleRenderer.h"
#include "FBO.h"
#include "ModuleShader.h"
#include "ModuleEditor.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "VertexBuffer.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/Lib/glew32.lib") 

ModuleRenderer::ModuleRenderer() : Module()
{

}

// Destructor
ModuleRenderer::~ModuleRenderer()
{
}

// Called before render is available
bool ModuleRenderer::Awake()
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

bool ModuleRenderer::Start()
{
	bool ret = true;

	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer::PreUpdate()
{
	bool ret = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return ret;
}

// PostUpdate present buffer to screen
bool ModuleRenderer::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool ModuleRenderer::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return ret;
}

SDL_GLContext ModuleRenderer::GetSDLGLContext() const
{
	return context;
}

void ModuleRenderer::SwapWindowBuffers()
{
	SDL_GL_SwapWindow(App->window->GetWindow());
}

void ModuleRenderer::SetPoligonModeWireframe() const
{
	bool wireframe = true;
	bool points = false;
	bool fill = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void ModuleRenderer::SetPoligonModePoints(float point_size) const
{
	bool wireframe = false;
	bool points = true;
	bool fill = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glPointSize(point_size);
}

void ModuleRenderer::SetPoligonModeFill() const
{
	bool wireframe = false;
	bool points = false;
	bool fill = true;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer::SetDepthTest(const bool& set) const
{
	if (set)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer::SetCullFace(const bool& set) const
{
	if (set)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void ModuleRenderer::SetLightingState(const bool& set) const
{
	if (set)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void ModuleRenderer::SetTexture2D(const bool& set) const
{
	if (set)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer::SetColorMaterial(const bool& set) const
{
	if (set)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer::SetAmbientLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, color);

	if (enabled)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}

void ModuleRenderer::SetDiffuseLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color);

	if (enabled)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
}

void ModuleRenderer::SetSpecularLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT2, GL_SPECULAR, color);

	if (enabled)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);
}

void ModuleRenderer::SetViewport(uint start_x, uint start_y, uint width, uint height)
{
	glViewport(start_x, start_y, width, height);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error setting viewport: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::GetViewport(uint & start_x, uint & start_y, uint & width, uint & height)
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

void ModuleRenderer::Clear(uint buffer)
{
	glClear(buffer);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error clearing buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer::GenBuffer() const
{
	uint ret = 0;

	glGenBuffers(1, (GLuint*)&ret);

	return ret;
}

void ModuleRenderer::UnloadBuffer(uint& id)
{
	if(id > 0)
		glDeleteBuffers(1, &id);
}

void ModuleRenderer::BindArrayBuffer(uint id) const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::BindElementArrayBuffer(uint id) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::RenderElement(uint num_indices) const
{
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error draw elements: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::UnbindArraybuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::UnbindElementArrayBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::EnableState(GLenum type) const
{
	glEnableClientState(type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error enable state: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::DisableState(GLenum type) const
{
	glDisableClientState(type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error disable state: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::SetVertexPointer() const
{
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error set vertex pointer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::SetNormalsPointer() const
{
	glNormalPointer(GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error set normals pointer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::SetTexCoordPointer()
{
	glTexCoordPointer(3, GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error set texcoord pointer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer::GenTexture() const
{
	uint ret = 0;

	glGenTextures(1, &ret);

	return ret;
}

void ModuleRenderer::BindTexture(uint id) const
{
	glBindTexture(GL_TEXTURE_2D, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::BindTexture(uint target, uint id) const
{
	glBindTexture((GLenum)target, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::UnbindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::UnbindTexture(uint target) const
{
	glBindTexture((GLenum)target, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::DeleteTexture(uint& id) const
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

uint ModuleRenderer::GenRenderBuffer() const
{
	uint ret = 0;

	glGenRenderbuffers(1, &ret);

	return ret;
}

void ModuleRenderer::BindRenderBuffer(uint id) const
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error binding render buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::UnbindRenderBuffer() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind render buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::Set2DMultisample(uint samples, uint width, uint height)
{
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_TRUE);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error setting 2D multisample texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::SetFrameBufferTexture2D(uint target, uint id)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, id, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error setting frame buffer texture2D: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::RenderStorageMultisample(uint samples, uint width, uint height)
{
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error rendering storage multisample: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::LoadArrayToVRAM(uint size, const float * values, GLenum type) const
{
	glBufferData(GL_ARRAY_BUFFER, size, values, type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error load array to vram: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::LoadElementArrayToVRAM(uint size, uint * values, GLenum type) const
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, values, type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error load array to vram: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer::LoadTextureToVRAM(uint w, uint h, GLubyte * tex_data, GLint format) const
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

void ModuleRenderer::UpdateVRAMArray(uint size, const float * values, GLenum type) const
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, values);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error load array to vram: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::PushMatrix()
{
	glPushMatrix();
}

void ModuleRenderer::PopMatrix()
{
	glPopMatrix();
}

void ModuleRenderer::MultMatrix(float * matrix)
{
	glMultMatrixf(matrix);
}

uint ModuleRenderer::GenVertexArrayBuffer() const
{
	uint ret = 0;

	glGenVertexArrays(1, (GLuint*)&ret);

	return ret;
}

void ModuleRenderer::BindVertexArrayBuffer(uint id) const
{
	glBindVertexArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bind vertex array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::UnbindVertexArrayBuffer() const
{
	glBindVertexArray(0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbind array buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer::GenFrameBuffer() const
{
	uint ret = 0;

	glGenFramebuffers(1, (GLuint*)&ret);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error generating frame buffer: %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer::BindFrameBuffer(uint id) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error binding frame buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::BindFrameBuffer(uint target, uint id) const
{
	glBindFramebuffer(target, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error binding frame buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::BlitFrameBuffer(uint x, uint y, uint w, uint h) const
{
	glBlitFramebuffer(x, y, w, h,  // src rect
		x, y, w, h,  // dst rect
		GL_COLOR_BUFFER_BIT, // buffer mask
		GL_NEAREST); // scale filter

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error bliting frame buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::RenderFrameBuffer(uint id) const
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error rendering frame buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::UnbindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error unbinding frame buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer::CheckFrameBufferStatus()
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

void ModuleRenderer::DeleteFrameBuffer(uint & id)
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

uint ModuleRenderer::CreateVertexShader(const char * source, std::string& compilation_error_msg)
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

uint ModuleRenderer::CreateFragmentShader(const char * source, std::string& compilation_error_msg)
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

uint ModuleRenderer::CreateGeometryShader(const char * source, std::string & compilation_error_msg)
{
	GLuint geometryShader = 0;

	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	glShaderSource(geometryShader, 1, &source, NULL);
	glCompileShader(geometryShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

	if (success == 0)
	{
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		INTERNAL_LOG("Shader compilation error:\n %s", infoLog);
		glDeleteShader(geometryShader);

		compilation_error_msg = infoLog;
		geometryShader = 0;
	}

	return geometryShader;
}

void ModuleRenderer::DeleteShader(uint shader_id)
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

uint ModuleRenderer::GetProgramBinary(uint program_id, uint buff_size, char * buff) const
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

int ModuleRenderer::GetProgramSize(uint program_id) const
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

void ModuleRenderer::LoadProgramFromBinary(uint program_id, uint buff_size, const char * buff)
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

void ModuleRenderer::BindAttributeLocation(uint program_id, uint index, const char * name)
{
	glBindAttribLocation(program_id, index, name);

		GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error binding attribute locaiton on shader %s\n", gluErrorString(error));
	}
}

GLint ModuleRenderer::GetVertexAttributeArray(uint program, const char * name)
{
	GLint ret = -1;

	ret = glGetAttribLocation(program, name);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error getting vertex attribute array %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer::EnableVertexAttributeArray(uint id)
{
	glEnableVertexAttribArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error enabling vertex attribute Pointer %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::DisableVertexAttributeArray(uint id)
{
	glDisableVertexAttribArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error disabling vertex attributePointer %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::SetVertexAttributePointer(uint id, uint element_size, uint elements_gap, uint infogap)
{
	glVertexAttribPointer(id, element_size, GL_FLOAT, GL_FALSE, elements_gap * sizeof(GLfloat), (void*)(infogap * sizeof(GLfloat)));

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error Setting vertex attributePointer %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer::GetAttributesCount(uint program)
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

void ModuleRenderer::GetAttributesInfo(uint program, uint index, std::string & name, GLenum & type)
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

void ModuleRenderer::SetUniformMatrix(uint program, const char * name, const float * data)
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

void ModuleRenderer::SetUniformFloat(uint program, const char * name, float data)
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

void ModuleRenderer::SetUniformInt(uint program, const char* name, int data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniform1i(modelLoc, data);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error Setting uniform int %s: %s\n", name, gluErrorString(error));
	}
}

void ModuleRenderer::SetUniformBool(uint program, const char * name, bool data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniform1i(modelLoc, data);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_ERROR("Error Setting uniform bool %s: %s\n", name, gluErrorString(error));
	}
}

void ModuleRenderer::SetUniformVec3(uint program, const char * name, const float3& data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniform3f(modelLoc, data.x, data.y, data.z);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error Setting uniform float3 %s: %s\n", name, gluErrorString(error));
	}
}

void ModuleRenderer::SetUniformVec4(uint program, const char * name, const float4 & data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniform4f(modelLoc, data.x, data.y, data.w, data.z);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error Setting uniform float4 %s: %s\n", name, gluErrorString(error));
	}
}

uint ModuleRenderer::GetUniformsCount(uint program)
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

void ModuleRenderer::GetUniformInfo(uint program, uint index, std::string& name, GLenum& type)
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

uint ModuleRenderer::CreateShaderProgram()
{
	uint ret = glCreateProgram();

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error creating shader program %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer::UseShaderProgram(uint id)
{
	glUseProgram(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		INTERNAL_LOG("Error at use shader program: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer::AttachShaderToProgram(uint program_id, uint shader_id)
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

bool ModuleRenderer::LinkProgram(uint program_id, std::string& link_error)
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

			link_error = infoLog;

			ret = false;
		}
	}

	return ret;
}

void ModuleRenderer::DeleteProgram(uint program_id)
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
