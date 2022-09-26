#include "Application.hpp"


int main()
{

	//if (glewInit() != GLEW_OK)
	//	std::cout << "Error! \n";
	Application::Init();

	while (Application::IsRunning())
	{
		Application::Update();
	}
	Application::Exit();
}