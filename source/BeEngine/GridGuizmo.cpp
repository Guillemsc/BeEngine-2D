#include "GridGuizmo.h"
#include "ModuleSceneRenderer.h"
#include "App.h"
#include "DynamicGridRenderer.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

void GridGuizmo::Start()
{
}

void GridGuizmo::Render(float relative_size)
{
	App->scene_renderer->grid_renderer->DrawGrid();
}
