#include "Engine.h"

Engine* Engine::m_instance = NULL;

Engine::Engine(void)
{
	m_graphics = NULL;
}

Engine::~Engine(void)
{
	if (m_graphics)
	{
		delete m_graphics;
		m_graphics = NULL;
	}
}

bool Engine::InitializeGraphics(HWND hwnd)
{
	m_graphics = new WE::Graphics::Graphics();

	return m_graphics->InitializeDX(hwnd);
}

bool Engine::Initialize(HINSTANCE instance, HWND hwnd)
{
	m_graphics->Initialize();

	return true;
}

void Engine::Run(void)
{
	Update();
	Render();
}

void Engine::Release(void)
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

WE::Graphics::Graphics* Engine::GetGraphics(void)
{
	return m_graphics;
}

Engine* Engine::GetEngine(void)
{
	if (m_instance == NULL)
	{
		m_instance = new Engine();
	}

	return m_instance;
}

void Engine::Update(void)
{

}

void Engine::Render(void)
{
	m_graphics->BeginScene(0.f, 0.f, 0.f, 1.f);

	//render stuff goes here
	m_graphics->EndScene();
}