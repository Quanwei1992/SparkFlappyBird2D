#include "GameLayer.h"

#include <Spark.h>
#include <Spark/Core/EntryPoint.h>

class Sandbox : public Spark::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameLayer());
	}
};


Spark::Application* Spark::CreateApplication()
{
	return new Sandbox();
}


