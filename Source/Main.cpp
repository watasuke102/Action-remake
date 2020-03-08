#include "main.h"

void Main()
{
/**/
	Graphics::SetTargetFrameRateHz(60);
	Window::SetStyle(WindowStyle::Sizable);
	Window::Maximize();
	Scene::Resize(1920,1080);

	_gameMain gameMain;
	int sp = 2;
	while (System::Update())
	{
		gameMain.update();
	}
}
/**/