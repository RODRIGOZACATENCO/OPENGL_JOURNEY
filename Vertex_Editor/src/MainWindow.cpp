//
// Created by rodrigo on 30/04/2026.
//

#include <glad/glad.h>
#include <filesystem>

#include "../include/MainWindow.h"

#include <GLFW/glfw3.h>

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
{	if(gui->reset_selection_buffer_flag) {
		resetSelectionBuffer(gui->reset_selection_buffer_flag);
		gui->reset_selection_buffer_flag=0;
	}
	switch(update_selected_buffer){
		case FACE_EDITING:
			
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER,
							0,										   // byte offset where to start
							face_selection_array.size() * sizeof(int), // size in bytes
							face_selection_array.data());			   // new data
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			update_selected_buffer = false;
			break;
		case VERTEX_EDITING:
		
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER,
					face_selection_array.size() * sizeof(int),// byte offset where to start
					vertex_selection_array.size() * sizeof(int), // size in bytes
					vertex_selection_array.data());			   // new data
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			update_selected_buffer = false;
			break;
		case EDGE_EDITING:
		
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER,
							face_selection_array.size() * sizeof(int)+vertex_selection_array.size() * sizeof(int),	 // byte offset where to start
							edge_selection_array.size() * sizeof(int), // size in bytes
							edge_selection_array.data());				// new data
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			update_selected_buffer = false;
			break;
		default:
		break;
	}

	if (is_window_ready)
	{
		mainRenderPass();
	}
	else
	{
		FramebufferSetup();
		meshRenderSetup();

		is_window_ready = true;
	}
}

void MainWindow::updateFacesSelected(unsigned int face_id,unsigned int mesh_id) {
	unsigned int global_face_id=0;
	for(int i=0;i<mesh_id;i++) {
		global_face_id+=meshes[i]->faces.size();
	}
	// change the state of the face in the global array
	face_selection_array[global_face_id+face_id]=!face_selection_array[global_face_id+face_id];
	update_selected_buffer = FACE_EDITING;
}

void MainWindow::updateVerticesSelected(unsigned int vertex_id,unsigned int mesh_id) {
	unsigned int global_id = 0;
	for (int i = 0; i < mesh_id; i++) {
		global_id += meshes[i]->vertices.size();
	}

	// change the state of the vertex in the global array
	vertex_selection_array[global_id + vertex_id] = !vertex_selection_array[global_id + vertex_id];
	update_selected_buffer = VERTEX_EDITING;
}

void MainWindow::updateEdgesSelected(unsigned int edge_id,unsigned int mesh_id) {
	unsigned int global_id = 0;
	for (int i = 0; i < mesh_id; i++) {
		global_id += meshes[i]->edges.size();
	}

	// change the state of the edge in the global array
	edge_selection_array[global_id + edge_id] = !edge_selection_array[global_id + edge_id];
	update_selected_buffer = EDGE_EDITING;
}

void MainWindow::resetSelectionBuffer(unsigned int type) {


	switch (type)
	{
	case FACE_EDITING:
		std::fill(face_selection_array.begin(), face_selection_array.end(), 0);
		update_selected_buffer = FACE_EDITING;
		break;
	case VERTEX_EDITING:
		std::fill(vertex_selection_array.begin(), vertex_selection_array.end(), 0);
		update_selected_buffer = VERTEX_EDITING;
		break;
	case EDGE_EDITING:
		std::fill(edge_selection_array.begin(), edge_selection_array.end(), 0);
		update_selected_buffer = EDGE_EDITING;
		break;	
	default:
		break;
	}
}


std::optional<std::tuple<unsigned int, unsigned int, unsigned int>> MainWindow::faceDetection(){

	auto [framebuffer_x, framebuffer_y] = getCursorPositionInViewport(window);
	glBindFramebuffer(GL_FRAMEBUFFER, color_picking_framebuffer_info.FBO);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	unsigned int pixel_data[3];
	GLint currentFBO;
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentFBO);
	std::cout << "Reading from FBO: " << currentFBO << " expected: " << color_picking_framebuffer_info.FBO << std::endl;
	glReadPixels(framebuffer_x, framebuffer_y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel_data);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	std::cout << pixel_data[0] << " " << pixel_data[1] <<" "<<pixel_data[2] << std::endl;
	if (pixel_data[0] == 0)
		return std::nullopt; // background
	return {{pixel_data[0]-1, pixel_data[1], pixel_data[2]}};
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
	int framebuffer_y = (height - mouse_y) * (framebuffer_height / (double)height);
	return {framebuffer_x, framebuffer_y};
	}

