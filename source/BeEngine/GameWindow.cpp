#include "GameWindow.h"
#include "App.h"
#include "ModuleCamera.h"
#include "ModuleEvent.h"
#include "ModuleState.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

GameWindow::GameWindow()
{
}

GameWindow::~GameWindow()
{
}

void GameWindow::Start()
{
	reference_size = float2(1280, 720);
}

void GameWindow::CleanUp()
{
}

void GameWindow::DrawEditor()
{
	Camera2D* game_camera = App->camera->GetGameCamera();

	float2 window_size = GetWindowSize();
	float2 window_pos = GetWindowPos();

	float2 image_pos = float2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
	float2 image_size = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 5);

	if (use_reference_size && !full_screen)
	{
		if (reference_size.x > 0 && reference_size.y > 0)
		{
			float wanted_ratio = reference_size.y / reference_size.x;
			float curr_ratio = image_size.y / image_size.x;

			if (curr_ratio > wanted_ratio)
			{
				image_size.y = image_size.x * wanted_ratio;
			}
			else if (curr_ratio < wanted_ratio)
			{
				image_size.x = image_size.y / wanted_ratio;
			}
		}
	}

	float2 game_view_center = float2(window_size.x * 0.5f, window_size.y * 0.5f);
	image_pos.x = game_view_center.x - image_size.x * 0.5f;
	image_pos.y = game_view_center.y - image_size.y * 0.5f;

	image_pos.y += 15.0f;

	game_viewport_rect = Rect(window_pos.x + image_pos.x, window_pos.y + image_pos.y, image_size.x, image_size.y);

	if (last_size.x != image_size.x || last_size.y != image_size.y)
	{
		App->event->SendEvent(new EventGameWindowResize(last_size, image_size));

		last_size = image_size;

		if (game_camera != nullptr)
			game_camera->SetViewportSize(image_size.x, image_size.y);
	}

	if(last_game_camera != game_camera && game_camera != nullptr)
		game_camera->SetViewportSize(image_size.x, image_size.y);

	if (game_camera != nullptr)
	{
		ImGui::SetCursorPos(ImVec2(image_pos.x, image_pos.y));
		ImGui::Image((void*)game_camera->GetTextId(), { image_size.x, image_size.y}, ImVec2(0, 1), ImVec2(1, 0));
	}

	last_game_camera = game_camera;

	if (ImGui::BeginMenuBar())
	{
		if(ImGui::Button("Full Screen Debug"))
		{
			App->state->SetEngineState(EngineState::ENGINE_STATE_BUILD);
		}

		ImGui::Checkbox("Use reference size", &use_reference_size);

		if (use_reference_size)
		{
			int values[2] { reference_size.x, reference_size.y };
			ImGui::DragInt2("", values, 1, 1, 9999999);

			reference_size.x = values[0];
			reference_size.y = values[1];

			if (reference_size.x < 1)
				reference_size.x = 1;

			if (reference_size.y < 1)
				reference_size.y = 1;
		}

		ImGui::EndMenuBar();
	}
}

ImGuiWindowFlags GameWindow::GetWindowFlags()
{
	return ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
}

Rect GameWindow::GetGameRect() const
{
	return game_viewport_rect;
}
