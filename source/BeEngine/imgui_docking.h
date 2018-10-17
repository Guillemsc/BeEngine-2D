#ifndef IMGUIDOCK_H_
#define IMGUIDOCK_H_

#ifndef IMGUI_API
#include  "imgui.h"
#endif //IMGUI_API

#include "ModuleJson.h"

typedef enum ImGuiDockSlot
{
	ImGuiDockSlot_Left = 0,
	ImGuiDockSlot_Right,
	ImGuiDockSlot_Top,
	ImGuiDockSlot_Bottom,
	ImGuiDockSlot_Tab,

	ImGuiDockSlot_Float,
	ImGuiDockSlot_None

} ImGuiDockSlot;

namespace ImGui
{
	IMGUI_API void BeginDockspace();
	IMGUI_API void EndDockspace();
	IMGUI_API void ShutdownDock();
	IMGUI_API void SetNextDock(ImGuiDockSlot slot);
	IMGUI_API bool BeginDock(const char* label, bool noTab = false, bool* opened = NULL, bool locked = false, ImGuiWindowFlags extra_flags = 0, const ImVec2& default_size = ImVec2(-1, -1));
	IMGUI_API void EndDock();
	IMGUI_API void SetDockActive();
	IMGUI_API void DockDebugWindow();
	void SaveLayout(JSON_Doc* json, const char* layout_name);
	bool LoadLayout(JSON_Doc* json, const char* layout_name);

} // namespace ImGui

extern bool gImGuiDockReuseTabWindowTextureIfAvailable; // [true] (used only when available)


#endif //IMGUIDOCK_H_