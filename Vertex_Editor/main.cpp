#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Shared headers from your global directory
#include <random>

#include "MouseHandling.h"
#include "ShaderHandler.h"
#include "TextureHandler.h"
#include "../include/CameraHandler.h"
#include "../include/Mesh.h"


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


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		use_main_buffer = !use_main_buffer;
	}
	if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

	}

}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}



void mainMeshWindow(GLFWwindow *window,Shader *shader,Mesh *mesh) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect=(float)width/(float)height;
	glm::mat4 model=glm::mat4(1.0f);
	model=glm::translate(model,glm::vec3(0.0f,0.0f,-1.5f));
	model=glm::rotate(model,glm::radians(35.0f),glm::vec3(0.4f,1.0f,0.0f));
	model=glm::scale(model,glm::vec3(0.7f,0.7f,0.7f));
	glm::mat4 view=glm::lookAt(glm::vec3(0.0f,0.0f,2.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 projection=glm::perspective(glm::radians(45.0f),aspect,0.1f,100.0f);
	if (use_main_buffer) {
		shader->use();
		shader->setMat4("model",model);
		shader->setMat4("view",view);
		shader->setMat4("projection",projection);

		glm::vec3 color=glm::vec3(0.0f,0.0f,1.0f);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		shader->setVec3("aColor",color);
		mesh->render_mesh();
	}
}

//i need to consolidate the windows in somethin, to make it easier to work with
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

	int frameBufferWidth = 800;
	int frameBufferHeight = 600;
	glfwGetFramebufferSize(window,&frameBufferWidth,&frameBufferHeight);
	glViewport(0,0,frameBufferWidth,frameBufferHeight);
	// Render Loop
	std::string vertex_shader_path="../shaders/shader.vert";
	std::string fragment_shader_path="../shaders/shader.frag";
	Shader shader(vertex_shader_path.c_str(),fragment_shader_path.c_str());
	std::string fragment_colorPickingShader_path="../shaders/colorPickingShader.frag";
	Shader color_picking_shader(vertex_shader_path.c_str(),fragment_colorPickingShader_path.c_str());
	std::vector<glm::vec3> colors;

	Mesh mesh(&triangle_vertices,&faces,&color_picking_shader);
	mesh.renderSetup();


	int width,height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, key_callback);



    while (!glfwWindowShouldClose(window)) {


    	processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    glfwTerminate();
    return 0;
}