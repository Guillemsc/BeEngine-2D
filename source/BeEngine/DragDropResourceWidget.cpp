#include "DragDropResourceWidget.h"
#include "Resource.h"

DragDropResourceWidget::DragDropResourceWidget()
{
}

bool DragDropResourceWidget::DrawDragSource(const std::vector<Resource*>& dragging_res)
{
	bool ret = false;

	if (dragging_res.size() > 0)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			int dummy = 0;

			ImGui::SetDragDropPayload(payload_name.c_str(), &dummy, sizeof(int));

			dragging = dragging_res;

			for (std::vector<Resource*>::iterator it = dragging.begin(); it != dragging.end(); ++it)
				ImGui::Text((*it)->GetDecomposedAssetFilepath().file_name.c_str());

			ImGui::EndDragDropSource();

			ret = true;
		}
	}

	return ret;
}

bool DragDropResourceWidget::DrawDragTarget(std::vector<Resource*>& dragging_res)
{
	bool ret = false;

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payload_name.c_str()))
		{
			dragging_res = dragging;

			ret = true;

			dragging.clear();
		}

		ImGui::EndDragDropTarget();
	}

	return ret;
}
