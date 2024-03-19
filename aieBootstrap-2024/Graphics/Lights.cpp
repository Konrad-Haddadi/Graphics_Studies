#include "Lights.h"
#include "Gizmos.h"
#include <string>
#include <imgui_glfw3.h>

void Lights::ImGUI_Functions(std::string _name, bool _canRemove)
{
	std::string name = _name;
	
	ImGui::Text(name.c_str());

	name += ": ";

	std::string nameColor = name + "Color";
	std::string namePos = name + "Position";
	std::string nameIntensity = name + "Intensity";

	if (_canRemove)
	{
		std::string nameRemove = "Remove " + name;
		ImGui::Checkbox(nameRemove.c_str(), &remove);
	}

	ImGui::ColorButton(ImVec4(color.x, color.y, color.z, 1));

	ImGui::SliderFloat3(nameColor.c_str(), &color.x, 0, 1);
	ImGui::SliderFloat3(namePos.c_str(), &direction.x, -50, 50);
	ImGui::SliderFloat(nameIntensity.c_str(), &intensity, 0, 100);
	
	ImGui::Text(" ");

	glm::vec3 dist = { direction.x, direction.y, direction.z };
	

	aie::Gizmos::addSphere(dist, intensity / 50, 10, 10, glm::vec4(color.x, color.y, color.z, 0.75f));

}
