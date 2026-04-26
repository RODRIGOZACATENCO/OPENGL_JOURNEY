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

int main()
{
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glm::mat4 translations=glm::mat4(1.0f);
		translations=glm::translate(translations,glm::vec3(0.5f, -0.5f, 0.0f));//rotates around the z axis
		translations=glm::rotate(translations,(float)glfwGetTime(),glm::vec3(0.0f, 0.0f, 1.0f));
		unsigned int translation_location=glGetUniformLocation(shader.ID, "transform");
		glUniformMatrix4fv(translation_location, 1, GL_FALSE, glm::value_ptr(translations));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		translations=glm::mat4(1.0f);
		translations=glm::translate(translations,glm::vec3(-0.5f, 0.5f, 0.0f));//rotates around the z axis
		float scaling_factor=static_cast<float>(sin(glfwGetTime()));
		translations=glm::scale(translations,glm::vec3(scaling_factor, scaling_factor, scaling_factor));
		glUniformMatrix4fv(translation_location, 1, GL_FALSE, glm::value_ptr(translations));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
}
