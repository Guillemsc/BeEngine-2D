#include "ModuleGuizmo.h"
#include "App.h"
#include "ModuleCamera.h"
#include "PositionGuizmo.h"
#include "ModuleRenderer.h"
#include "QuadRenderer.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"

ModuleGuizmo::ModuleGuizmo()
{
}

ModuleGuizmo::~ModuleGuizmo()
{
}

bool ModuleGuizmo::Awake()
{
	bool ret = true;

	AddGuizmo(new PositionGuizmo());

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

void ModuleGuizmo::AddGuizmo(Guizmo * add)
{
	if (add != nullptr)
	{
		guizmos.push_back(add);
		add->Start();
	}
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
				GuizmoHandler* handler = (*it)->GetHandler(i);

				if (render_handlers)
					App->renderer->quad_renderer->DrawQuad(handler->GetPos(), handler->GetSize(), float3(0, 122.0f / 255.0f, 204.0f / 255.0f), 0.5f);

				LineSegment ls = App->camera->GetEditorCamera()->ShootRay(App->editor->scene_window->GetWindowRect(), App->input->GetMouse());

				if (handler->CheckRay(ls))
					handler->hovered = true;
				else
					handler->hovered = false;
			}
			
			float relative_size = 1;

			if ((*it)->keep_size)
				relative_size = App->camera->GetEditorCamera()->GetSize();

			(*it)->Render(relative_size);
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
