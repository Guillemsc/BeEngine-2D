#ifndef __MODULE_SCENE_RENDERER_H__
#define __MODULE_SCENE_RENDERER_H__

#include "Module.h"

class Renderer;
class LineRenderer;
class QuadRenderer;
class DinamicTriangleRenderer;
class StaticSpriteRenderer;
class Camera2D;

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

	StaticSpriteRenderer*    static_sprite_renderer = nullptr;

private:
	Renderer* AddRenderer(Renderer* gm);
	void RenderRenderers(Camera2D* camera);
	void DestroyAllRenderers();

	void RenderOnCameras();

private:
	std::vector<Renderer*> renderers;

};

#endif // !__MODULE_SCENE_RENDERER_H__