//
// Created by rodrigo on 30/04/2026.
//

#include <glad/glad.h>
#include <filesystem>

#include "../include/MainWindow.h"

#include <GLFW/glfw3.h>
#include "MainWindow.h"
#include <random>
#include <imgui.h>


glm::vec3 generateRandomColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	return {dis(gen) / 255.0f, dis(gen) / 255.0f, dis(gen) / 255.0f};
}


void MainWindow::use()
{
	if (SetupRun)
	{
		switch (gui->currentState)
		{
		case FACE_EDITING:
			if (update_faces_selected){

				glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
				glBufferSubData(GL_SHADER_STORAGE_BUFFER,
								0,	   // byte offset where to start
								face_selection_array.size() * sizeof(int), // size in bytes
								face_selection_array.data());			   // new data
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
				update_faces_selected=false;
			}

			colorPickingPass();
			break;

		default:
			break;
		}
		mainRenderPass();
	}
	else
	{
		FramebufferSetup();
		meshRenderSetup();
		SetupRun = true;
	}
}

void MainWindow::updateFacesSelected(unsigned int face_id,unsigned int mesh_id) {
	unsigned int global_face_id=0;
	for(int i=0;i<mesh_id;i++) {
		global_face_id+=meshes[i]->faces.size();
	}
	// change the state of the face in the global array
	face_selection_array[global_face_id+face_id]=!face_selection_array[global_face_id+face_id];
	update_faces_selected = true;
}
std::tuple<unsigned int, unsigned int, unsigned int> MainWindow::faceDetection(){

	auto [framebuffer_x, framebuffer_y] = getCursorPositionInViewport(window);
	glBindFramebuffer(GL_FRAMEBUFFER, color_picking_framebuffer_info.FBO);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	unsigned int pixel_data[3];
	glReadPixels(framebuffer_x, framebuffer_y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel_data);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return {pixel_data[0]-1, pixel_data[1], pixel_data[2]};
}


std::pair<int,int> MainWindow::getCursorPositionInViewport(GLFWwindow *window) {
	
	// GLFW cursor: 0,0 is top left corner, width,height is bottom right corner
	// glReadPixels: 0,0 is bottom left corner, width,height is top right corner
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	glfwGetWindowSize(window, &width, &height);
	int framebuffer_width, framebuffer_height;
	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
	// convert mouse coordinates to framebuffer coordinates
	int framebuffer_x = mouse_x * (framebuffer_width / (double)width);
	int framebuffer_y = (framebuffer_height - mouse_y) * (framebuffer_height / (double)height);
	return {framebuffer_x, framebuffer_y};
	}

void MainWindow::mainRenderPass()
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shader->use();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	int current_face_offset=0;
	for (auto mesh : meshes)
	{

		shader->setMat4("model", mesh->render_info.model);
		shader->setInt("num_faces_offset", current_face_offset);
		current_face_offset += mesh->faces.size();

		glBindVertexArray(mesh->render_info.VAO);
		glDrawElements(GL_TRIANGLES, mesh->render_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
/*Setup all the meshes RenderInfo to show on the screen */
void MainWindow::meshRenderSetup()

{
	for (auto mesh : meshes)
	{
		RenderInfo *ri = &mesh->render_info;
		glGenVertexArrays(1, &ri->VAO);
		glGenBuffers(1, &ri->VBO);
		glGenBuffers(1, &ri->EBO);
		
		glBindBuffer(GL_ARRAY_BUFFER, ri->VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->render_indices.size() * sizeof(unsigned int), mesh->render_indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(ri->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, ri->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, point));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		
		addFacesToSelectionArray(mesh);
	}
	std::filesystem::path shadersRoot = std::filesystem::path(ROOT_DIR) / "shaders";
	std::string vertex_shader_path = (shadersRoot / "shader.vert").string();
	std::string geometry_shader_path = (shadersRoot / "shader.geom").string();
	std::string fragment_shader_path = (shadersRoot / "shader.frag").string();
	std::string fragment_colorPickingShader_path = (shadersRoot / "colorPickingShader.frag").string();
	shader = new Shader(vertex_shader_path.c_str(), fragment_shader_path.c_str(), geometry_shader_path.c_str());
	color_picking_shader = new Shader(vertex_shader_path.c_str(), fragment_colorPickingShader_path.c_str());
	shader->use();
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, face_selection_array.size() * sizeof(int),
				 face_selection_array.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	color_picking_shader->use();
	color_picking_shader->setMat4("view", view);
	color_picking_shader->setMat4("projection", projection);
	
}

void MainWindow::addFacesToSelectionArray(Mesh* mesh) {
	for(int i=0;i<mesh->faces.size();i++) {
		face_selection_array.push_back(0);
	}
	
}

void MainWindow::colorPickingPass()
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, color_picking_framebuffer_info.FBO);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	color_picking_shader->use();
	unsigned int mesh_id = 0;
	for (auto mesh : meshes)
	{;
		color_picking_shader->setMat4("model", mesh->render_info.model);
		color_picking_shader->setUint("MeshID", mesh_id++);
		glBindVertexArray(mesh->render_info.VAO);
		glDrawElements(GL_TRIANGLES, mesh->render_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
void MainWindow::FramebufferSetup()
{
	glfwGetFramebufferSize(window, &width, &height);
	// it will create the frame buffer
	glGenFramebuffers(1, &color_picking_framebuffer_info.FBO);
	glGenTextures(1, &color_picking_framebuffer_info.texture);
	glGenRenderbuffers(1, &color_picking_framebuffer_info.DBO);
	glBindFramebuffer(GL_FRAMEBUFFER, color_picking_framebuffer_info.FBO);
	glBindTexture(GL_TEXTURE_2D, color_picking_framebuffer_info.texture);
	// it's a texture that will cover the entire viewport
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// binds the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_picking_framebuffer_info.texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, color_picking_framebuffer_info.DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, color_picking_framebuffer_info.DBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
}

void MainWindow::resizeFramebuffer(int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, color_picking_framebuffer_info.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind when done

	// 2. Bind and resize the depth renderbuffer attachment
	glBindRenderbuffer(GL_RENDERBUFFER, color_picking_framebuffer_info.DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind when done
}

void MainWindow::cleanup()
{
	glDeleteFramebuffers(1, &color_picking_framebuffer_info.FBO);
	glDeleteTextures(1, &color_picking_framebuffer_info.texture);
	glDeleteRenderbuffers(1, &color_picking_framebuffer_info.DBO);
	glDeleteBuffers(1, &SSBO);
	for (auto mesh : meshes)
	{
		RenderInfo *ri =&mesh->render_info;
		glDeleteVertexArrays(1, &ri->VAO);
		glDeleteBuffers(1, &ri->VBO);
		glDeleteBuffers(1, &ri->EBO);

	}
	delete shader;
	delete color_picking_shader;
}