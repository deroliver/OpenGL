#ifndef MAIN_H_
#define MAIN_H_

#include "../Headers/WindowManager.h"
#include "../Headers/Camera.h"

static const GLuint WIDTH = 1600;
static const GLuint HEIGHT = 900;

class Main {
public:

	Main() {}
	~Main() { Destroy(); }

	int KMain();

	WindowManager *GetWindowManager() { return WindowManager; }
	void SetWindowManager(WindowManager *WM) { WindowManager = WM; }

	Camera *GetCamera() { return Camera; }
	void SetCamera(Camera *Cam) { Camera = Cam; }

	void Initialize();
	void GameLoop();
	void Destroy();

protected:

	WindowManager *WindowManager;
	Camera *Camera;
};

#endif