#include "ModuleGuizmo.h"
#include "App.h"
#include "ModuleCamera.h"
#include "PositionGuizmo.h"
#include "ModuleSceneRenderer.h"
#include "QuadRenderer.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "SceneWindow.h"
#include "ModuleGameObject.h"
#include "ComponentTransfrom.h"
#include "GridGuizmo.h"
#include "PhysicsPolygonGuizmo.h"

ModuleGuizmo::ModuleGuizmo() : Module()
{
}

ModuleGuizmo::~ModuleGuizmo()
{
}

bool ModuleGuizmo::Awake()
{
	bool ret = true;

	(GridGuizmo*)AddGuizmo(new GridGuizmo());
	position_guizmo = (PositionGuizmo*)AddGuizmo(new PositionGuizmo());
	physics_polygon_guizmo = (PhysicsPolygonGuizmo*)AddGuizmo(new PhysicsPolygonGuizmo());

	return ret;
}

bool ModuleGuizmo::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleGuizmo::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleGuizmo::Update()
{
	bool ret = true;

	// Temporal
	if (App->gameobject->GetSelectedGameObjectsCount() == 1)
	{
		position_guizmo->visible = true;

		GameObject* selected = App->gameobject->GetSelectedGameObjects()[0];

		float4x4 world_trans = selected->transform->GetWorldTransform();

		if (position_guizmo->UpdateTransform(world_trans))
		{
			selected->transform->SetWorldTransform(world_trans);
		}
	}
	else
		position_guizmo->visible = false;

	return ret;
}

bool ModuleGuizmo::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleGuizmo::CleanUp()
{
	bool ret = true;

	DestroyAllGuizmos();

	return ret;
}

void ModuleGuizmo::OnLoadConfig(JSON_Doc * config)
{
	render_all_physics_guizmos = config->GetBool("guizmos.render_all_physics_guizmos");
}

void ModuleGuizmo::OnSaveConfig(JSON_Doc * config)
{
	config->SetBool("guizmos.render_all_physics_guizmos", render_all_physics_guizmos);
}

Guizmo* ModuleGuizmo::AddGuizmo(Guizmo * add)
{
	Guizmo* ret = nullptr;

	if (add != nullptr)
	{
		guizmos.push_back(add);
		add->Start();

		ret = add;
	}

	return ret;
}

void ModuleGuizmo::DestroyAllGuizmos()
{
	for (std::vector<Guizmo*>::iterator it = guizmos.begin(); it != guizmos.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	guizmos.clear();
}

void ModuleGuizmo::RenderGuizmos()
{
	float relative_size = App->camera->GetEditorCamera()->GetSize();

	for (std::vector<Guizmo*>::iterator it = guizmos.begin(); it != guizmos.end(); ++it)
	{
		if ((*it)->visible)
		{			
			int handlers_count = (*it)->GetHandlersCount();

			for (int i = 0; i < handlers_count; ++i)
			{
				bool inside_window = App->editor->scene_window->GetMouseInsideWindow();

				GuizmoHandler* handler = (*it)->GetHandler(i);

				if (render_handlers)
					App->scene_renderer->quad_renderer->DrawQuad(handler->GetPos(), handler->GetSize(), float3(0, 122.0f / 255.0f, 204.0f / 255.0f), 0.5f);

				LineSegment ls = App->camera->GetEditorCamera()->ShootRay(App->editor->scene_window->GetSceneRect(), App->input->GetMouse());

				if (handler->CheckRay(ls))
				{
					if(inside_window)
						handler->hovered = true;

					if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && inside_window)
						handler->pressed = true;
				}
				else
					handler->hovered = false;

				if (handler->pressed)
				{
					if (App->input->GetMouseButton(SDL_BUTTON_LEFT) != KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) != KEY_DOWN)
					{
						handler->pressed = false;
					}
				}
			}
			
			(*it)->Render(relative_size);
		}
	}

	RenderSelectedGameObjectGuizmos(relative_size);
	RenderAllPhysicsGuizmos(relative_size);
}

void ModuleGuizmo::RenderSelectedGameObjectGuizmos(float relative_size)
{
	if (App->gameobject->GetSelectedGameObjectsCount() == 1)
	{
		GameObject* selected = App->gameobject->GetSelectedGameObjects()[0];

		std::vector<GameObjectComponent*> components = selected->GetComponents();

		for (std::vector<GameObjectComponent*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			(*it)->RenderGuizmos(relative_size);
		}
	}
}

void ModuleGuizmo::RenderAllPhysicsGuizmos(float relative_size)
{
	if (render_all_physics_guizmos)
	{
		std::vector<GameObject*> game_objects = App->gameobject->GetGameObjects();

		for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
		{
			std::vector<GameObjectComponent*> components = (*it)->GetComponents();

			for (std::vector<GameObjectComponent*>::iterator com = components.begin(); com != components.end(); ++com)
			{
				if ((*com)->GetGroup() == ComponentGroup::PHYSICS)
				{
					(*com)->RenderGuizmos(relative_size);
				}
			}
		}
	}
}

void ModuleGuizmo::SetRenderHandlers(bool set)
{
	render_handlers = set;
}

bool ModuleGuizmo::GetRenderHandlers() const
{
	return render_handlers;
}

void ModuleGuizmo::SetRenderAllPhysicsGuizmos(bool set)
{
	render_all_physics_guizmos = set;
}

bool ModuleGuizmo::GetRenderAllPhysicsGuizmos() const
{
	return render_all_physics_guizmos;
}
