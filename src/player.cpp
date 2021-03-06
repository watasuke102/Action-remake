#include "main.h"

void _player::init(_mapData* m)
{
	debug = U"void";
	map = m;
	pos.set(480, 400);//480,400
	speed.set(0, 0);
	hitBox.size.set(60, 60);
}
void _player::update()
{
	Print << debug << U'\n';
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
	int sp;
	//シフトキーで速度アップ
	if (KeyShift.pressed())
		sp = PLAYER_HIGHSPEED;
	else sp = PLAYER_SPEED;
	//左右移動
	if(!dashing)
	{
		if (KeyLeft.pressed())  speed.x = -sp;
		if (KeyRight.pressed()) speed.x =  sp;
	}

	pos.y -= speed.y;

	/**
	if (KeyUp.pressed())   pos.y -= 5;
	if (KeyDown.pressed()) pos.y += 5;
	map->scr.x -= speed.x;
	return;
	/**/

	//重力
	speed.y--;
	/**
	if (pos.y >= 900-hitBox.size.y)
	{
		jumpCnt = 0;
		pos.y = 900-hitBox.size.y;
		speed.y = 0;
	}
	/**/
	//当たり判定
	_mapHitState hit = map->checkMapHitState(pos-map->scr, speed, hitBox);

	Print << U"top:{},bottom:{},right:{},left:{}"_fmt(hit.top, hit.bottom, hit.right, hit.left);

	if(hit.bottom)
		jumpCnt = 0;
	if(hit.top || hit.bottom)
	{
		speed.y = 0;
	}
	if(hit.left || hit.right)
	{
		//speed.x =/**/ hit.pos.x - (pos.x - map->scr.x);
		//debug = U"speed.xはhit.pos.x[{}] - (pos.x[{}] - map->scr.x[{}])の結果{}となりました"_fmt(hit.pos.x, pos.x, map->scr.x, speed.x);
		/*/pos.x-(hit.pos.x + map->scr.x);
		/**/
		speed.x = map->scr.x - (pos.x - hit.pos.x);
		debug = U"speed.x = map->scr.x[{}] - (pos.x[{}]-hit.pos.x[{}]) = {}"_fmt(map->scr.x, pos.x, hit.pos.x, speed.x);
		dashingTime = 20;//次のupdate()でダッシュを終了させる
	}
	//当たり判定終わり

	//移動
	Print << U"hit{} player{}"_fmt(hit.pos, pos-map->scr);
	Print << U"playerSpeed" << speed;
	pos.y = hit.pos.y;
	map->scr.x -= speed.x;
}