void MainWindow::mainRenderPass()
{
	selectionBufferPass();
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	int current_offset=0,vertex_offset=face_selection_array.size();
	int edges_offset = face_selection_array.size()+vertex_selection_array.size();

	for (auto mesh : meshes)
	{
		RenderInfo *ri = &mesh_to_render_info[mesh];
		main_shader->use();
		main_shader->setMat4("model", ri->model);
		main_shader->setInt("num_faces_offset", current_offset);
		main_shader->setUint("gui_state", gui->currentState);
		current_offset += mesh->faces.size();

		glBindVertexArray(ri->VAO);
		glDrawElements(GL_TRIANGLES, mesh->face_render_indices.size(), GL_UNSIGNED_INT, 0);
		if(gui->currentState==VERTEX_EDITING) {
			
			vertex_pass_shader->use();
			vertex_pass_shader->setMat4("model", ri->model);
			vertex_pass_shader->setInt("num_vertices_offset", vertex_offset);
			vertex_offset += mesh->vertices.size();
			glDrawArrays(GL_POINTS, 0,mesh->vertices.size());
		}
		/*
		if (gui->currentState == EDGE_EDITING)
		{
			glDisable(GL_CULL_FACE);
			main_pass_edge_shader->use();
			main_pass_edge_shader->setMat4("model", ri->model);
			main_pass_edge_shader->setInt("num_vertices_offset", vertex_offset);
			edges_offset += mesh->edges.size();
			glDrawElements(GL_LINES, mesh->edge_render_indices.size(), GL_UNSIGNED_INT, 0);
		}
		*/
		glBindVertexArray(0);
	}
}

