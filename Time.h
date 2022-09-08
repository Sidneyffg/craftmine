#ifndef TIME_CLASS_H
#define TIME_CLASS_H

#include <glm/glm.hpp>

class Time
{
public:
	Time(bool logFPS);

	void tick(glm::vec3 camPos);
	double deltaTime();
	double fps();
	bool toggleFPSLog();
private:
	bool logFPS;
	double lastTime;
	double currentTime;
	double delta;
	double logTime;
};

#endif