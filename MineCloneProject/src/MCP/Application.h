#pragma once
#include "Window.h"
#include "LayerStack.h"

//DEBUG
#include "Object/Mesh/MeshLoader.h"
#include "MCP/Renderer/Shader/Shader.h"
#include <memory>
#include "MCP/Renderer/Camera/Camera.h"

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

		void OnEvent(Event& e);

		inline void setRunning(bool isRunning) { m_Running = isRunning; }

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);
	private:
		static Application* s_Instance;
	
	private:
		bool m_Running;
		Window* m_Window;
		LayerStack m_LayerStack;

		//DEBUG
	private:
		vec2 xoffset;
		Camera test = vec3( 0.0f, 0.0f, 10.0f);
		MC::Mesh mesh;
		MC::Shader* shader;
		unsigned int CubeVBO, CubeVAO, CubeEBO;
		//DEBUG
	};

	Application* CreateApplication();

}