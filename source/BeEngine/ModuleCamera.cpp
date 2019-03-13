#include <limits>

#include "Globals.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "Cursor.h"
#include "imgui.h"
#include "ModuleRenderer.h"
#include "ModuleJson.h"
#include "ModuleAction.h"
#include "ModuleEditor.h"
#include "SceneWindow.h"
#include "imgui.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ModuleCamera::ModuleCamera() : Module()
{
	editor_camera = CreateCamera();
}

ModuleCamera::~ModuleCamera()
{}

bool ModuleCamera::Awake()
{
	bool ret = true;


	return ret;
}

bool ModuleCamera::Start()
{
	bool ret = true;
	return ret;
}

bool ModuleCamera::CleanUp()
{
	bool ret = true;

	DestroyAllCameras();

	return ret;
}

void ModuleCamera::OnLoadConfig(JSON_Doc * config)
{
	wheel_speed = config->GetNumber("camera.wheel_speed", 0.05f);

	float2 cam_pos = config->GetNumber2("camera.position");
	editor_camera->SetPosition(cam_pos);

	float camera_size = config->GetNumber("camera.size", 1.0f);
	editor_camera->SetSize(camera_size);
}

void ModuleCamera::OnSaveConfig(JSON_Doc * config)
{
	config->SetNumber("camera.wheel_speed", wheel_speed);
	config->SetNumber2("camera.position", editor_camera->GetPosition());
	config->SetNumber("camera.size", editor_camera->GetSize());
}

void ModuleCamera::UpdateEditorCameraInput()
{
	if (editor_camera != nullptr)
	{
		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			if (App->editor->scene_window->GetMouseInsideWindow())
				dragging = true;
		}

		if (dragging && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_DOWN)
			dragging = false;

		if (dragging)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

			float2 mouse_pos = App->input->GetMouse();

			float2 motion = float2(App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

			motion.x *= editor_camera->GetViewScale();

			if (motion.x > 0)
				editor_camera->MoveLeft(motion.x);
			if (motion.x < 0)
				editor_camera->MoveRight(-motion.x);

			motion.y *= editor_camera->size;

			if (motion.y > 0)
				editor_camera->MoveUp(motion.y);
			if (motion.y < 0)
				editor_camera->MoveDown(-motion.y);
		}

		if (App->editor->scene_window->GetMouseInsideWindow())
		{
			float speed = wheel_speed * editor_camera->size;

			if (App->input->GetMouseWheel() == 1)
			{
				editor_camera->SetSize(editor_camera->size - (speed));
			}
			else if (App->input->GetMouseWheel() == -1)
			{
				editor_camera->SetSize(editor_camera->size + (speed));
			}
		}
	}
}

Camera2D* ModuleCamera::CreateCamera()
{
	Camera2D* ret = nullptr;

	ret = new Camera2D;
	cameras.push_back(ret);

	return ret;
}

void ModuleCamera::DestroyCamera(Camera2D* cam)
{
	if (cam != nullptr)
	{
		for (std::vector<Camera2D*>::iterator it = cameras.begin(); it != cameras.end();)
		{
			if (cam == (*it))
			{
				(*it)->CleanUp();
				RELEASE(*it);

				cameras.erase(it);
				break;
			}
			else
				++it;
		}
	}
}

void ModuleCamera::DestroyAllCameras()
{
	for (std::vector<Camera2D*>::iterator it = cameras.begin(); it != cameras.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	cameras.clear();
}

std::vector<Camera2D*> ModuleCamera::GetCameras()
{
	return cameras;
}

Camera2D* ModuleCamera::GetEditorCamera() const
{
	return editor_camera;
}

void ModuleCamera::SetGameCamera(Camera2D * set)
{
	game_camera = set;
}

Camera2D* ModuleCamera::GetGameCamera() const
{
	return game_camera;
}

void ModuleCamera::SetWheelSpeed(const float& set)
{
	wheel_speed = set;
}

void ModuleCamera::SetCameraSpeed(const float& set)
{
	camera_speed = set;
}

const float ModuleCamera::GetWheelSpeed() const
{
	return wheel_speed;
}

const float ModuleCamera::GetCameraSpeed() const
{
	return camera_speed;
}

// -----------------------------------------------------------------
bool ModuleCamera::Update()
{
	bool ret = true;

	UpdateEditorCameraInput();

	return ret;
}

Camera2D::Camera2D()
{
	SetViewportSize(1280, 720);
	SetSize(1);

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumLeftHanded);

	SetPosition(float2(0, 0));
	SetZDir(float3(0, 0, 1));
	SetYDir(float3(0, 1, 0));

	SetNearPlaneDistance(1.0f);
	SetFarPlaneDistance(9999999);

	render_tex = new RenderTexture();
}

void Camera2D::CleanUp()
{
	render_tex->CleanUp();
	RELEASE(render_tex);
}

