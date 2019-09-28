#pragma once
#include "Window.h"

namespace MC
{

	/*
	* Aqui � a central do programa, basicamente poderia se chamar "Engine", a Application � um singleton (como s� temos uma Application) que ter�
	  suas janelas, seus sistemas de renderer e ir� tamb�m distribuir os eventos que � recebido por meio de nossa janela.
	  Aqui tamb�m atualizaremos todos os sistemas e com isso daremos oportunidade ao App.cpp funcionar perfeitamente.
	*/

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

	private:
		static Application* s_Instance;
		
	private:
		bool m_Running;
		Window* m_Window;
	};

	Application* CreateApplication();

}