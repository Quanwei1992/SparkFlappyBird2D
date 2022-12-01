#pragma once

#include "Level.h"

#include <Spark.h>

struct ImFont;

class GameLayer : public Spark::Layer
{
public:
	GameLayer();
	virtual ~GameLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Spark::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Spark::Event& e)override;
private:
	bool OnWindowResize(Spark::WindowResizeEvent& e);
	bool OnMouseButtonPressed(Spark::MouseButtonPressedEvent& e);
	void CreateCamera(uint32_t width, uint32_t height);
private:
	Spark::Scope<Spark::OrthographicCamera> m_Camera;
	Level m_Level;
	ImFont* m_Font = nullptr;
	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play = 0,MainMenu,GameOver = 2
	};
	GameState m_State = GameState::MainMenu;

};