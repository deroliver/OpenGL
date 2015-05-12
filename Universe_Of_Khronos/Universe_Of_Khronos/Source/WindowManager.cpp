#include "../Headers/WindowManager.h"
#include "../Headers/Main.h"



int main() {

	WindowManager *WM = new WindowManager;

	Camera *Cam = new Camera();

	Main main;
	main.SetWindowManager(WM);

	main.SetCamera(Cam);
	WM->GetInputManager()->SetCamera(Cam);

	return main.KMain();
}


int WindowManager::Initialize(int W, int H, std::string Title, bool Full) {

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (Full)
		Window = glfwCreateWindow(W, H, Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		Window = glfwCreateWindow(W, H, Title.c_str(), nullptr, nullptr);


	if (Window == nullptr) {
		fprintf(stderr, "Failed to create a GLFW window\n");
		Destroy();

		return -1;
	}

	glfwMakeContextCurrent(Window);
	glfwSetInputMode(Window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();

	if (GLEW_OK != err) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	return 0;
}


void WindowManager::SwapTheBuffers() {

	glfwSwapBuffers(Window);
}


bool WindowManager::ProcessInput(bool Continue = true) {
	bool firstmouse = true;

	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(Window) != 0)
		return false;

	if (glfwGetKey(Window, GLFW_KEY_UP) || glfwGetKey(Window, GLFW_KEY_W))
		InputManager.KeyPressed(InputCode::Up);
	if (glfwGetKey(Window, GLFW_KEY_DOWN) || glfwGetKey(Window, GLFW_KEY_S))
		InputManager.KeyPressed(InputCode::Down);
	if (glfwGetKey(Window, GLFW_KEY_LEFT) || glfwGetKey(Window, GLFW_KEY_A))
		InputManager.KeyPressed(InputCode::Left);
	if (glfwGetKey(Window, GLFW_KEY_RIGHT) || glfwGetKey(Window, GLFW_KEY_D))
		InputManager.KeyPressed(InputCode::Right);

	double mouseX, mouseY;
	glfwGetCursorPos(Window, &mouseX, &mouseY);

	if (mouseX != 0 && mouseY != 0) {
		InputManager.MouseMoved((float)mouseX, (float)mouseY);
	}

	glfwSetCursorPos(Window, 0, 0);

	glfwPollEvents();

	return Continue;
}


void WindowManager::Destroy() {
	glfwTerminate();
}