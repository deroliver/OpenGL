#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#define GLM_FORCE_RADIANS	
#define GLEW_STATIC

#include <string>
#include <fstream>
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "../Headers/InputManager.h"

// Class to manage creation of the window, input, and creating OpenGL context.
// This class is inherited from for specific implementations (GLFWManager)
class WindowManager {

public:

	WindowManager() {}
	~WindowManager() { Destroy(); }


	int Initialize(int width, int height, std::string Title, bool FullScreen = false);

	void SwapTheBuffers();

	bool ProcessInput(bool continueGame);

	void Destroy();

	InputManager *GetInputManager() { return &InputManager; }

	GLFWwindow *GetWindow() { return Window; }

protected:

	GLFWwindow *Window;
	InputManager InputManager;
};

#endif