void Camera2D::Bind()
{
	if(render_tex != nullptr)
		render_tex->Bind(viewport_size.x, viewport_size.y);
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

void Camera2D::SetPosition(const float2& pos)
{
	frustum.SetPos(float3(pos.x, pos.y, -1));
}

const float2 Camera2D::GetPosition()
{
	float3 pos = frustum.Pos();
	return float2(pos.x, pos.y);
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

std::vector<float2> Camera2D::GetCorners()
{
	std::vector<float2> ret;

	float2 center = GetPosition();
	float2 size = GetViewportSize();
	float2 half_size = size * 0.5f;

	ret.push_back(float2(center.x - half_size.x, center.y + half_size.y));
	ret.push_back(float2(center.x - half_size.x, center.y - half_size.y));
	ret.push_back(float2(center.x + half_size.x, center.y - half_size.y));
	ret.push_back(float2(center.x + half_size.x, center.y + half_size.y));

	return ret;
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
		float ratio = 1;
		
		if(height > 0)
			ratio = height / width;

		frustum.SetOrthographic(1280 * size, 1280 * size * ratio);

		viewport_size = float2(width, height);
	}
}

void Camera2D::SetSize(float _size)
{	
	size = _size;

	if (size < 0.0001f)
		size = 0.0001f;

	SetViewportSize(viewport_size.x, viewport_size.y);
}

float2 Camera2D::GetViewportSize() const
{
	return float2(frustum.OrthographicWidth(), frustum.OrthographicHeight());
}

const float Camera2D::GetSize() const
{
	return size;
}

const float Camera2D::GetNearPlaneDistance() const
{
	return frustum.NearPlaneDistance();
}

const float Camera2D::GetFarPlaneDistance() const
{
	return frustum.FarPlaneDistance();
}

const float Camera2D::GetViewScale() const
{
	return size * (1280.0f / viewport_size.x);
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

LineSegment Camera2D::ShootRay(const Rect& bounds_rect, const float2& shoot_pos)
{
	float first_normalized_x = (shoot_pos.x - bounds_rect.left) / (bounds_rect.right - bounds_rect.left);
	float first_normalized_y = (shoot_pos.y - bounds_rect.top) / (bounds_rect.bottom - bounds_rect.top);

	float normalized_x = (first_normalized_x * 2) - 1;
	float normalized_y = 1 - (first_normalized_y * 2);

	LineSegment ret = frustum.UnProjectLineSegment(normalized_x, normalized_y);

	return ret;
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
	fbo_msaa_id = App->renderer->GenFrameBuffer();
	App->renderer->BindFrameBuffer(fbo_msaa_id);

	// Create a multisampled color attachment texture
	texture_msaa_id = App->renderer->GenTexture();
	App->renderer->BindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_msaa_id);
	App->renderer->Set2DMultisample(current_msaa_samples, width, height);
	App->renderer->UnbindTexture(GL_TEXTURE_2D_MULTISAMPLE);

	App->renderer->SetFrameBufferTexture2D(GL_TEXTURE_2D_MULTISAMPLE, texture_msaa_id);

	// Create a renderbuffer for depth and stencil
	rbo_id = App->renderer->GenRenderBuffer();
	App->renderer->BindRenderBuffer(rbo_id);
	App->renderer->RenderStorageMultisample(current_msaa_samples, width, height);
	App->renderer->UnbindRenderBuffer();
	
	App->renderer->RenderFrameBuffer(rbo_id);
	App->renderer->UnbindFrameBuffer();

	// Configure post-processing framebuffer
	fbo_id = App->renderer->GenFrameBuffer();
	App->renderer->BindFrameBuffer(fbo_id);

	// Create the color attachment texture
	texture_id = App->renderer->GenTexture();
	App->renderer->BindTexture(texture_id);
	
	// Set Parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	App->renderer->SetFrameBufferTexture2D(GL_TEXTURE_2D, texture_id);

	App->renderer->UnbindTexture();

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
	
	App->renderer->BindFrameBuffer(fbo_msaa_id);
	App->renderer->SetViewport(0, 0, width, height);
	App->renderer->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::Unbind()
{
	App->renderer->BindFrameBuffer(GL_READ_FRAMEBUFFER, fbo_msaa_id);
	App->renderer->BindFrameBuffer(GL_DRAW_FRAMEBUFFER, fbo_id);

	App->renderer->BlitFrameBuffer(0, 0, width, height);

	App->renderer->UnbindFrameBuffer();
	App->renderer->SetViewport(last_x, last_y, last_width, last_height);
}

void RenderTexture::ChangeMSAALevel(int MSAA_level)
{
	Destroy();

	current_msaa_samples = MSAA_level;

	Create(width, height);
}

void RenderTexture::Destroy()
{
	App->renderer->DeleteTexture(texture_id);
	App->renderer->DeleteTexture(texture_msaa_id);
	App->renderer->DeleteFrameBuffer(fbo_id);
	App->renderer->DeleteFrameBuffer(fbo_msaa_id);
	App->renderer->DeleteFrameBuffer(rbo_id);

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
