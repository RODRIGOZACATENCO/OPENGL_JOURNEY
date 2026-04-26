#include "model_loader.h"
float deltaTime = 0.0f;
float lastFrame = 0.0f;
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
    lastX=w/2.0;
    lastY=h/2.0;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwFocusWindow(window);
    glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, w / 2.0, h / 2.0);
    glfwSetCursorPosCallback(window,mouse_callback);

    stbi_set_flip_vertically_on_load(true);
	std::cout<<"Models Loading"<<std::endl;
    Shader shader("/home/rodrigo/CLionProjects/Models/Model_assimp/Shaders/shader.vert",
        "/home/rodrigo/CLionProjects/Models/Model_assimp/Shaders/shader.frag");
    Shader backpack_shader("/home/rodrigo/CLionProjects/Models/Model_assimp/Shaders/backpack.vert",
        "/home/rodrigo/CLionProjects/Models/Model_assimp/Shaders/backpack.frag");
    Model object_model("/home/rodrigo/CLionProjects/Models/Model_assimp/models/cyborg/cyborg.obj");
    Model backpack_model("/home/rodrigo/CLionProjects/Models/Model_assimp/models/backpack/backpack.obj");
	std::cout<<"Models Loaded!"<<std::endl;

	glEnable(GL_DEPTH_TEST);
    while (glfwWindowShouldClose(window) == 0) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(w)/(float)(h), 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        object_model.Draw(shader);

        backpack_shader.use();
        backpack_shader.setMat4("projection", projection);
        backpack_shader.setMat4("view", view);
        model= glm::mat4(1.0f);
        model=glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.7f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
        backpack_shader.setMat4("model", model);
        backpack_model.Draw(shader);

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;

}
