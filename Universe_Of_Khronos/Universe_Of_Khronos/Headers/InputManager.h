#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#define GLM_FORCE_RADIANS	

#include "../Headers/Camera.h"
#include "../Headers/TimeManager.h"

enum InputCode {
	Escape = 1,
	Space = 2,
	Left = 3,
	Up = 4,
	Right = 5,
	Down = 6,
	X = 7,
};



class InputManager {

public:

	// Takes a code and sends it to the camera 
	void KeyPressed(InputCode c) {

		if (Camera == nullptr)
			return;

		switch (c) {

		case Up:
			Camera->MoveCamera(Camera->GetSpeed() * TimeManager::Instance().DeltaTime);
			break;
		case Down:
			Camera->MoveCamera(-1 * Camera->GetSpeed() * TimeManager::Instance().DeltaTime);
			break;
		case Left:
			Camera->Strafe(-1 * Camera->GetSpeed() * TimeManager::Instance().DeltaTime);
			break;
		case Right:
			Camera->Strafe(Camera->GetSpeed() * TimeManager::Instance().DeltaTime);
			break;
		}
	}

	void SetCamera(Camera *Cam) { Camera = Cam; }
	Camera *GetCamera() { return Camera; }


	void MouseMoved(float X, float Y) {
		if (Camera == nullptr)
			return;

		Camera->SetViewByMouse(X, Y);
	}

protected:

	Camera *Camera;
};


#endif