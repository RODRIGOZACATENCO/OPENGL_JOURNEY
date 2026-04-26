#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<ShaderHandler.h>
glm::vec3 cameraPos= glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp= glm::vec3(0.0f, 1.0f, 0.0f);
float lastX,lastY,yaw,pitch;
bool firstMouse=true;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
		cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
		cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	int w,h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	lastX=w/2;
	lastY=h/2;
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwFocusWindow(window);
	glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window,mouse_callback);



	std::string vertexShaderPath = std::string(ROOT_DIR) + "Shaders/shader.vs";
	std::string fragmentShaderPath = std::string(ROOT_DIR) + "Shaders/shader.fs";
	Shader shader = Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	float vertices[] = {
	   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,
 0.0f, 0.0f,
-0.5f, 0.5f, 0.5f,
 1.0f, 0.0f,
0.5f, 0.5f, 0.5f,
 1.0f, 0.0f,
0.5f, 0.5f, -0.5f,
 1.0f, 1.0f,
0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
0.5f, -0.5f, 0.5f,
 0.0f, 0.0f,
0.5f, 0.5f, 0.5f,
 1.0f, 0.0f,
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
0.5f, -0.5f, -0.5f,
 1.0f, 1.0f,
0.5f, -0.5f, 0.5f,
 1.0f, 0.0f,
0.5f, -0.5f, 0.5f,
 1.0f, 0.0f,
-0.5f, -0.5f, 0.5f,
 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f,
-0.5f,
 0.5f, -0.5f,
 0.0f, 1.0f,
0.5f,
 0.5f, -0.5f,
 1.0f, 1.0f,
0.5f,
 0.5f, 0.5f,
 1.0f, 0.0f,
0.5f,
 0.5f, 0.5f,
 1.0f, 0.0f,
-0.5f,
 0.5f, 0.5f,
 0.0f, 0.0f,
-0.5f,
 0.5f, -0.5f,
 0.0f, 1.0f
};//CUBE shape, 3 vertices, 2 indices for texture
	glm::vec3 cubePositions[] = {//positon of cubes in space
		glm::vec3( 0.0f, 0.0f,
		 0.0f),
		glm::vec3( 2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f, 2.0f, -2.5f),
		glm::vec3( 1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
		};

	unsigned int VBO, VAO, EBO;//vertex buffer, vertex array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);//element buffer, sortesx the order to draw shapes
	glGenBuffers(1, &VBO);//vertex buffer

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	//rendering vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)0);//positions
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);//shader coordinates
	glBindVertexArray(0);

	int width, height, nrChannels;
	std::string texturePath = std::string(ROOT_DIR) + "textures/container.jpg";
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	unsigned int texture1,texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	texturePath = std::string(ROOT_DIR) + "textures/awesomeface.png";
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(texturePath.c_str(), &width, &height,&nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout<<"Texture Not Loaded"<<std::endl;
	}
	stbi_image_free(data);
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	shader.setInt("texture2", 1);
	glEnable(GL_DEPTH_TEST);//checks depth when rendering
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glm::mat4 model=glm::mat4(1.0f),view=glm::mat4(1.0f),projection=glm::mat4(1.0f);
		model=glm::translate(model,glm::vec3(0.0f, 0.0f, 1.0f));
		model=glm::rotate(model,(float)glfwGetTime()*glm::radians(45.0f),glm::vec3(0.2f, 0.4f, 0.8f));
		view=glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)(w/h), 0.1f, 100.0f);
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		shader.setMat4("model", model);
		shader.setMat4("view",view);
		shader.setMat4("projection",projection);
		glBindVertexArray(VAO);
		shader.use();
		for (int i=0;i<10;i++) {
			model=glm::mat4(1.0f);
			model=translate(model,cubePositions[i]);
			float angle = glm::radians(45.0f)*static_cast<float>(i+1);
			model=rotate(model, (float)glfwGetTime()*angle, glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		glBindVertexArray(0);
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;

	}