#include "Application.hpp"


int main()
{
	Application::Init();

	while (Application::IsRunning())
	{
		Application::Update();
	}
	Application::Exit();
}