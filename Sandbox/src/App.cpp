#include <MC.h>
#include "WorldLayer.h"

/*
* Detalhes sobre a implementa��o em Application.h
*/

class SandBoxApp : public MC::Application
{
public:
	SandBoxApp() { PushLayer(new WorldLayer()); };
	~SandBoxApp() {};
};

MC::Application* MC::CreateApplication()
{
	return new SandBoxApp();
}