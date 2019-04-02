#include "DragDropGameObjectWidget.h"
#include "imgui.h"
#include "GameObject.h"

DragDropGameObjectWidget::DragDropGameObjectWidget()
{
}

bool DragDropGameObjectWidget::DrawDragSource(const std::vector<GameObject*>& dragging_gos)
{
	bool ret = false;

	if (dragging_gos.size() > 0)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			int dummy = 0;

			ImGui::SetDragDropPayload(payload_name.c_str(), &dummy, sizeof(int));
			
			dragging = dragging_gos;

			for (std::vector<GameObject*>::iterator it = dragging.begin(); it != dragging.end(); ++it)
				ImGui::Text((*it)->GetName().c_str());

			ImGui::EndDragDropSource();
			
			ret = true;
		}
	}

	return ret;
}

bool DragDropGameObjectWidget::DrawDragTarget(std::vector<GameObject*>& dragging_gos)
{
	bool ret = false;

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payload_name.c_str()))
		{
			dragging_gos = dragging;

			ret = true;

			dragging.clear();
		}

		ImGui::EndDragDropTarget();
	}
	
	return ret;
}
