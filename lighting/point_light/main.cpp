#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<ShaderHandler.h>
#include<TextureHandler.h>
glm::vec3 cameraPos= glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp= glm::vec3(0.0f, 1.0f, 0.0f);
float lastX,lastY,yaw,pitch;
bool firstMouse=true;
constexpr int BOX_TEXTURE_ID=0;
constexpr int SMILEY_TEXTURE_ID=1;
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
		lastX = 400;
		lastY = 300;
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



	std::string vertexShaderPath = std::string(ROOT_DIR) + "Shaders/shader.vert";
	std::string fragmentShaderPath = std::string(ROOT_DIR) + "Shaders/shader.frag";
	std::string lightSourceVertexShaderPath = std::string(ROOT_DIR) + "Shaders/lightSourceShader.vert";
	std::string lightSourceFragmentShaderPath = std::string(ROOT_DIR) + "Shaders/lightSourceShader.frag";
	Shader shader = Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	Shader lightSourceShader =Shader(lightSourceVertexShaderPath.c_str(),lightSourceFragmentShaderPath.c_str());

	shader.use();
	Texture2d box_texture=Texture2d(std::string(ROOT_DIR)+"textures/container.png",BOX_TEXTURE_ID);
	box_texture.setWrapMode(GL_REPEAT,GL_REPEAT);
	box_texture.use();
	shader.setInt("material.diffuse1", BOX_TEXTURE_ID);

	Texture2d smiley_texture=Texture2d(std::string(ROOT_DIR)+"textures/awesomeface.png",SMILEY_TEXTURE_ID);
	smiley_texture.setWrapMode(GL_REPEAT,GL_REPEAT);
	smiley_texture.use();
	shader.setInt("material.diffuse2", SMILEY_TEXTURE_ID);


	float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
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
	//for the cube only
	unsigned int VBO, VAO;//vertex buffer, vertex array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);//vertex buffer

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	//rendering vertex buffer object
	//rendering vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*)0);//positions
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	unsigned int lightVAO;
	glGenVertexArrays(1,&lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float),
		(void*)0);//positions
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		shader.use();
		box_texture.use();
		// view-projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(w/h), 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setVec3("viewPos", cameraPos);

		shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		shader.setFloat("material.shininess", 32.0f);

		glm::vec3 lightColor(1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f);

		shader.setVec3("light.ambient",ambientColor);
		shader.setVec3("light.diffuse",diffuseColor);
		shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		shader.setFloat("light.constant", 1.0f);
		shader.setFloat("light.linear", 0.09f);
		shader.setFloat("light.quadratic", 0.032f);

		//glm::vec3  lightDir(-0.2f, -1.0f, -0.3f);
		//shader.setVec3("light.direction",lightDir);
		float radius=3;
		glm::vec3 lightPos(radius*glm::sin(glfwGetTime()),1.0f,radius*glm::cos(glfwGetTime()));


		shader.setVec3("light.position",lightPos);
		glm::mat4 model = glm::mat4(1.0f);
		for(unsigned int i = 0; i < 10; i++)
		{
			//renders cube
			shader.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = glm::radians(35.0f) * (i+1);
			model = glm::rotate(model, angle,
			glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);


		}
		//render light source
		lightSourceShader.use();
		model=glm::mat4(1.0f);

		model=glm::translate(model,glm::vec3(0.0f,0.0f,-0.3f));
		model=glm::translate(model, lightPos);
		model=glm::scale(model, glm::vec3(0.3f,0.3f,0.3f));
		shader.setMat4("model", model);
		lightSourceShader.setMat4("model", model);
		lightSourceShader.setMat4("view", view);
		lightSourceShader.setMat4("projection",projection);
		lightSourceShader.setVec3("viewPos", cameraPos);
		lightSourceShader.setVec3("bulbColor", lightColor);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();
	return 0;

	}