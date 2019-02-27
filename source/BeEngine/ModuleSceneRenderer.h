#ifndef __MODULE_SCENE_RENDERER_H__
#define __MODULE_SCENE_RENDERER_H__

#include "Module.h"

class Renderer;
class LineRenderer;
class QuadRenderer;
class DinamicTriangleRenderer;
class DynamicGridRenderer;
class StaticSpriteRenderer;
class Camera2D;

class LayerSpace
{
	friend class ModuleSceneRenderer;

private:
	LayerSpace();
	LayerSpace(float from, float to, uint max_layer_level);

public:
	float GetLayerValue(uint layer_level);

private:
	float from = 0.0f;
	float to = 0.0f;
	uint max_layer_level = 0;
};

class ModuleSceneRenderer : public Module
{
public:
	ModuleSceneRenderer();
	~ModuleSceneRenderer();

	bool Awake();
	bool PostUpdate();
	bool CleanUp();

public:
	LineRenderer*	         line_renderer = nullptr;
	QuadRenderer*	         quad_renderer = nullptr;
	DinamicTriangleRenderer* triangle_renderer = nullptr;
	DynamicGridRenderer*	 grid_renderer = nullptr;

	StaticSpriteRenderer*    static_sprite_renderer = nullptr;

public:
	LayerSpace layer_space_guizmos;
	LayerSpace layer_space_component_sprite;

private:
	Renderer* AddRenderer(Renderer* gm);
	void RenderRenderers(Camera2D* camera);
	void DestroyAllRenderers();

	void RenderOnCameras();

private:
	std::vector<Renderer*> renderers;

};

#endif // !__MODULE_SCENE_RENDERER_H__