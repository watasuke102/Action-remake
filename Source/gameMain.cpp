#include "main.h"

void _gameMain::init()
{
	map.scr.set(0, 0);
	const CSVData csv(U"Data/map.csv");
	double height = csv.columns(0);
	double wight  = csv.rows();
	map.init(height, wight);
	for (auto y : step(map.height()))
	for (auto x : step(map.width()))
	{
		int n = Parse<int>(csv[y][x]);
		map.set(y, x, n);
	}

	player.init(&map);
	mapDraw();
}
void _gameMain::update()
{
	static int sp=10;
	if (KeyLeft.pressed())  map.scr.x+=sp;
	if (KeyRight.pressed()) map.scr.x-=sp;
	if (KeyUp.down())       sp+=5;
	if (KeyDown.down())     sp-=5;

	if (KeyF5.down()) init();
	player.update();
	draw();
}
void _gameMain::draw()
{
	mapTexture.draw(map.scr);
	player.draw();
}

void _gameMain::mapDraw()
{
	Size mapSize(map.width() * MAP_CHIPSIZE, map.height() * MAP_CHIPSIZE);
	mapTexture = RenderTexture(mapSize, ColorF(0.1));
	ScopedRenderTarget2D target(mapTexture);
	for (auto y : step(map.height()))
	for (auto x : step(map.width()))
	{
		int mapStat = map.get(y, x);
		if (mapStat != 0)
		{
			HSV col(mapStat * (360/9), 0.5);
			col = ColorF(0.1, 0.2, 0.6);
			Rect((x*MAP_CHIPSIZE), y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
				.draw(col);
		}
	}
}