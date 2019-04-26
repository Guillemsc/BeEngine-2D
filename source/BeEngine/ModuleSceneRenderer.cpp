#include <limits>

#include "ModuleSceneRenderer.h"
#include "App.h"
#include "ModuleRenderer.h"
#include "Renderer.h"
#include "ModuleCamera.h"
#include "LineRenderer.h"
#include "QuadRenderer.h"
#include "DinamicTriangleRenderer.h"
#include "StaticRenderer.h"
#include "DynamicGridRenderer.h"
#include "ModuleWindow.h"
#include "ModuleGuizmo.h"
#include "ModuleEditor.h"
#include "ModulePhysics.h"
#include "ModuleState.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ModuleSceneRenderer::ModuleSceneRenderer() : Module()
{
}

ModuleSceneRenderer::~ModuleSceneRenderer()
{
}

bool ModuleSceneRenderer::Awake()
{
	bool ret = true;

	line_renderer = (LineRenderer*)AddRenderer(new LineRenderer());
	quad_renderer = (QuadRenderer*)AddRenderer(new QuadRenderer());
	triangle_renderer = (DinamicTriangleRenderer*)AddRenderer(new DinamicTriangleRenderer());

	static_renderer = (StaticRenderer*)AddRenderer(new StaticRenderer());

	grid_renderer = (DynamicGridRenderer*)AddRenderer(new DynamicGridRenderer());

	// Far plane 9 999 999
	layer_space_guizmos = LayerSpace(110000, 100000, 10000);
	layer_space_component_scene = LayerSpace(5010000, 5000000, 10000);
	layer_space_grid = LayerSpace(4000001, 4000000, 1);

	return ret;
}

bool ModuleSceneRenderer::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleSceneRenderer::PostUpdate()
{
	bool ret = true;

	// ---------------------------------------------------------------------

	RenderOnCameras();

	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------

	App->editor->RenderEditor();

	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------

	App->renderer->SwapWindowBuffers();

	// ---------------------------------------------------------------------

	return ret;
}

bool ModuleSceneRenderer::CleanUp()
{
	bool ret = true;

	DestroyAllRenderers();

	return ret;
}

Renderer * ModuleSceneRenderer::AddRenderer(Renderer * gm)
{
	Renderer* ret = nullptr;

	if (gm != nullptr)
	{
		gm->Start();

		renderers.push_back(gm);

		ret = gm;
	}

	return ret;
}

void ModuleSceneRenderer::RenderRenderers(Camera2D* camera)
{
	float4x4 view_mat = camera->GetOpenGLViewMatrix();
	float4x4 projection_mat = camera->GetOpenGLProjectionMatrix();

	for (std::vector<Renderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it)
	{
		(*it)->Render(view_mat, projection_mat);
	}	
}

void ModuleSceneRenderer::DestroyAllRenderers()
{
	for (std::vector<Renderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	renderers.clear();
}

void ModuleSceneRenderer::RenderOnCameras()
{
	std::vector<Camera2D*> cameras = App->camera->GetCameras();

	for (std::vector<Camera2D*>::iterator it = cameras.begin(); it != cameras.end(); ++it)
	{
		Camera2D* curr_camera = (*it);

		bool is_editor_camera = false;
		if (curr_camera == App->camera->GetEditorCamera())
			is_editor_camera = true;

		if (!is_editor_camera || App->state->GetEngineState() != EngineState::ENGINE_STATE_BUILD)
		{
			curr_camera->Bind();

			if (is_editor_camera)
			{
				App->guizmo->RenderGuizmos();
			}

			RenderRenderers(curr_camera);

			curr_camera->Unbind();
		}
	}
}

LayerSpace::LayerSpace()
{
}

LayerSpace::LayerSpace(float from, float to, uint max_layer_level)
{
	this->from = from;
	this->to = to;
	this->max_layer_level = max_layer_level;
}

float LayerSpace::GetLayerValue(uint layer_level)
{
	if (layer_level < max_layer_level)
	{
		float difference = to - from;

		double max_float = (float)max_layer_level;

		double val = (difference * layer_level) / max_float;

		return val + from;
	}

	return from;
}
