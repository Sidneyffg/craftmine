#include <iostream>
#include <GLFW/glfw3.h>
#include "Time.h"

Time::Time(bool logFPS) {
	Time::lastTime = glfwGetTime();
	Time::logTime = Time::lastTime;
	Time::logFPS = logFPS;

	Time::delta = 0;
	Time::currentTime = Time::lastTime;
}

// Updates deltaTime to current frame
void Time::tick() {
	Time::currentTime = glfwGetTime() * 1000;
	Time::delta = Time::currentTime - Time::lastTime;
	Time::logTime += Time::delta;
	if (Time::logTime > 1000) {
		if (Time::logFPS == true) { std::cout << 1000 / Time::delta << std::endl; }
		Time::logTime -= 1000;
	}
	Time::lastTime = Time::currentTime;
}

// Returns fps from last time Time.tick() was called
double Time::fps() {
	return 1000 / Time::delta;
}

// Returns deltaTime from last time Time.tick() was called
double Time::deltaTime() {
	return Time::delta;
}

// Toggles fps log in the console
bool Time::toggleFPSLog() {
	Time::logFPS = !Time::logFPS;
	return Time::logFPS;
}