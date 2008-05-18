#include "application.h"

#include <windows.h>

Application::Application()
{
	// Create content view
    mView = new MyView;
    mView->init();
}

Application::~Application()
{
    delete mView;
}

void Application::run()
{
    while(1)
    {
        mView->pump();
        mView->drawRect();
        Sleep(1000/30);
    }
}