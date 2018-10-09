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

	void Bind(uint x, uint y, uint width, uint height);
	void Unbind();
	void ChangeMSAALevel(int MSAA_level);

	uint GetTextureID() const;
	int GetMaxMSAALevel() const;
	int GetCurrentMSAALevel() const;

	uint GetWidth() const;
	uint GetHeight() const;

private:
	bool Create(uint x, uint y, uint width, uint height);
	void Resize(uint x, uint y, uint width, uint height);
	void Destroy();

private:
	uint fbo_id = 0;
	uint fbo_msaa_id = 0;
	uint texture_id = 0;
	uint rbo_id = 0;
	uint texture_msaa_id = 0;

	uint x = 0;
	uint y = 0;
	uint width = 0;
	uint height = 0;

	uint last_x = 0;
	uint last_y = 0;
	uint last_width = 0;
	uint last_height = 0;

	int max_msaa_samples = 8;
	int current_msaa_samples = 8;

	bool created = false;
};

class Camera3D
{
public:
	Camera3D();

	void CleanUp();

	void Bind(uint x, uint y, uint width, uint heigth);
	void Unbind();
	uint GetTextId();

	void SetPosition(const float3& pos);
	const float3 GetPosition();

	void SetZDir(const float3& front);
	void SetYDir(const float3& front);
	float3 GetZDir();
	float3 GetYDir();

	void GetCorners(float3* corners);

	void SetNearPlaneDistance(const float& set);
	void SetFarPlaneDistance(const float& set);
	void SetFOV(const float& set);
	void SetAspectRatio(const float& set);
	const float GetNearPlaneDistance() const;
	const float GetFarPlaneDistance() const;
	const float GetVerticalFOV() const;
	const float GetHorizontalFOV() const;
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

	void Orbit(const float3& rotate_center, const float& motion_x, const float& motion_y);
	void Rotate(const float& motion_x, const float& motion_y);
	void Focus(const float3& focus_center, const float& distance);
	void Focus(const AABB& aabb);

	void Look(const float3& look_pos);

	bool CheckInsideFrustum(const AABB& box);
	void SetFrustumCulling(bool set);
	bool GetFrustumCulling();

	Frustum GetFrustum();

private:
	Frustum		   frustum;
	float		   aspect_ratio = 0.0f;
	float          vertical_fov = 0.0f;

	bool	       frustum_culling = true;

	RenderTexture* render_tex = nullptr;
};

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D();
	~ModuleCamera3D();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);

	Camera3D* CreateCamera();
	void DestroyCamera(Camera3D* cam);
	void DestroyAllCameras();
	std::vector<Camera3D*> GetCameras();

	Camera3D* GetEditorCamera() const;

	void SetGameCamera(Camera3D* set);
	Camera3D* GetGameCamera() const;

	void SetMouseSensitivity(const float& set);
	void SetWheelSpeed(const float& set);
	void SetCameraSpeed(const float& set);

	const float GetMouseSensitivity() const;
	const float GetWheelSpeed() const;
	const float GetCameraSpeed() const;

private:
	Camera3D* editor_camera = nullptr;
	Camera3D* game_camera = nullptr;

	std::vector<Camera3D*> cameras;

	// Camera Movement
	bool  mouse_movement = false;
	float camera_speed = 0.0f;
	float wheel_speed = 0.0f;
	float mouse_sensitivity = 0.0f;
};

#endif // !__MODULE_CAMERA3D_H__