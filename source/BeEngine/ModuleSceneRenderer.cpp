#include "ModuleSceneRenderer.h"
#include "App.h"
#include "ModuleRenderer.h"
#include "Renderer.h"
#include "ModuleCamera.h"
#include "LineRenderer.h"
#include "QuadRenderer.h"
#include "DinamicTriangleRenderer.h"
#include "StaticSpriteRenderer.h"
#include "ModuleWindow.h"
#include "ModuleGuizmo.h"
#include "ModuleEditor.h"

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

	static_sprite_renderer = (StaticSpriteRenderer*)AddRenderer(new StaticSpriteRenderer());

	return ret;
}

bool ModuleSceneRenderer::PostUpdate()
{
	bool ret = true;

	// ---------------------------------------------------------------------

	App->camera->GetEditorCamera()->Bind(App->window->GetWindowSize().x, App->window->GetWindowSize().y);

	App->guizmo->RenderGuizmos();

	App->camera->GetEditorCamera()->Unbind();

	RenderOnCameras();

	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------

	App->editor->RenderEditor();

	// ---------------------------------------------------------------------

	App->renderer->SwapWindowBuffers();

	return ret;
}

bool ModuleSceneRenderer::CleanUp()
{
	bool ret = true;

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
	if (camera != nullptr)
	{
		camera->Bind(App->window->GetWindowSize().x, App->window->GetWindowSize().y);

		float4x4 view_mat = camera->GetOpenGLViewMatrix();
		float4x4 projection_mat = camera->GetOpenGLProjectionMatrix();

		for (std::vector<Renderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it)
		{
			(*it)->Render(view_mat, projection_mat);
		}

		camera->Unbind();
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

		RenderRenderers(curr_camera);
	}
}
