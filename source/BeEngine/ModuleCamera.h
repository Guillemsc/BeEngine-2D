#ifndef __MODULE_CAMERA3D_H__
#define __MODULE_CAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "GeometryMath.h"

class GameObject;
class RenderTexture;

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	void CleanUp();

	void Bind(uint width, uint height);
	void Unbind();
	void ChangeMSAALevel(int MSAA_level);

	uint GetTextureID() const;
	int GetMaxMSAALevel() const;
	int GetCurrentMSAALevel() const;

	uint GetWidth() const;
	uint GetHeight() const;

private:
	bool Create(uint width, uint height);
	void Resize(uint width, uint height);
	void Destroy();

private:
	uint fbo_id = 0;
	uint fbo_msaa_id = 0;
	uint texture_id = 0;
	uint rbo_id = 0;
	uint texture_msaa_id = 0;

	uint width = 0;
	uint height = 0;

	uint last_x = 0;
	uint last_y = 0;
	uint last_width = 0;
	uint last_height = 0;

	int max_msaa_samples = 2;
	int current_msaa_samples = 2;

	bool created = false;
};

class Camera2D
{
	friend class ModuleCamera;

private:
	void operator delete(void *) {}

public:
	Camera2D();

	void CleanUp();

	void Bind();
	void Unbind();
	uint GetTextId();

	void SetPosition(const float2& pos);
	const float2 GetPosition();

	void SetZDir(const float3& front);
	void SetYDir(const float3& up);
	float3 GetZDir();
	float3 GetYDir();

	std::vector<float2> GetCorners();

	void SetNearPlaneDistance(const float& set);
	void SetFarPlaneDistance(const float& set);
	void SetViewportSize(float width, float height);
	void SetSize(float size);

	float2 GetViewportSize() const;

	const float GetSize() const;
	const float GetNearPlaneDistance() const;
	const float GetFarPlaneDistance() const;

	const float4x4 GetViewMatrix() const;
	const float4x4 GetProjectionMatrix() const;
	const float4x4 GetOpenGLViewMatrix() const;
	const float4x4 GetOpenGLProjectionMatrix() const;

	void MoveFront(const float& speed);
	void MoveBack(const float& speed);
	void MoveRight(const float& speed);
	void MoveLeft(const float& speed);
	void MoveUp(const float& speed);
	void MoveDown(const float& speed);

	LineSegment ShootRay(const Rect& bounds_rect, const float2& shoot_pos);

	bool CheckInsideFrustum(const AABB& box);
	void SetFrustumCulling(bool set);
	bool GetFrustumCulling();

	Frustum GetFrustum();

private:
	void UpdateTransform();

private:
	Frustum		   frustum;
	float2		   viewport_size = float2::zero;
	float		   size = 1;

	bool	       frustum_culling = true;

	RenderTexture* render_tex = nullptr;
};

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);

	void UpdateEditorCameraInput();

	Camera2D* CreateCamera();
	void DestroyCamera(Camera2D* cam);
	void DestroyAllCameras();
	std::vector<Camera2D*> GetCameras();

	Camera2D* GetEditorCamera() const;

	void SetGameCamera(Camera2D* set);
	Camera2D* GetGameCamera() const;

	void SetWheelSpeed(const float& set);
	void SetCameraSpeed(const float& set);

	const float GetWheelSpeed() const;
	const float GetCameraSpeed() const;

private:
	Camera2D* editor_camera = nullptr;
	Camera2D* game_camera = nullptr;

	std::vector<Camera2D*> cameras;

	// Camera Movement
	bool  mouse_movement = false;
	float camera_speed = 0.0f;
	float wheel_speed = 0.0f;

	bool dragging = false;
};

#endif // !__MODULE_CAMERA3D_H__