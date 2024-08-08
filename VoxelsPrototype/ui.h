#pragma once

#ifndef UI_H
#define UI_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

/* UI TODO
* - load and unload chunks of various types
* - change various parameters / options regarding camera and control 
* - display system information RAM VRAM
* - modular - each UI component called as single function within render() allowing for easy addition of new UI components
* - try to minimize passing parameters into render()
*/


class UI {
public:
		UI(GLFWwindow* window);
		void render(float fps, bool* renderChunks);
		void initialize(GLFWwindow* window);	
};

UI::UI(GLFWwindow* window) {
	initialize(window);
}

void UI::initialize(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::render(float fps, bool* renderChunks) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("UI Test");
	ImGui::Text("FPS: %.1f", fps);
	ImGui::Checkbox("Render Chunks", renderChunks);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#endif // !UI_H