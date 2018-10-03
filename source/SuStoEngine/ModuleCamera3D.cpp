#include "Globals.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "Cursor.h"
#include "imgui.h"
#include "JSONLoader.h"
#include "ModuleRenderer3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	editor_camera = CreateCamera();
	current_camera = editor_camera;
	current_camera->SetFrustumCulling(false);
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Awake()
{
	bool ret = true;

	INTERNAL_LOG("Loading Camera3D");

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	bool ret = true;

	INTERNAL_LOG("Setting up the camera");

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Cleaning cameras");

	DestroyAllCameras();

	return ret;
}

void ModuleCamera3D::OnLoadConfig(JSON_Doc * config)
{
	mouse_sensitivity = config->GetNumber("camera.mouse_sensitivity", 0.25f);
	wheel_speed = config->GetNumber("camera.wheel_speed", 5.0f);
	camera_speed = config->GetNumber("camera.camera_speed", 20.0f);

	float3 cam_pos = config->GetNumber3("camera_position");
	float3 z_dir = config->GetNumber3("camera_front", float3(0, 0, 1));
	float3 y_dir = config->GetNumber3("camera_up", float3(0, 1, 0));
	editor_camera->SetPosition(cam_pos);
	editor_camera->SetZDir(z_dir);
	editor_camera->SetYDir(y_dir);
}

void ModuleCamera3D::OnSaveConfig(JSON_Doc * config)
{
	config->SetNumber("camera.mouse_sensitivity", mouse_sensitivity);
	config->SetNumber("camera.wheel_speed", wheel_speed);
	config->SetNumber("camera.camera_speed", camera_speed);
	config->SetNumber3("camera_position", editor_camera->GetPosition());
	config->SetNumber3("camera_front", editor_camera->GetZDir());
	config->SetNumber3("camera_up", editor_camera->GetYDir());
}

Camera3D * ModuleCamera3D::CreateCamera()
{
	Camera3D* ret = nullptr;

	ret = new Camera3D;
	cameras.push_back(ret);

	return ret;
}

void ModuleCamera3D::DestroyCamera(Camera3D * cam)
{
	for (std::vector<Camera3D*>::iterator it = cameras.begin(); it != cameras.end();)
	{
		if (cam == (*it))
		{
			RELEASE(*it);
			cameras.erase(it);
			break;
		}
		else
			++it;
	}
}

void ModuleCamera3D::DestroyAllCameras()
{
	for (std::vector<Camera3D*>::iterator it = cameras.begin(); it != cameras.end();)
	{
		(*it)->CleanUp();
		RELEASE(*it);
		it = cameras.erase(it);
	}
}

std::vector<Camera3D*> ModuleCamera3D::GetCameras()
{
	return cameras;
}

Camera3D * ModuleCamera3D::GetEditorCamera() const
{
	return editor_camera;
}

void ModuleCamera3D::SetMouseSensitivity(const float& set)
{
	mouse_sensitivity = set;
}

void ModuleCamera3D::SetWheelSpeed(const float& set)
{
	wheel_speed = set;
}

void ModuleCamera3D::SetCameraSpeed(const float& set)
{
	camera_speed = set;
}

const float ModuleCamera3D::GetMouseSensitivity() const
{
	return mouse_sensitivity;
}

const float ModuleCamera3D::GetWheelSpeed() const
{
	return wheel_speed;
}

const float ModuleCamera3D::GetCameraSpeed() const
{
	return camera_speed;
}

void ModuleCamera3D::SetCurrentCamera(Camera3D * set)
{
	if (set != nullptr)
		current_camera = set;
}

Camera3D * ModuleCamera3D::GetCurrentCamera() const
{
	return current_camera;
}

void ModuleCamera3D::SetCurrentCameraToEditorCamera()
{
	current_camera = editor_camera;
}

const float * ModuleCamera3D::GetViewMatrix() const
{
	return current_camera->GetOpenGLViewMatrix().ptr();
}

