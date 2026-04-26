#include "model_loader.h"
#include "TextureHandler.h"
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
int MARBLE_TEXTURE_ID=0,METAL_TEXTURE_ID=1,GRASS_TEXTURE_ID=2;
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

	glEnable(GL_DEPTH_TEST);
	std::string vertex_path= std::string(ROOT_DIR)+ "shaders/shader.vert";
	std::string fragment_path= std::string(ROOT_DIR)+ "shaders/shader.frag";
	std::string outlinefrag_path= std::string(ROOT_DIR)+ "shaders/outline.frag";
	Shader shader(vertex_path.c_str(),fragment_path.c_str());
	Shader outline_shader(vertex_path.c_str(),outlinefrag_path.c_str());
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cubeVertices[] = {
	 // positions          // texture Coords
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	float planeVertices[] = {
	 // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	  5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	 -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	 -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	  5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	 -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	  5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	float grassVertices[]={
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,


	};
	unsigned int grassIndices[]={
		0,1,2,
		2,3,1
	};
	std::vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3( 1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3( 0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3( 0.5f, 0.0f, -0.6f));

	unsigned int planeVAO,cubeVAO;
	unsigned int planeVBO,cubeVBO;
	unsigned int grassVAO,grassVBO,grassEBO;

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	//vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glGenVertexArrays(1,&cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glGenVertexArrays(1,&grassVAO);
	glGenBuffers(1,&grassVBO);
	glGenBuffers(1,&grassEBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grassEBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(grassVertices),grassVertices,GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(grassIndices),grassIndices,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	std::string marble_path = std::string(ROOT_DIR)+ "textures/marble.jpg";
	std::string metal_path = std::string(ROOT_DIR)+ "textures/metal.png";
	std::string grass_path = std::string(ROOT_DIR)+ "textures/grass.png";
	stbi_set_flip_vertically_on_load(true);
	Texture2d marble=Texture2d(marble_path.c_str(),MARBLE_TEXTURE_ID);
	Texture2d metal= Texture2d(metal_path.c_str(),METAL_TEXTURE_ID);
	Texture2d grass=Texture2d(grass_path.c_str(),GRASS_TEXTURE_ID,true);
	marble.setWrapMode(GL_REPEAT,GL_REPEAT);
	metal.setWrapMode(GL_REPEAT,GL_REPEAT);
	grass.setWrapMode(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);

	//start the stencil test
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	while (glfwWindowShouldClose(window) == 0) {
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);


		//if the stencil test fails, if stencil test passes and the depth test fails, if both succeed
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//set the stencil buffer to one wherever we draw in the screen
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilMask(0x00);// do not stencil the floor

		// floor
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(45.0F), (float)800 / (float)600, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		glBindVertexArray(planeVAO);
		shader.setInt("texture1",METAL_TEXTURE_ID);
		metal.use();
		model=glm::mat4(1.0f);
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// cubes
		glStencilMask(0xff);

		glBindVertexArray(cubeVAO);
		shader.setInt("texture1",MARBLE_TEXTURE_ID);
		marble.use();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//draw wherever the stencil is not equal to 1
		glStencilMask(0x00); // disable writing to the stencil buffer
		glDisable(GL_DEPTH_TEST);//disable depth test so the outline doesnt clip on the floor
		outline_shader.use();
		outline_shader.setMat4("view", view);
		outline_shader.setMat4("projection", projection);
		marble.use();
		model=glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		outline_shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		outline_shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glStencilMask(0xff);
		glStencilFunc(GL_ALWAYS,1,0xff);
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(grassVAO);
		shader.use();
		shader.setInt("texture1",GRASS_TEXTURE_ID);
		grass.use();
		for (int i=0;i<vegetation.size();i++) {
			model=glm::mat4(1.0f);
			model = glm::translate(model, vegetation[i]);
			shader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &grassVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1,&grassVBO);
	glDeleteBuffers(1,&grassEBO);


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