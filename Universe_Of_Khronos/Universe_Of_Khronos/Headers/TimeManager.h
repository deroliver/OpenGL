#ifndef TIMEMANAGER_H_
#define TIMEMANAGER_H_

#include <chrono>
#include <thread>


// TimeManager class to calculate frame rate and give a time slice
class TimeManager {
	
public:
	static TimeManager& Instance() {

		static TimeManager instance;

		return instance;
	}

	// This calculate our current frames per second
	double CalculateFrameRate(bool Write) {

		static double FramesPerSecond = 0.0f;		
		static double StartTime = GetTime();		
		static double LastTime = GetTime();			
		static char strFrameRate[50] = { 0 };		
		static double CurrentFPS = 0.0f;			

		CurrentTime = GetTime();

		DeltaTime = CurrentTime - LastTime;

		LastTime = CurrentTime;

		++FramesPerSecond;

		if (CurrentTime - StartTime > 1.0f) {

			StartTime = CurrentTime;

			if (Write)
				fprintf(stderr, "FPS: %d\n", int(FramesPerSecond));
				
			CurrentFPS = FramesPerSecond;	
			FramesPerSecond = 0;
		}

		return CurrentFPS;
	}

	double TimeManager::GetTime() {
		auto BeginningOfTime = std::chrono::system_clock::now().time_since_epoch();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(BeginningOfTime).count();
		return ms * 0.001;
	}

	void Sleep(int MS) {
		std::this_thread::sleep_for(std::chrono::milliseconds(MS));
	}

	double DeltaTime = 0;
	double CurrentTime = 0;

private:
	TimeManager() {}							 // Private constructor -- Cannot be called
	TimeManager(TimeManager const&);			 // Copy constructor is private
	TimeManager& operator=(TimeManager const&);  // Assignment operator is private
};

#endif