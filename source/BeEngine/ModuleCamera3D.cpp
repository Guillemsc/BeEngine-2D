#include "Globals.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "Cursor.h"
#include "imgui.h"
#include "ModuleRenderer3D.h"
#include "ModuleJson.h"
#include "ModuleAction.h"

ModuleCamera3D::ModuleCamera3D() : Module()
{
	editor_camera = CreateCamera();
	game_camera = CreateCamera();
	editor_camera->SetFrustumCulling(false);
	game_camera->SetFrustumCulling(false);
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Awake()
{
	bool ret = true;


	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	bool ret = true;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	bool ret = true;

	DestroyAllCameras();

	return ret;
}

void ModuleCamera3D::OnLoadConfig(JSON_Doc * config)
{
	mouse_sensitivity = config->GetNumber("camera.mouse_sensitivity", 0.25f);
	wheel_speed = 10000; //config->GetNumber("camera.wheel_speed", 5.0f);
	camera_speed = 1000;//config->GetNumber("camera.camera_speed", 20.0f);

	float3 cam_pos = config->GetNumber3("camera_position");
	//float3 z_dir = config->GetNumber3("camera_front", float3(0, 0, 1));
	//float3 y_dir = config->GetNumber3("camera_up", float3(0, 1, 0));
	editor_camera->SetPosition(float3(0, 0, 0));
	editor_camera->SetZDir(float3(0, 0, 1));
	editor_camera->SetYDir(float3(0, 1, 0));
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

Camera2D * ModuleCamera3D::CreateCamera()
{
	Camera2D* ret = nullptr;

	ret = new Camera2D;
	cameras.push_back(ret);

	return ret;
}

void ModuleCamera3D::DestroyCamera(Camera2D * cam)
{
	if (cam != nullptr)
	{
		for (std::vector<Camera2D*>::iterator it = cameras.begin(); it != cameras.end();)
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
}

void ModuleCamera3D::DestroyAllCameras()
{
	for (std::vector<Camera2D*>::iterator it = cameras.begin(); it != cameras.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	cameras.clear();
}

std::vector<Camera2D*> ModuleCamera3D::GetCameras()
{
	return cameras;
}

Camera2D * ModuleCamera3D::GetEditorCamera() const
{
	return editor_camera;
}

void ModuleCamera3D::SetGameCamera(Camera2D * set)
{
	game_camera = set;
}

Camera2D * ModuleCamera3D::GetGameCamera() const
{
	return game_camera;
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
		cam_speed = camera_speed / 2 * App->GetDT();

	//if (IsMouseInsideWindow())
	//{
	mouse_movement = true;
	//}

	// Mouse motion ----------------
	if (mouse_movement)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			last_mouse_position = float2(App->input->GetMouseX(), App->input->GetMouseY());
		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT || App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			dragging = true;
		}
		else
			dragging = false;

		if (dragging)
		{
			float2 mouse_pos = float2(App->input->GetMouseX(), App->input->GetMouseY());

			float2 motion = mouse_pos - last_mouse_position;

			//float x_motion = App->input->GetMouseXMotion();

			motion.x *= editor_camera->size;

			if(motion.x > 0)
				editor_camera->MoveLeft(motion.x);
			if (motion.x < 0)
				editor_camera->MoveRight(-motion.x);

			//float y_motion = App->input->GetMouseYMotion();

			motion.y *= editor_camera->size;

			if (motion.y > 0)
				editor_camera->MoveUp(motion.y);
			if (motion.y < 0)
				editor_camera->MoveDown(-motion.y);

			last_mouse_position = mouse_pos;

			//INTERNAL_LOG("%d", x_motion);
		}

		if (App->input->GetMouseWheel() == 1)
		{
			editor_camera->SetSize(editor_camera->size - (100 * App->GetDT()));
		}
		else if (App->input->GetMouseWheel() == -1)
		{
			editor_camera->SetSize(editor_camera->size + (100 * App->GetDT()));
		}
	
		App->window->GetCursor()->Hand();

		//if (App->input->GetKeyRepeat(SDL_SCANCODE_Z))
		//	editor_camera->MoveUp(cam_speed);

		//if (App->input->GetKeyRepeat(SDL_SCANCODE_X))
		//	editor_camera->MoveDown(cam_speed);

		if (App->input->GetKeyRepeat(SDL_SCANCODE_W))
			editor_camera->MoveUp(cam_speed);

		if (App->input->GetKeyRepeat(SDL_SCANCODE_S))
			editor_camera->MoveDown(cam_speed);

		if (App->input->GetKeyRepeat(SDL_SCANCODE_A))
			editor_camera->MoveLeft(cam_speed);

		if (App->input->GetKeyRepeat(SDL_SCANCODE_D))
			editor_camera->MoveRight(cam_speed);

		//editor_camera->Rotate(-App->input->GetMouseXMotion() * mou_speed, -App->input->GetMouseYMotion()*mou_speed);

		//App->gameobj->SetCanMove(false);
		
			
		//if (App->input->GetKeyRepeat(SDL_SCANCODE_LALT) || App->input->GetKeyRepeat(SDL_SCANCODE_RALT))
		//{
		//	if (App->input->GetKeyRepeat(SDL_SCANCODE_W))
		//		editor_camera->MoveFront(cam_speed);

		//	if (App->input->GetKeyRepeat(SDL_SCANCODE_S))
		//		editor_camera->MoveBack(cam_speed);

		//	editor_camera->Orbit(float3(0, 0, 0), -App->input->GetMouseXMotion()*mou_speed, -App->input->GetMouseYMotion()*mou_speed);
		//	editor_camera->Look(float3(0, 0, 0));

		//	App->window->GetCursor()->SizeAll();

		//	//App->gameobj->SetCanMove(false);
		//}
		
		
		
		//mouse_movement = false;
		//App->gameobj->SetCanMove(true);
	
	}

	if (App->input->GetKeyDown("f"))
	{
		editor_camera->Focus(float3(0, 0, 0), 10);
	}

	return ret;
}

Camera2D::Camera2D()
{
	frustum.SetOrthographic(1920, 1080);
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	frustum.SetPos(float3(0, 1, -1));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	SetNearPlaneDistance(0.1f);
	SetFarPlaneDistance(10000.0f);
	//SetAspectRatio(1.3f);
	//SetFOV(60);

	render_tex = new RenderTexture();
}

void Camera2D::CleanUp()
{
	render_tex->CleanUp();
	RELEASE(render_tex);
}

void Camera2D::Bind(uint width, uint heigth)
{
	if(render_tex != nullptr)
		render_tex->Bind(width, heigth);
}

void Camera2D::Unbind()
{
	if(render_tex != nullptr)
		render_tex->Unbind();
}

uint Camera2D::GetTextId()
{
	uint ret = 0;

	if (render_tex != nullptr)
		ret = render_tex->GetTextureID();

	return ret;
}

void Camera2D::SetPosition(const float3 & pos)
{
	frustum.SetPos(pos);
}

const float3 Camera2D::GetPosition()
{
	return frustum.Pos();
}

void Camera2D::SetZDir(const float3 & front)
{
	frustum.SetFront(front.Normalized());
}

void Camera2D::SetYDir(const float3 & front)
{
	frustum.SetUp(front.Normalized());
}

float3 Camera2D::GetZDir()
{
	return frustum.Front();
}

float3 Camera2D::GetYDir()
{
	return frustum.Up();
}

void Camera2D::GetCorners(float3* corners)
{
	frustum.GetCornerPoints(corners);
}

void Camera2D::SetNearPlaneDistance(const float & set)
{
	frustum.SetViewPlaneDistances(set, frustum.FarPlaneDistance());
}

void Camera2D::SetFarPlaneDistance(const float & set)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), set);
}

