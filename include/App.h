#pragma once

#include <string>

#include "Window.h"
#include "Instance.h"
#include "Device.h"

class App
{
public:
	App();

	void start();

private:
	Window m_window;
	Instance m_instance{m_window};
	Device m_device;

	void init();
	void update();
	void loop();
	void draw();
};