const float * ModuleCamera3D::GetProjectionMatrix() const
{
	return current_camera->GetOpenGLProjectionMatrix().ptr();
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Update()
{
	bool ret = true;

	if (editor_camera == nullptr)
		return true;

	float cam_speed = camera_speed * App->GetDT();
	float whe_speed = wheel_speed * App->GetDT();
	float mou_speed = mouse_sensitivity * App->GetDT();

	if (App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT))
		cam_speed = camera_speed/2 * App->GetDT();

	//if (IsMouseInsideWindow())
	//{
	//	mouse_movement = true;
	//}

	// Mouse motion ----------------
	if (mouse_movement)
	{
		if (App->input->GetMouseWheel() == 1)
		{
			editor_camera->MoveFront(whe_speed);
		}
		else if (App->input->GetMouseWheel() == -1)
		{
			editor_camera->MoveBack(whe_speed);
		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			App->window->GetCursor()->Hand();

			if (App->input->GetKeyRepeat(SDL_SCANCODE_Z))
				editor_camera->MoveUp(cam_speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_X))
				editor_camera->MoveDown(cam_speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_W))
				editor_camera->MoveFront(cam_speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_S))
				editor_camera->MoveBack(cam_speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_A))
				editor_camera->MoveLeft(cam_speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_D))
				editor_camera->MoveRight(cam_speed);

			editor_camera->Rotate(-App->input->GetMouseXMotion()*mou_speed, -App->input->GetMouseYMotion()*mou_speed);

			//App->gameobj->SetCanMove(false);
			
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			if (App->input->GetKeyRepeat(SDL_SCANCODE_LALT) || App->input->GetKeyRepeat(SDL_SCANCODE_RALT))
			{
				if (App->input->GetKeyRepeat(SDL_SCANCODE_W))
					editor_camera->MoveFront(cam_speed);

				if (App->input->GetKeyRepeat(SDL_SCANCODE_S))
					editor_camera->MoveBack(cam_speed);

				editor_camera->Orbit(float3(0, 0, 0), -App->input->GetMouseXMotion()*mou_speed, -App->input->GetMouseYMotion()*mou_speed);
				editor_camera->Look(float3(0, 0, 0));

				App->window->GetCursor()->SizeAll();

				//App->gameobj->SetCanMove(false);
			}
		}
		else
		{
			//mouse_movement = false;
			//App->gameobj->SetCanMove(true);
		}
	}

	if (App->input->GetKeyDown("f"))
	{
		editor_camera->Focus(float3(0, 0, 0), 10);
	}

	return ret;
}

//const bool ModuleCamera3D::IsMouseInsideWindow() const
//{
//	return PointInRect(float2(App->input->GetMouseX(), App->input->GetMouseY()), App->editorUI->GameRect());
//}

Camera3D::Camera3D()
{
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	frustum.SetPos(float3(0, 1, -1));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	aspect_ratio = 0;

	SetNearPlaneDistance(0.1f);
	SetFarPlaneDistance(10000.0f);
	SetAspectRatio(1.3f);
	SetFOV(60);

	render_tex = new RenderTexture();
}

void Camera3D::CleanUp()
{
	render_tex->CleanUp();
	RELEASE(render_tex);
}

void Camera3D::Bind(uint x, uint y, uint width, uint heigth)
{
	if(render_tex != nullptr)
		render_tex->Bind(x, y, width, heigth);
}

void Camera3D::Unbind()
{
	if(render_tex != nullptr)
		render_tex->Unbind();
}

uint Camera3D::GetTextId()
{
	uint ret = 0;

	if (render_tex != nullptr)
		ret = render_tex->GetTextureID();

	return ret;
}

void Camera3D::SetPosition(const float3 & pos)
{
	frustum.SetPos(pos);
}

const float3 Camera3D::GetPosition()
{
	return frustum.Pos();
}

void Camera3D::SetZDir(const float3 & front)
{
	frustum.SetFront(front.Normalized());
}

void Camera3D::SetYDir(const float3 & front)
{
	frustum.SetUp(front.Normalized());
}

float3 Camera3D::GetZDir()
{
	return frustum.Front();
}

float3 Camera3D::GetYDir()
{
	return frustum.Up();
}

void Camera3D::GetCorners(float3* corners)
{
	frustum.GetCornerPoints(corners);
}

void Camera3D::SetNearPlaneDistance(const float & set)
{
	frustum.SetViewPlaneDistances(set, frustum.FarPlaneDistance());
}

void Camera3D::SetFarPlaneDistance(const float & set)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), set);
}

void Camera3D::SetFOV(const float & set)
{
	if (set > 0)
	{
		vertical_fov = set * DEGTORAD;
		frustum.SetVerticalFovAndAspectRatio(DEGTORAD * set, aspect_ratio);
	}
}

void Camera3D::SetAspectRatio(const float & set)
{
	if (set > 0)
	{
		aspect_ratio = set;
		frustum.SetVerticalFovAndAspectRatio(vertical_fov, set);
	}
}

const float Camera3D::GetNearPlaneDistance() const
{
	return frustum.NearPlaneDistance();
}

const float Camera3D::GetFarPlaneDistance() const
{
	return frustum.FarPlaneDistance();
}

const float Camera3D::GetVerticalFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

const float Camera3D::GetHorizontalFOV() const
{
	return frustum.HorizontalFov() * RADTODEG;
}

const float4x4 Camera3D::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

const float4x4 Camera3D::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

const float4x4 Camera3D::GetOpenGLViewMatrix() const
{
	float4x4 view = frustum.ViewMatrix();
	return view.Transposed();
}

const float4x4 Camera3D::GetOpenGLProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed();
}

