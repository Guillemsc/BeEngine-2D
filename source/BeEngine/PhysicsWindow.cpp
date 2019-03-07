#include "PhysicsWindow.h"
#include "App.h"
#include "ModulePhysics.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

PhysicsWindow::PhysicsWindow()
{
}

PhysicsWindow::~PhysicsWindow()
{
}

void PhysicsWindow::Start()
{
}

void PhysicsWindow::CleanUp()
{
}

void PhysicsWindow::DrawEditor()
{
	float2 gravity = App->physics->GetWorldGravity();

	if (ImGui::DragFloat2("World gravity", (float*)&gravity, 0.1f))
	{
		App->physics->SetWorldGravity(gravity);
	}

	std::string bodies_text = "Physics Bodies: " + std::to_string(App->physics->GetPhysicsBodiesCount());
	std::string shapes_text = "Physics Shapes: " + std::to_string(App->physics->GetPhysicsShapesCount());

	ImGui::Text(bodies_text.c_str());
	ImGui::Text(shapes_text.c_str());
}

ImGuiWindowFlags PhysicsWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}
