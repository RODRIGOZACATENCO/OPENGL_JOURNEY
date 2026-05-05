//
// Created by rodrigo on 30/04/2026.
//

#include <glad/glad.h>
#include <filesystem>

#include "../include/MainWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <random>
#include<glm/glm.hpp>
glm::vec3 generateRandomColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	return {dis(gen) / 255.0f, dis(gen) / 255.0f, dis(gen) / 255.0f};
}


//default scene has 3 objects, 2 pyramind and 1 cube


//given the scene to render, the window will render all objects
//mainWindow always initializes with a default window named "default"
void MainWindow::use(std::string *scene_name) {
	std::string default_scene_name="default";
	if (scene_name == nullptr) scene_name=&default_scene_name;
	//the renderer will accept the scene and draw it in the screen
	renderer->setCurrentScene(scene_name_to_scene_object[*scene_name].get());
	std::string error;
	if (isWindowReady(&error)) {
		while (!glfwWindowShouldClose(window))
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			processInput();
			//SET THE SCENE TO DRAW
			renderer->mainRenderPass();
			gui.showMainWindowGUI();
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}else {
		std::cout << error << std::endl;
	}



}




void MainWindow::cleanup()
{

}


void MainWindow::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	MainWindow* instance = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));

	// 4. Forward the call to the non-static member
	if (instance) {
		instance->onFramebufferSize();
	}
}

void MainWindow::onFramebufferSize() {
	renderer->setScreenSize(width, height);
	renderer->resizeFramebuffer();
}

void MainWindow::mainWindowMouseCallback(GLFWwindow* window, int button, int action, int mods) {
	// 3. Retrieve the pointer we stored earlier
	MainWindow* instance = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));

	// 4. Forward the call to the non-static member
	if (instance) {
		instance->onMouseButton(button, action, mods);
	}
}

void MainWindow::onMouseButton(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//todo
			std::cout <<"fjsnf"<<std::endl;
		}
}


void MainWindow::processInput()
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

bool MainWindow::isWindowReady(std::string* out_error) const {
	auto fail = [&](const std::string& msg) {
		if (out_error) *out_error = msg;
		return false;
	};

	if (!window)                        return fail("No GLFW window set.");
	if (!renderer)                      return fail("No renderer set.");
	if (width <= 0 || height <= 0)      return fail("Invalid window dimensions.");
	if (scene_name_to_scene_object.empty())    return fail("No scenes added.");

	bool has_active_scene = false;
	for (auto& [name, scene] : scene_name_to_scene_object) {
		if (scene != nullptr) { has_active_scene = true; break; }
	}
	if (!has_active_scene)              return fail("All scenes are null.");

	return true;
}