void Camera2D::SetViewportSize(float width, float height)
{
	if (width > 0 && height > 0 && size > 0)
	{
		frustum.SetOrthographic(width * size, height * size);

		viewport_size = float2(width, height);
	}
}

void Camera2D::SetSize(float _size)
{
	if (_size > 0)
	{
		size = _size;

		SetViewportSize(viewport_size.x, viewport_size.y);
	}
}

const float Camera2D::GetNearPlaneDistance() const
{
	return frustum.NearPlaneDistance();
}

const float Camera2D::GetFarPlaneDistance() const
{
	return frustum.FarPlaneDistance();
}

const float Camera2D::GetVerticalFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

const float Camera2D::GetHorizontalFOV() const
{
	return frustum.HorizontalFov() * RADTODEG;
}

const float4x4 Camera2D::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

const float4x4 Camera2D::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

const float4x4 Camera2D::GetOpenGLViewMatrix() const
{
	float4x4 view = frustum.ViewMatrix();
	return view.Transposed();
}

const float4x4 Camera2D::GetOpenGLProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed();
}

void Camera2D::MoveFront(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.Front() * speed;
	frustum.Translate(movement);

	UpdateTransform();
}

void Camera2D::MoveBack(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.Front() * speed;
	frustum.Translate(movement);

	UpdateTransform();
}

void Camera2D::MoveRight(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.WorldRight() * speed;
	frustum.Translate(movement);

	UpdateTransform();
}

void Camera2D::MoveLeft(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.WorldRight() * speed;
	frustum.Translate(movement);

	UpdateTransform();
}

void Camera2D::MoveUp(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += float3::unitY * speed;
	frustum.Translate(movement);

	UpdateTransform();
}

void Camera2D::MoveDown(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= float3::unitY * speed;
	frustum.Translate(movement);

	UpdateTransform();
}

void Camera2D::Orbit(const float3 & rotate_center, const float & motion_x, const float & motion_y)
{
	float3 distance = frustum.Pos() - rotate_center;

	Quat X(frustum.WorldRight(), motion_y);
	Quat Y(frustum.Up(), motion_x);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	frustum.SetPos(distance + rotate_center);
}

