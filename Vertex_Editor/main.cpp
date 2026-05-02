#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Shared headers from your global directory
#include <random>

#include "MainWindow.h"

#include "ShaderHandler.h"
#include "TextureHandler.h"

#include "../include/Mesh.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
std::vector<float> triangle_vertices{
	-1,-1,-1,//bottom left back 0
	-1,-1, 1,//bottom left front 1
	 1,-1,-1,//bottom right back 2
	 1,-1, 1,//bottom right front 3
	-1, 1,-1,//top left back 4
	-1, 1, 1,//top left front 5
	 1, 1,-1,//top right back 6
	 1, 1, 1,//top right front 7

};
/*
std::vector<int> faces={
	// Bottom Face
	0, 2, 1,
	1, 2, 3,

	// Back Face
	0, 4, 2,
	2, 4, 6,

	// Right Face
	2, 6, 3,
	3, 6, 7,

	// Front Face
	1, 3, 5,
	3, 7, 5,

	// Top Face
	5, 7, 4,
	6, 4, 7,

	// Left Face
	0, 1, 4,
	5, 4, 1
};
*/
std::vector<int> faces={
	// Bottom Face
	0, 2, 1,
	1, 2, 3,

	// Back Face
	0, 4, 2,
	2, 4, 6,

	// Right Face
	2, 6, 3,
	3, 6, 7,

	// Front Face
	1, 3, 5,
	3, 7, 5,

	// Top Face
	5, 7, 4,
	6, 4, 7,

	// Left Face
	0, 1, 4,
	5, 4, 1
};
bool use_main_buffer=true;
Mesh mesh(&triangle_vertices,&faces);
MainWindow main_window(nullptr);
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		use_main_buffer = !use_main_buffer;
	}
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
void mouseCallback(GLFWwindow* window,int button,int action,int mods) {
	if (button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS) {
		if(main_window.mouseDetection()){
			std::cout<<"Face "<<main_window.mouseDetection()<<" clicked!"<<std::endl;
			mesh.updateFaceSelection(main_window.mouseDetection()-1);
			main_window.update_faces_selected = true;//sets a flag on the main window to update the selected faces on the shader
		}
		else {
			std::cout<<"No face clicked!"<<std::endl;
		}
	}

}


int main() {
	// GLFW Init
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "VertexEditor", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// GLAD Init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");

	int frameBufferWidth = 800;
	int frameBufferHeight = 600;
	glfwGetFramebufferSize(window,&frameBufferWidth,&frameBufferHeight);
	glViewport(0,0,frameBufferWidth,frameBufferHeight);
	
	std::vector<glm::vec3> colors;
	main_window.setWindow(window);

	glm::mat4 view=glm::lookAt(glm::vec3(0,0,3),glm::vec3(0,0,0),glm::vec3(0,1,0));
	glm::mat4 projection=glm::perspective(glm::radians(45.0f),(float)frameBufferWidth/(float)frameBufferHeight,0.1f,100.0f);
	main_window.addMesh(&mesh,"cube",glm::mat4(1.0f));

	main_window.setViewMatrix(view);
	main_window.setProjectionMatrix(projection);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouseCallback);

    while (!glfwWindowShouldClose(window)) {

		glm::mat4 model=glm::mat4(1.0f);
		model=glm::translate(model,glm::vec3(0,0,-2));
		model=glm::scale(model,glm::vec3(0.7f));
		//model=glm::rotate(model,(float)glfwGetTime(),glm::vec3(1,1,0));
		model=glm::rotate(model,(float)glfwGetTime(),glm::vec3(0.3,0.7,0));
		main_window.setModelMatrix("cube",model);
		main_window.use();

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
    }

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}