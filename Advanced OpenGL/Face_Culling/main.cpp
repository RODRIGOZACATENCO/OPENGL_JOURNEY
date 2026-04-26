#include "model_loader.h"
#include "TextureHandler.h"
#include <map>;
glm::vec3 cameraPos= glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp= glm::vec3(0.0f, 1.0f, 0.0f);
float lastX,lastY,yaw,pitch;
bool firstMouse=true;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
int MARBLE_TEXTURE_ID=0;
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
	lastX=w/2.0;
	lastY=h/2.0;
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwFocusWindow(window);
	glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, w / 2.0, h / 2.0);
	glfwSetCursorPosCallback(window,mouse_callback);


	std::string vertex_path= std::string(ROOT_DIR)+ "shaders/shader.vert";
	std::string fragment_path= std::string(ROOT_DIR)+ "shaders/shader.frag";
	std::string screen_vertex_path= std::string(ROOT_DIR)+ "shaders/screenShader.vert";
	std::string screen_fragment_path= std::string(ROOT_DIR)+ "shaders/screenShader.frag";
	Shader shader(vertex_path.c_str(),fragment_path.c_str());
	Shader screenShader(screen_vertex_path.c_str(),screen_fragment_path.c_str());
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	std::string texture_path= std::string(ROOT_DIR)+"textures/container.jpg";
	Texture2d marble_texture= Texture2d(texture_path,MARBLE_TEXTURE_ID);
	float quadVertices[] = {
		// positions
		// texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f,
		 1.0f, 0.0f,
		1.0f, 1.0f,
		 1.0f, 1.0f
		};


	float mirrorVertices[]={
		-0.50f,0.75f,0.0f,0.0f,//bottom-left
		 0.50f,0.75f,1.0f,0.0f,//bottom-right
		-0.50f,1.0f,0.0f,1.0f,//top-left
		 0.50f,1.0f,1.0f,1.0f,//top-right
		 0.50f,0.75f,1.0f,0.0f,//bottom-left
		-0.50f,1.0f,0.0f,1.0f,//top-left

	};
	float vertices[] = {
		// back face
		-0.5f, -0.5f, -0.5f,
		0.0f, 0.0f, // bottom-left
		0.5f, 0.5f, -0.5f,
		 1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f,
		 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f,
		 1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,
		 0.0f, 0.0f, // bottom-left
		-0.5f, 0.5f, -0.5f,
		 0.0f, 1.0f, // top-left
		// front face
		-0.5f, -0.5f, 0.5f,
		 0.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f,
		 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, 0.5f,
		 1.0f, 1.0f, // top-right
		0.5f, 0.5f, 0.5f,
		 1.0f, 1.0f, // top-right
		-0.5f, 0.5f, 0.5f,
		 0.0f, 1.0f, // top-left
		-0.5f, -0.5f, 0.5f,
 0.0f, 0.0f, // bottom-left
// left face
-0.5f, 0.5f, 0.5f,
 1.0f, 0.0f, // top-right
-0.5f, 0.5f, -0.5f,
 1.0f, 1.0f, // top-left
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f, // bottom-left
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f, // bottom-left
-0.5f, -0.5f, 0.5f,
 0.0f, 0.0f, // bottom-right
-0.5f, 0.5f, 0.5f,
 1.0f, 0.0f, // top-right
// right face
0.5f, 0.5f, 0.5f,
 1.0f, 0.0f, // top-left
0.5f, -0.5f, -0.5f,
 0.0f, 1.0f, // bottom-right
0.5f, 0.5f, -0.5f,
 1.0f, 1.0f, // top-right
0.5f, -0.5f, -0.5f,
 0.0f, 1.0f, // bottom-right
0.5f, 0.5f, 0.5f,
 1.0f, 0.0f, // top-left
0.5f, -0.5f, 0.5f,
 0.0f, 0.0f, // bottom-left
// bottom face
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f, // top-right
0.5f, -0.5f, -0.5f,
 1.0f, 1.0f, // top-left
0.5f, -0.5f, 0.5f,
 1.0f, 0.0f, // bottom-left
0.5f, -0.5f, 0.5f,
 1.0f, 0.0f, // bottom-left
-0.5f, -0.5f, 0.5f,
 0.0f, 0.0f, // bottom-right
-0.5f, -0.5f, -0.5f,
 0.0f, 1.0f, // top-right
// top face
-0.5f, 0.5f, -0.5f,
 0.0f, 1.0f, // top-left
0.5f, 0.5f, 0.5f,
 1.0f, 0.0f, // bottom-right
0.5f, 0.5f, -0.5f,
 1.0f, 1.0f, // top-right
0.5f, 0.5f, 0.5f,
 1.0f, 0.0f, // bottom-right
-0.5f, 0.5f, -0.5f,
 0.0f, 1.0f, // top-left
-0.5f, 0.5f, 0.5f,
 0.0f, 0.0f // bottom-left
};
	unsigned int VAO, VBO;
	unsigned int quadVAO,quadVBO;
	unsigned int mirrorVao,mirrorVBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1,&quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4* sizeof(float), (void*)0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glGenVertexArrays(1, &mirrorVao);
	glGenBuffers(1,&mirrorVBO);
	glBindVertexArray(mirrorVao);
	glBindBuffer(GL_ARRAY_BUFFER,mirrorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mirrorVertices), mirrorVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4* sizeof(float), (void*)0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	unsigned int framebuffer;
	glGenFramebuffers(1,&framebuffer);
	//to use a frame buffer you need
	//1.attach one buffer at least
	//2.one color attachment
	//3.attachments should be complete
	//4.each buffer should contain the same number of samples
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	//Texture attachments
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB,
	GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	//type of media in wich you can optimize memory, are read-only
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER, rbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" <<
		std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (glfwWindowShouldClose(window) == 0) {
		processInput(window);
		//render the mirror scene
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		shader.use();
		glm::mat4 model=glm::mat4(1.0f);
		model=glm::translate(model,glm::vec3(0.0f,0.0f,-0.3f));
		model=glm::rotate(model,glm::radians(45.0f),glm::vec3(0.0f,1.0f,0.0f));
		glm::mat4 view=glm::lookAt(cameraPos,cameraPos-cameraFront,cameraUp);//rotated camera
		glm::mat4 projection = glm::perspective(glm::radians(45.0F), (float)800 / (float)600, 0.1f, 100.0f);
		glBindVertexArray(VAO);
		shader.setMat4("model",model);
		shader.setMat4("view",view);
		shader.setMat4("projection",projection);
		shader.setInt("texture1",MARBLE_TEXTURE_ID);
		marble_texture.use();
		glDrawArrays(GL_TRIANGLES,0,36);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		view=glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);

		glBindVertexArray(VAO);
		shader.setMat4("view",view);
		glDrawArrays(GL_TRIANGLES,0,36);

		screenShader.use();
		glBindVertexArray(mirrorVao);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1,&VAO);
	glDeleteFramebuffers(1,&framebuffer);
	glfwTerminate();
	return 0;


}

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