void Camera2D::Rotate(const float & motion_x, const float & motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	frustum.SetFront(rotation_x.Mul(frustum.Front()).Normalized());
	frustum.SetUp(rotation_x.Mul(frustum.Up()).Normalized());

	Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), motion_y);
	frustum.SetFront(rotation_y.Mul(frustum.Front()).Normalized());
	frustum.SetUp(rotation_y.Mul(frustum.Up()).Normalized());
}

void Camera2D::Look(const float3 & look_pos)
{
	float3 dir = look_pos - frustum.Pos();

	float3x3 direction_matrix = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(direction_matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(direction_matrix.MulDir(frustum.Up()).Normalized());
}

bool Camera2D::CheckInsideFrustum(const AABB & box)
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

void Camera2D::SetFrustumCulling(bool set)
{
	frustum_culling = set;
}

bool Camera2D::GetFrustumCulling()
{
	return frustum_culling;
}


Frustum Camera2D::GetFrustum()
{
	return frustum;
}

void Camera2D::UpdateTransform()
{
	frustum.SetFront(frustum.Front());
	frustum.SetUp(frustum.Up());
}

void Camera2D::Focus(const float3 & focus_center, const float & distance)
{
	float3 dir = frustum.Pos() - focus_center;
	frustum.SetPos(dir.Normalized() * distance);

	Look(focus_center);
}

void Camera2D::Focus(const AABB & aabb)
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

bool RenderTexture::Create(uint _width, uint _height)
{
	bool ret = true;

	width = _width;
	height = _height;

	if (width < 1)
		width = 1;

	if (height < 1)
		height = 1;

	// Create MSAA framebufer
	fbo_msaa_id = App->renderer3D->GenFrameBuffer();
	App->renderer3D->BindFrameBuffer(fbo_msaa_id);

	// Create a multisampled color attachment texture
	texture_msaa_id = App->renderer3D->GenTexture();
	App->renderer3D->BindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_msaa_id);
	App->renderer3D->Set2DMultisample(current_msaa_samples, width, height);
	App->renderer3D->UnbindTexture(GL_TEXTURE_2D_MULTISAMPLE);

	App->renderer3D->SetFrameBufferTexture2D(GL_TEXTURE_2D_MULTISAMPLE, texture_msaa_id);

	// Create a renderbuffer for depth and stencil
	rbo_id = App->renderer3D->GenRenderBuffer();
	App->renderer3D->BindRenderBuffer(rbo_id);
	App->renderer3D->RenderStorageMultisample(current_msaa_samples, width, height);
	App->renderer3D->UnbindRenderBuffer();
	
	App->renderer3D->RenderFrameBuffer(rbo_id);
	App->renderer3D->UnbindFrameBuffer();

	// Configure post-processing framebuffer
	fbo_id = App->renderer3D->GenFrameBuffer();
	App->renderer3D->BindFrameBuffer(fbo_id);

	// Create the color attachment texture
	texture_id = App->renderer3D->GenTexture();
	App->renderer3D->BindTexture(texture_id);
	
	// Set Parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	App->renderer3D->SetFrameBufferTexture2D(GL_TEXTURE_2D, texture_id);

	App->renderer3D->UnbindTexture();

	created = true;

	return ret;
}

void RenderTexture::Resize(uint width, uint height)
{
	Destroy();

	Create(width, height);
}

void RenderTexture::Bind(uint _width, uint _height)
{
	if (!created)
		Create(_width, _height);

	if (width != _width || height != _height)
		Resize(_width, _height);
	
	App->renderer3D->BindFrameBuffer(fbo_msaa_id);
	App->renderer3D->SetViewport(0, 0, width, height);
	App->renderer3D->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::Unbind()
{
	App->renderer3D->BindFrameBuffer(GL_READ_FRAMEBUFFER, fbo_msaa_id);
	App->renderer3D->BindFrameBuffer(GL_DRAW_FRAMEBUFFER, fbo_id);
	//
	//App->renderer3D->BlitFrameBuffer(0, 0, width, height);

	glBlitFramebuffer(0, 0, width, height,  // src rect
		0, 0, width, height,  // dst rect
		GL_COLOR_BUFFER_BIT, // buffer mask
		GL_NEAREST); // scale filter

	App->renderer3D->UnbindFrameBuffer();
	App->renderer3D->SetViewport(last_x, last_y, last_width, last_height);
}

void RenderTexture::ChangeMSAALevel(int MSAA_level)
{
	Destroy();

	current_msaa_samples = MSAA_level;

	Create(width, height);
}

void RenderTexture::Destroy()
{
	App->renderer3D->DeleteTexture(texture_id);
	App->renderer3D->DeleteTexture(texture_msaa_id);
	App->renderer3D->DeleteFrameBuffer(fbo_id);
	App->renderer3D->DeleteFrameBuffer(fbo_msaa_id);
	App->renderer3D->DeleteFrameBuffer(rbo_id);

	texture_id = 0;
	texture_msaa_id = 0;
	fbo_id = 0;
	fbo_msaa_id = 0;
	rbo_id = 0;

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
