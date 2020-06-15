#include "main.h"

void _gameMain::init()
{
	map.init();
	player.init(&map);
}
void _gameMain::update()
{
	if (KeyF5.down()) init();
	player.update();
	map.update();

	draw();
}
void _gameMain::draw()
{
	map.draw();
	player.draw();
}