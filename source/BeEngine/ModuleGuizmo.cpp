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

ModuleGuizmo::ModuleGuizmo() : Module()
{
}

ModuleGuizmo::~ModuleGuizmo()
{
}

bool ModuleGuizmo::Awake()
{
	bool ret = true;

	position_guizmo = (PositionGuizmo*)AddGuizmo(new PositionGuizmo());
	(GridGuizmo*)AddGuizmo(new GridGuizmo());

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
			
			float relative_size = 1;

			if ((*it)->keep_size)
				relative_size = App->camera->GetEditorCamera()->GetSize();

			(*it)->Render(relative_size);
		}
	}
}

void ModuleGuizmo::RenderSelectedGameObjectGuizmos()
{
	if (App->gameobject->GetSelectedGameObjectsCount() == 1)
	{
		GameObject* selected = App->gameobject->GetSelectedGameObjects()[0];

		std::vector<GameObjectComponent*> components = selected->GetComponents();

		for (std::vector<GameObjectComponent*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			(*it)->DrawGuizmos();
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
