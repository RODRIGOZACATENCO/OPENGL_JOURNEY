//
// Created by rodrigo on 29/04/2026.
//

#ifndef VERTEXEDITOR_GUI_H
#define VERTEXEDITOR_GUI_H
#include "ShaderHandler.h"
enum GUIState
{
	FACE_EDITING=1,
	VERTEX_EDITING=2,
	EDGE_EDITING=3
	
};
struct MainGUI_state
{
	bool isFaceSelectionActive = false;
	bool isVertexSelectionActive = false;
	bool isEdgeSelectionActive = false;
};

class GUI{

public:
	GUIState currentState = FACE_EDITING;
	unsigned int reset_selection_buffer_flag=0;//flag to reset the selection buffer on the GPU when the clear button is pressed
	MainGUI_state main_state;
	void showMainWindowGUI();


};


#endif //VERTEXEDITOR_GUI_H