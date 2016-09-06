#ifndef ENGINE_H
#define ENGINE_H

#include "Graphics.h"

class Engine
{
public:
	~Engine(void);
	
	bool InitializeGraphics(HWND hwnd);
	bool Initialize        (HINSTANCE instance, HWND hwnd);
	void Run               (void);

	void Release(void);

	WE::Graphics::Graphics* GetGraphics(void);
	static Engine* GetEngine(void);

private:
	//Singleton.
	Engine(void);

	void Update(void);
	void Render(void);

	WE::Graphics::Graphics* m_graphics;
	static Engine* m_instance;
};

#endif