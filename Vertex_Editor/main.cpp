#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Shared headers from your global directory
#include <random>

#include "MainWindow.h"

#include "ShaderHandler.h"
#include "TextureHandler.h"

#include "Mesh.h"
#include "GUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



std::vector<float> cube_vertices = {
    -1, -1, -1, // 0 bottom left back
    -1, -1,  1, // 1 bottom left front
     1, -1, -1, // 2 bottom right back
     1, -1,  1, // 3 bottom right front
    -1,  1, -1, // 4 top left back
    -1,  1,  1, // 5 top left front
     1,  1, -1, // 6 top right back
     1,  1,  1, // 7 top right front
};

std::vector<int> faces = {
    0, 2, 1,  1, 2, 3, // Bottom
    0, 4, 2,  2, 4, 6, // Back
    2, 6, 3,  3, 6, 7, // Right
    1, 3, 5,  3, 7, 5, // Front
    5, 7, 4,  6, 4, 7, // Top
    0, 1, 4,  5, 4, 1, // Left
};
std::vector<float> piramid_vertices = {
    -1, -1, -1, // 0 bottom left back
    -1, -1,  1, // 1 bottom left front
     1, -1, -1, // 2 bottom right back
     1, -1,  1, // 3 bottom right front
     0,  1,  0, // 4 apex
};

std::vector<int> piramid_faces = {
    0, 2, 1,  1, 2, 3, // Bottom
    0, 1, 4,            // Left
    1, 3, 4,            // Front
    3, 2, 4,            // Right
    2, 0, 4,            // Back
};

bool use_main_buffer = true;
Mesh mesh1(&cube_vertices, &faces);
Mesh mesh2(&piramid_vertices, &piramid_faces);
Mesh mesh3(&piramid_vertices, &piramid_faces);
GUI gui;
MainWindow main_window(nullptr);


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	main_window.resizeFramebuffer(width, height);
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		use_main_buffer = !use_main_buffer;
	}
}

void processInput(GLFWwindow *window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
void mainWindowMouseCallback(GLFWwindow *window, int button, int action, int mods);


int main()
{
	std::cout<<ROOT_DIR;
	// GLFW Init
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(1200, 800, "VertexEditor", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// GLAD Init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mainWindowMouseCallback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");

	int frameBufferWidth = 1200;
	int frameBufferHeight = 800;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);

	main_window.setWindow(window);
	main_window.setGui(&gui);

	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)frameBufferWidth / (float)frameBufferHeight, 0.1f, 100.0f);
	main_window.addMesh(&mesh1, "cube", glm::mat4(1.0f));
	main_window.addMesh(&mesh2, "pyramid", glm::mat4(1.0f));
	main_window.addMesh(&mesh3, "pyramid2", glm::mat4(1.0f));

	main_window.setViewMatrix(view);


	main_window.setProjectionMatrix(projection);

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	gui.main_state.isEdgeSelectionActive = true; // sets the initial state of the face selection button to active
	gui.currentState=EDGE_EDITING;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);


		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1, 0, -2));
		model = glm::scale(model, glm::vec3(0.7f));

		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.3, 0.7, 0));
		main_window.setModelMatrix("cube", model);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1, 0, -2));
		model = glm::scale(model, glm::vec3(0.7f));

		model = glm::rotate(model, -(float)glfwGetTime(), glm::vec3(0.3, 0.7, 0));
		main_window.setModelMatrix("pyramid", model);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 1, -6));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, -(float)glfwGetTime(), glm::vec3(0.3, 0.7, 1));
		main_window.setModelMatrix("pyramid2", model);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		main_window.use();


		gui.showMainWindowGUI();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	//Cleanup all buffers and shaders
	main_window.cleanup();
	return 0;
}

void mainWindowMouseCallback(GLFWwindow *window, int button, int action, int mods)
{
	ImGuiIO &io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		main_window.selectionBufferPass();
		auto result = main_window.faceDetection();
		if (result)
		{
			auto [clicked_ID, mesh_id, empty] = *result;


		switch (gui.currentState)
		{
		case FACE_EDITING: 
			std::cout << "Clicked on face ID: " << clicked_ID << " of mesh ID: " << mesh_id << std::endl;
			main_window.updateFacesSelected(clicked_ID, mesh_id);
			break;

		case VERTEX_EDITING:
			std::cout << "Clicked on vertex ID: " << clicked_ID << " of mesh ID: " << mesh_id << std::endl;
			main_window.updateVerticesSelected(clicked_ID, mesh_id);
			break;
		case EDGE_EDITING:
			std::cout << "Clicked on edge ID: " << clicked_ID << " of mesh ID: " << mesh_id << std::endl;
			main_window.updateEdgesSelected(clicked_ID, mesh_id);
			break;
		}
		}
		else
			std::cout << "Clicked on empty space" << std::endl;
	}
}