#include "App.h"

App::App()
	: m_device(m_instance)
{
}

void App::start()
{
	init();
	while (!m_window.isWindowClosed())
	{
		update();
		loop();
		draw();
	}
}

void App::init()
{
}

void App::update()
{
	glfwPollEvents();
}

void App::loop()
{
}

void App::draw()
{
}
