#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Shared headers from your global directory
#include "ShaderHandler.h"
#include "TextureHandler.h"
#include "../include/CameraHandler.h"
#include "../include/HalfEdge.h"


std::vector<float> triangle_vertices{//2 rectangles, one on top of another
	-0.5f, 0.5f, 0.0f,//top left 0
	-0.5f, 0.0f, 0.0f,//middle left 1
	-0.5f,-0.5f, 0.0f,//bottom left 2
	 0.5f, 0.5f, 0.0f,//top right 3
	 0.5f, 0.0f, 0.0f,//middle right 4
	 0.5f,-0.5f, 0.0f,//bottom right 5
};

std::vector<int> faces={
	0,1,4,
	4,3,0,
	1,2,5,
	5,4,1
};



//triangle needs; 3 vertices , 3 edges, 1 face 3 half edges, and 3 twins, but they will be empty


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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

	int frameBufferWidth = 800;
	int frameBufferHeight = 600;
	glfwGetFramebufferSize(window,&frameBufferWidth,&frameBufferHeight);
	Mesh mesh(&triangle_vertices,&faces);
	mesh.renderSetup();
	mesh.show_mesh_structure();
	glViewport(0,0,frameBufferWidth,frameBufferHeight);
    // Render Loop
	std::string vertex_shader_path="/home/rodrigo/CLionProjects/Vertex_Editor/shaders/shader.vert";
	std::string fragment_shader_path="/home/rodrigo/CLionProjects/Vertex_Editor/shaders/shader.frag";
	Shader shader(vertex_shader_path.c_str(),fragment_shader_path.c_str());
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
    	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    	shader.use();
    	mesh.render_mesh();
    	processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}