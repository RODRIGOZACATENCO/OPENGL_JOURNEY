#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<ShaderHandler.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwFocusWindow(window);
	std::string vertexShaderPath = std::string(ROOT_DIR) + "Shaders/shader.vs";
	std::string fragmentShaderPath = std::string(ROOT_DIR) + "Shaders/shader.fs";
	Shader shader = Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	float vertices[] = {
		// positions // colors // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f // top left
	};
	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};
	unsigned int VBO, VAO, EBO;//vertex buffer, vertex array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);//element buffer, sotres the order to draw shapes
	glGenBuffers(1, &VBO);//vertex vuffer

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	//rendering vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*)0);//positions
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*)(3*sizeof(float)));//color
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
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



	while (!glfwWindowShouldClose(window)) {
		//model matrix: transforms coordinates from local space into world space
		//like putting objects on a table
		glm::mat4 model=glm::mat4(1.0f);
		model=glm::rotate(model,glm::radians(-45.0f),glm::vec3(1.0f,0.0f,0.0f));
		//inclines the whole plane 45 degrees
		//view matrix:moves the scene to where the camera is supposed to be
		//it has to be in the ooposite "direction" the camera is moving
		//camera forward=scene backwards
		glm::mat4 view=glm::mat4(1.0f);
		//opengl uses right-handed set of coordinates
		view=glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));

		//using perspective projection
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(w/h), 0.1f, 100.0f);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		unsigned int model_location=glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
		unsigned int view_location=glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
		unsigned int projection_location=glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

