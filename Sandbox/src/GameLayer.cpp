#include "GameLayer.h"
#include "Random.h"


#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Spark;

GameLayer::GameLayer()
	:Layer("GameLayer")
{
	auto& window = Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());

	Random::Init();
}


GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	m_Level.Init();
	ImGuiIO io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf",120.0f);
}

void GameLayer::OnDetach()
{
	
}

void GameLayer::OnUpdate(Spark::Timestep ts)
{
	m_Time += ts;
	if ((int)(m_Time * 10.0f) % 8 > 4)
	{
		m_Blink = !m_Blink;
	}

	if (m_Level.IsGameOver())
	{
		m_State = GameState::GameOver;
	}

	const auto& playerPos = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ playerPos.x,playerPos.y,0.0f });

	if (m_State == GameState::Play)
	{
		m_Level.OnUpdate(ts);
	}

	// Render
	Spark::RenderCommand::SetClearColor({ 0.0f,0.0f,0.0f,1.0f });
	Spark::RenderCommand::Clear();

	Spark::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	Spark::Renderer2D::EndScene();

}

void GameLayer::OnImGuiRender()
{
	switch (m_State)
	{
	case GameState::Play:
	{
		uint32_t playerScore = m_Level.GetPlayer().GetScore();
		std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
		break;
	}
	case GameState::MainMenu:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Application::Get().GetWindow().GetWidth();
		auto height = Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;
		if (m_Blink)
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click space to Play!");
		break;
	}
	case GameState::GameOver:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Application::Get().GetWindow().GetWidth();
		auto height = Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;
		if (m_Blink)
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click space to Play!");

		pos.x += 200.0f;
		pos.y += 150.0f;
		uint32_t playerScore = m_Level.GetPlayer().GetScore();
		std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
		break;
	}
	}
	m_Level.OnImGuiRender();
}

void GameLayer::OnEvent(Spark::Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(SK_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<KeyPressedEvent>(SK_BIND_EVENT_FN(GameLayer::OnKeyPressed));
}

bool GameLayer::OnWindowResize(Spark::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}

bool GameLayer::OnKeyPressed(Spark::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == SK_KEY_SPACE)
	{
		if (m_State == GameState::GameOver)
		{
			m_Level.Reset();
		}
		if (m_State != GameState::Play)
		{
			m_State = GameState::Play;

			return true;
		}
	}
	return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / (float)height;
	float camWidth = 8.0f;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	m_Camera = CreateScope<OrthographicCamera>(left, right, bottom, top);
}
