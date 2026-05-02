//
// Created by rodrigo on 29/04/2026.
//

#include "../include/GUI.h"

#include "glad/glad.h"
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void GUI::showMainWindowGUI() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ImGui::Begin("My Tools");

	bool faceActive = main_state.isFaceSelectionActive;
	bool vertexActive = main_state.isVertexSelectionActive;
	bool edgeActive = main_state.isEdgeSelectionActive;


	// Face button
	ImGui::PushStyleColor(ImGuiCol_Button, faceActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, faceActive ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, faceActive ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Face Selection:");
	ImGui::SameLine();
	ImGui::BeginDisabled(vertexActive || edgeActive);
	if (ImGui::Button(faceActive ? "ON##face" : "OFF##face")){
		main_state.isFaceSelectionActive = !main_state.isFaceSelectionActive;
		currentState=FACE_EDITING;
	}
	ImGui::PopStyleColor(3);
	ImGui::EndDisabled();


	// Vertex button
	ImGui::PushStyleColor(ImGuiCol_Button, vertexActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vertexActive ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vertexActive ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Vertex Selection:");
	ImGui::SameLine();
	ImGui::BeginDisabled(faceActive || edgeActive);
	if (ImGui::Button(vertexActive ? "ON##vertex" : "OFF##vertex"))
	{
		main_state.isVertexSelectionActive = !main_state.isVertexSelectionActive;
		currentState=VERTEX_EDITING;
	}
	ImGui::PopStyleColor(3);
	ImGui::EndDisabled();

	// Edge button
	ImGui::PushStyleColor(ImGuiCol_Button, edgeActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, edgeActive ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, edgeActive ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Edge Selection:");
	ImGui::SameLine();
	ImGui::BeginDisabled(vertexActive || faceActive);
	if (ImGui::Button(edgeActive ? "ON##edge" : "OFF##edge"))
	{
		main_state.isEdgeSelectionActive = !main_state.isEdgeSelectionActive;
		currentState=EDGE_EDITING;
	}
	
	ImGui::PopStyleColor(3);
	ImGui::EndDisabled();
	ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}