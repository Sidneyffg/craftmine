#ifndef TIME_CLASS_H
#define TIME_CLASS_H

class Time
{
public:
	Time(bool logFPS);

	void tick();
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