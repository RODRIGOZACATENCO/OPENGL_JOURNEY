//
// Created by rodrigo on 04/05/2026.
//

#ifndef VERTEXEDITOR_RENDERER_H
#define VERTEXEDITOR_RENDERER_H
#include <GLFW/glfw3.h>
#include "ShaderHandler.h"
#include "GUI.h"
#include "Scene.h"
struct FramebufferInfo {
	unsigned int FBO;
	unsigned int texture;
	unsigned int DBO;
};
/*Renderer Class makes the calls to the graphic API
 *it holds all the shaders
 **/
class Renderer {
	private:
	Scene *current_scene;//scene currently being drawn

	GUIState render_mode;//FACE, VERTEX OR EDGE EDITING

	public:
	Renderer(GLFWwindow *window,Scene *start_scene) {

		this->window = window;
		current_scene = start_scene;
		FramebufferSetup();
		shaderSetup();
	}
	GLFWwindow *window;
	int width, height;
	Shader *main_shader,*color_picking_shader;//shaders for the main render pass and the color picking pass
	Shader *vertex_pass_shader, *edge_pass_shader;//shader for the vertex selection pass
	Shader *main_pass_edge_shader;

	FramebufferInfo color_picking_framebuffer_info;//framebuffer for color picking pass


	void selectionBufferPass();
	void mainRenderPass();

	void FramebufferSetup();
	void shaderSetup();

	void resizeFramebuffer();

	void setScreenSize(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}

	void setCurrentScene(Scene *scene) {
		this->current_scene = scene;
	}
	Scene* getCurrentScene() {
		return current_scene;
	}
	void setRenderMode(GUIState state) {
		this->render_mode = state;
	}
	GUIState getRenderMode(){
		return render_mode;
	}

	void setViewProjectionMatrices();
	void cleanup();
	bool rendererIsReady(std::string* out_error = nullptr)const ;
	std::optional<std::tuple<unsigned int, unsigned int, unsigned int>> meshElementDetection();
	std::pair<int,int> getCursorPositionInViewport(GLFWwindow *window);
};


#endif //VERTEXEDITOR_RENDERER_H