void Camera3D::MoveFront(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.Front() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveBack(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.Front() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveRight(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveLeft(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveUp(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveDown(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera3D::Orbit(const float3 & rotate_center, const float & motion_x, const float & motion_y)
{
	float3 distance = frustum.Pos() - rotate_center;

	Quat X(frustum.WorldRight(), motion_y);
	Quat Y(frustum.Up(), motion_x);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	frustum.SetPos(distance + rotate_center);
}

void Camera3D::Rotate(const float & motion_x, const float & motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	frustum.SetFront(rotation_x.Mul(frustum.Front()).Normalized());
	frustum.SetUp(rotation_x.Mul(frustum.Up()).Normalized());

	Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), motion_y);
	frustum.SetFront(rotation_y.Mul(frustum.Front()).Normalized());
	frustum.SetUp(rotation_y.Mul(frustum.Up()).Normalized());
}

void Camera3D::Look(const float3 & look_pos)
{
	float3 dir = look_pos - frustum.Pos();

	float3x3 direction_matrix = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(direction_matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(direction_matrix.MulDir(frustum.Up()).Normalized());
}

void Camera3D::GetElementsToDraw()
{
	//std::vector<GameObject*> to_check = App->gameobj->GetDynamicGameObjects();

	//// Clean all objects that doesn't have aabb
	//for (std::vector<GameObject*>::iterator it = to_check.begin(); it != to_check.end();)
	//{
	//	if ((*it)->GetComponent(MESH) == nullptr)
	//	{
	//		(*it)->SetDraw(false);
	//		it = to_check.erase(it);
	//	}
	//	else 
	//		it++;
	//}
	//App->gameobj->GetKDTree()->GetElementsToTest(frustum, to_check);

	////test elements with frustum
	//for (std::vector<GameObject*>::iterator it = to_check.begin(); it != to_check.end(); ++it)
	//{
	//	if (CheckInsideFrustum((*it)->GetBbox()))
	//	{
	//		(*it)->SetDraw(true);
	//	}
	//	else
	//	{
	//		(*it)->SetDraw(false);
	//	}
	//}
}

void Camera3D::DiscardElementsToDraw(std::vector<GameObject*>& dynamics, std::vector<GameObject*>& statics)
{
}

bool Camera3D::CheckInsideFrustum(const AABB & box)
{
	bool ret = true;

	// Get aabb corners
	float3 corners[8];
	box.GetCornerPoints(corners);

	// Test all corners for each plane
	for (int p = 0; p < 6; ++p)
	{
		uint corners_in = 8;

		for (int c = 0; c < 8; ++c)
		{
			if (frustum.GetPlane(p).IsOnPositiveSide(corners[c]))
			{
				corners_in--;
			}
		}

		if (corners_in == 0)
		{
			ret = false;
			break;
		}
	}

	return ret;
}

void Camera3D::SetFrustumCulling(bool set)
{
	frustum_culling = set;
}

bool Camera3D::GetFrustumCulling()
{
	return frustum_culling;
}


Frustum Camera3D::GetFrustum()
{
	return frustum;
}

void Camera3D::Focus(const float3 & focus_center, const float & distance)
{
	float3 dir = frustum.Pos() - focus_center;
	frustum.SetPos(dir.Normalized() * distance);

	Look(focus_center);
}

void Camera3D::Focus(const AABB & aabb)
{
	Focus(aabb.CenterPoint(), aabb.Size().Length());
}

RenderTexture::RenderTexture()
{
}

RenderTexture::~RenderTexture()
{
}

void RenderTexture::CleanUp()
{
	if(created)
		Destroy();
}

bool RenderTexture::Create(uint _x, uint _y, uint _width, uint _height)
{
	bool ret = true;

	x = _x;
	y = _y;
	width = _width;
	height = _height;

	if (x < 0)
		x = 0;

	if (y < 0)
		y = 0;

	if (width < 1)
		width = 1;

	if (height < 1)
		height = 1;

	//Create MSAA framebufer
	glGenFramebuffers(1, &fbo_msaa_id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_msaa_id);

	//Create a multisampled color attachment texture
	glGenTextures(1, &texture_msaa_id);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_msaa_id);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, current_msaa_samples, GL_RGB, width, height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture_msaa_id, 0);

	//Create a renderbuffer for depth and stencil
	glGenRenderbuffers(1, &rbo_id);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, current_msaa_samples, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_id);

	GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error != GL_FRAMEBUFFER_COMPLETE)
	{
		CONSOLE_ERROR("RenderTextureMSAA: Framebuffer is not complete! %s", gluErrorString(error));
		ret = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//configure post-processing framebuffer
	glGenFramebuffers(1, &fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

	//create the color attachment texture
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error != GL_FRAMEBUFFER_COMPLETE)
	{
		CONSOLE_ERROR("RenderTextureMSAA: Intermediate framebuffer is not complete! %s", gluErrorString(error));
		ret = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	////Create MSAA framebufer
	//fbo_msaa_id = App->renderer3D->GenFrameBuffer();
	//App->renderer3D->BindFrameBuffer(fbo_msaa_id);

	////Create a multisampled color attachment texture
	//texture_msaa_id = App->renderer3D->GenTexture();
	//App->renderer3D->BindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_msaa_id);
	//App->renderer3D->Set2DMultisample(current_msaa_samples, width, height);
	//App->renderer3D->UnbindTexture(GL_TEXTURE_2D_MULTISAMPLE);
	//App->renderer3D->SetFrameBufferTexture2D(GL_TEXTURE_2D_MULTISAMPLE, texture_msaa_id);

	////Create a renderbuffer for depth and stencil
	//rbo_id = App->renderer3D->GenRenderBuffer();
	//App->renderer3D->BindRenderBuffer(rbo_id);
	//App->renderer3D->RenderStorageMultisample(current_msaa_samples, width, height);
	//App->renderer3D->UnbindRenderBuffer();
	//App->renderer3D->RenderFrameBuffer(rbo_id);

	//GLenum error = App->renderer3D->CheckFrameBufferStatus();
	//if (error != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	CONSOLE_ERROR("RenderTextureMSAA: Framebuffer is not complete! %s", gluErrorString(error));
	//	ret = false;
	//}

	//App->renderer3D->UnbindFrameBuffer();

	////configure post-processing framebuffer
	//fbo_id = App->renderer3D->GenFrameBuffer();
	//App->renderer3D->BindFrameBuffer(fbo_id);

	////create the color attachment texture
	//texture_id = App->renderer3D->GenTexture();

	//App->renderer3D->BindTexture(texture_id);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//App->renderer3D->SetFrameBufferTexture2D(GL_TEXTURE_2D, texture_id);

	//error = App->renderer3D->CheckFrameBufferStatus();
	//if (error != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	CONSOLE_ERROR("RenderTextureMSAA: Intermediate framebuffer is not complete! %s", gluErrorString(error));
	//	ret = false;
	//}
	//App->renderer3D->UnbindFrameBuffer();

	created = true;

	return ret;
}

void RenderTexture::Resize(uint x, uint y, uint width, uint height)
{
	Destroy();

	Create(x, y, width, height);
}

void RenderTexture::Bind(uint x, uint y, uint _width, uint _height)
{
	if (!created)
		Create(x, y, _width, _height);

	if (width != _width || height != _height)
		Resize(x, y, width, height);
	
	//App->renderer3D->GetViewport(last_x, last_y, last_width, last_height);

	//App->renderer3D->BindFrameBuffer(fbo_msaa_id);
	//App->renderer3D->SetViewport(0, 0, width, height);
	//App->renderer3D->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::Unbind()
{
	//App->renderer3D->UnbindFrameBuffer();
	//glViewport(last_x, last_y, last_width, last_height);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_msaa_id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id);
	glBlitFramebuffer(0, 0, width, height,  // src rect
		0, 0, width, height,  // dst rect
		GL_COLOR_BUFFER_BIT, // buffer mask
		GL_NEAREST); // scale filter

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(last_x, last_y, last_width, last_height);
}

void RenderTexture::ChangeMSAALevel(int MSAA_level)
{
	Destroy();

	current_msaa_samples = MSAA_level;

	Create(x, y, width, height);
}

void RenderTexture::Destroy()
{
	//App->renderer3D->DeleteTexture(texture_id);
	//App->renderer3D->DeleteTexture(texture_msaa_id);
	//App->renderer3D->DeleteFrameBuffer(fbo_id);
	//App->renderer3D->DeleteFrameBuffer(fbo_msaa_id);
	//App->renderer3D->DeleteFrameBuffer(rbo_id);

	glDeleteTextures(1, &texture_id);
	texture_id = 0;
	glDeleteTextures(1, &texture_msaa_id);
	texture_msaa_id = 0;

	glDeleteFramebuffers(1, &fbo_id);
	fbo_id = 0;
	glDeleteFramebuffers(1, &fbo_msaa_id);
	fbo_msaa_id = 0;
	glDeleteRenderbuffers(1, &rbo_id);
	rbo_id = 0;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_ERROR("RenderTextureMSAA: error at destroy! %s\n", gluErrorString(error));
	}

	created = false;
}

uint RenderTexture::GetTextureID() const
{
	return texture_id;
}

int RenderTexture::GetMaxMSAALevel() const
{
	return max_msaa_samples;
}

int RenderTexture::GetCurrentMSAALevel() const
{
	return current_msaa_samples;
}

uint RenderTexture::GetWidth() const
{
	return width;
}

uint RenderTexture::GetHeight() const
{
	return height;
}