void MainWindow::selectionBufferPass()
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, color_picking_framebuffer_info.FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE); // enable setting point size in the shader
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	unsigned int mesh_id = 0;
	switch (gui->currentState)
	{
	case FACE_EDITING:
		color_picking_shader->use();
		for (auto mesh : meshes)
		{
			RenderInfo *ri = &mesh_to_render_info[mesh];
			color_picking_shader->setMat4("model", ri->model);
			color_picking_shader->setUint("MeshID", mesh_id++);
			color_picking_shader->setUint("gui_state", gui->currentState);
			glBindVertexArray(ri->VAO);
			glDrawElements(GL_TRIANGLES, mesh->face_render_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		break;
	case VERTEX_EDITING:
		color_picking_shader->use();
		for (auto mesh : meshes)
		{
			RenderInfo *ri = &mesh_to_render_info[mesh];
			color_picking_shader->setMat4("model", ri->model);
			color_picking_shader->setUint("MeshID", mesh_id++);
			color_picking_shader->setUint("gui_state", gui->currentState);
			glBindVertexArray(ri->VAO);
			glDrawElements(GL_POINTS, mesh->face_render_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		break;
	case EDGE_EDITING:
		edge_pass_shader->use();
		for (auto mesh : meshes)
		{
			RenderInfo *ri = &mesh_to_render_info[mesh];
			edge_pass_shader->setMat4("model", ri->model);
			edge_pass_shader->setUint("MeshID", mesh_id++);
			glBindVertexArray(ri->edge_VAO);
			glDrawElements(GL_LINES, mesh->edge_render_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		break;
	}
}

/*Setup all the meshes RenderInfo to show on the screen */
void MainWindow::meshRenderSetup()

{
	for (auto mesh : meshes)
	{	//generate VAO,VBO,EBO for the mesh
		RenderInfo *ri = &mesh_to_render_info[mesh];
		glGenVertexArrays(1, &ri->VAO);
		glGenVertexArrays(1, &ri->edge_VAO);
		glGenBuffers(1, &ri->VBO);
		glGenBuffers(1, &ri->EBO);
		glGenBuffers(1, &ri->edge_EBO);
		//send the vertex data to the GPU
		glBindBuffer(GL_ARRAY_BUFFER, ri->VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->face_render_indices.size() * sizeof(unsigned int), mesh->face_render_indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->edge_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->edge_render_indices.size() * sizeof(unsigned int), mesh->edge_render_indices.data(), GL_STATIC_DRAW);

		//setup face VAO
		glBindVertexArray(ri->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, ri->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, point));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		//setup edge VAO
		glBindVertexArray(ri->edge_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, ri->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->edge_EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, point));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		setupSelectionArrays(mesh);
	}
	std::filesystem::path shadersRoot = std::filesystem::path(ROOT_DIR) / "shaders";
	std::string vertex_shader_path = (shadersRoot / "shader.vert").string();
	std::string geometry_shader_path = (shadersRoot / "shader.geom").string();
	std::string fragment_shader_path = (shadersRoot / "shader.frag").string();
	std::string fragment_colorPickingShader_path = (shadersRoot / "colorPickingShader.frag").string();
	std::string fragment_vertexPassShader_path = (shadersRoot / "vertexPass.frag").string();
	std::string fragment_edgePass_path = (shadersRoot / "edgePass.frag").string();
	std::string geometry_edgePass_shader_path = (shadersRoot / "edgePass.geom").string();
	std::string vertex_edgePass_shader_path = (shadersRoot / "edgePass.vert").string();
	std::string main_edge_pass_shader_path = (shadersRoot / "mainEdgePass.frag").string();
	main_shader = new Shader(vertex_shader_path.c_str(), geometry_shader_path.c_str(), fragment_shader_path.c_str());
	color_picking_shader = new Shader(vertex_shader_path.c_str(), fragment_colorPickingShader_path.c_str());
	vertex_pass_shader = new Shader(vertex_shader_path.c_str(), fragment_vertexPassShader_path.c_str());
	edge_pass_shader = new Shader(vertex_edgePass_shader_path.c_str(), geometry_edgePass_shader_path.c_str(), fragment_edgePass_path.c_str());
	main_pass_edge_shader = new Shader(vertex_edgePass_shader_path.c_str(), geometry_edgePass_shader_path.c_str(),main_edge_pass_shader_path.c_str());
	main_shader->use();
	main_shader->setMat4("view", view);
	main_shader->setMat4("projection", projection);

	//Setup the combined face selection array to color selection on the window
	std::vector<int> combined_selection_array;
	combined_selection_array.reserve(face_selection_array.size() + vertex_selection_array.size() + edge_selection_array.size());
	combined_selection_array.insert(combined_selection_array.end(), face_selection_array.begin(), face_selection_array.end());
	combined_selection_array.insert(combined_selection_array.end(), vertex_selection_array.begin(), vertex_selection_array.end());
	combined_selection_array.insert(combined_selection_array.end(), edge_selection_array.begin(), edge_selection_array.end());

	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (face_selection_array.size() + vertex_selection_array.size() + edge_selection_array.size()) * sizeof(int),
				 combined_selection_array.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	color_picking_shader->use();
	color_picking_shader->setMat4("view", view);
	color_picking_shader->setMat4("projection", projection);

	vertex_pass_shader->use();
	vertex_pass_shader->setMat4("view", view);	
	vertex_pass_shader->setMat4("projection", projection);

	edge_pass_shader->use();
	edge_pass_shader->setMat4("view", view);
	edge_pass_shader->setMat4("projection", projection);

	main_pass_edge_shader->use();
	main_pass_edge_shader->setMat4("view", view);
	main_pass_edge_shader->setMat4("projection", projection);
}


void MainWindow::setupSelectionArrays(Mesh* mesh) {


	for(int i=0;i<mesh->faces.size();i++) {
		face_selection_array.push_back(0);
	}
	for (int i = 0; i < mesh->vertices.size(); i++)
	{
		vertex_selection_array.push_back(0);
	}
	for (int i = 0; i < mesh->edges.size(); i++)
	{
		edge_selection_array.push_back(0);
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
	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
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
		RenderInfo *ri = &mesh_to_render_info[mesh];
		glDeleteVertexArrays(1, &ri->VAO);
		glDeleteBuffers(1, &ri->VBO);
		glDeleteBuffers(1, &ri->EBO);

	}
	delete main_shader;
	delete color_picking_shader;
}