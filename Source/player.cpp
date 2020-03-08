#include "main.h"

void _player::init(_mapData* m)
{
	map = m;
	pos.set(480, 400);
	speed.set(0, 0);
	hitBox.size.set(60, 60);
}
void _player::update()
{
	jump();
	dash();
	move();
}
void _player::draw()
{
	RectF(pos, hitBox.size).draw(ColorF(0.3, 0.8, 0.4, 0.5));
}


void _player::jump()
{
	if (KeySpace.pressed())
	{
		//すでに二弾ジャンプしていない、かつ押された瞬間なら
		if (!wasSpacePressed)
		{
			if(jumpCnt < 2)
			{
				speed.y = 0;
				spacePressedFrame = 0;
				wasSpacePressed = true;
			}
		}else //押され続けていたら
		{
			spacePressedFrame++;
			if (spacePressedFrame < 13)
				speed.y = JUMP_POWER;
		}
	}
	else
	{
		if (wasSpacePressed)
			jumpCnt++;
		spacePressedFrame = 0;
		wasSpacePressed = false;
	}
}
void _player::dash()
{
	//ダッシュ中なら更新する
	if(dashing)
	{
		speed.set(20*dashSp, 0);
		dashingTime++;
		if(dashingTime > 18)
		{
			dashing = false;
			dashSp = 0;
			dashingTime = 0;
		}
	}
	//ダッシュしてなかったら
	else
	{
		speed.x = 0;
		if(KeyD.down()) dashSp = 1;
		if(KeyA.down())	dashSp = -1;
		if(dashSp!=0)
		{
			dashing = true;
			dashingTime = 0;
		}
	}
}
void _player::move()
{
	map->scr.x -= speed.x;
	pos.y -= speed.y;
	//重力
	speed.y--;
	if (pos.y >= 900-hitBox.size.y)
	{
		jumpCnt = 0;
		pos.y = 900-hitBox.size.y;
		speed.y = 0;
	}
}
