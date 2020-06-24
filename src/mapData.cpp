#include "main.h"

void _mapData::init()
{
	scr.set(0, 0);
	mapTexture = RenderTexture(width()*MAP_CHIPSIZE, height()*MAP_CHIPSIZE, Palette::Black);
	origin     = RenderTexture(width()*MAP_CHIPSIZE, height()*MAP_CHIPSIZE, Palette::Black);

	//読み込み+書き込み
	const CSVData csv(U"Data/map.csv");
	int heightCSV = csv.columns(0);
	int  widthCSV = csv.rows();
	map = Grid<int>(heightCSV, widthCSV);

	{
		ScopedRenderTarget2D a(mapTexture);
		for (auto y : step( height() ))
		for (auto x : step(  width() ))
		{
			int n = Parse<int>(csv[y][x]);
			set(y, x, n);
			//mapTexture描画
			if (get(y, x) != 0)
				Rect(x * MAP_CHIPSIZE, y * MAP_CHIPSIZE, MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(ColorF(0.4, 0.2, 0.6));
		}
	}
	{
		ScopedRenderTarget2D a(origin);
		mapTexture.draw();
	}
}

void _mapData::update()
{
	if(!origin) Shader::Copy(mapTexture, origin);
	if( scr.x > 0) scr.x = 0;
	if( scr.x < WINDOW_X-textureWidth())
		scr.x = WINDOW_X-textureWidth();
	Print << U"scr={}"_fmt(scr);
}

void _mapData::draw()
{
	mapTexture.draw(scr);
	Shader::Copy(origin, mapTexture);
}


inline int _mapData::get(int y, int x)
{
	if ( (y<0||y>map.height()) || (x<0 || x>map.width()) )
		return 0;
	return map[y][x];
}
inline int _mapData::get(Point p)
{
	if ( (p.y<0||p.y>map.height()) || (p.x<0 || p.x>map.width()) )
		return 0;
	return map[p.y][p.x];
}
void _mapData::set(int y, int x, int n)
{
	if (y<0 || y>map.height() || x<0 || x>map.width())
		return;
	map[y][x] = n;
}

///////////////////////////////////////////
//当たり判定

//プレイヤー上下左右の、マップとの接触情報を取得
//Vec2（位置）、Rect（サイズ）、Vec2（速度）
//いまのところあたったところを表示するだけ
_mapHitState _mapData::checkMapHitState(Vec2 pos, Vec2 speed, const Rect hitBox)
{
	//checkPos
	constexpr int LEFT_TOP      = 0;
	constexpr int LEFT_BOTTOM   = 1;
	constexpr int RIGHT_TOP     = 2;
	constexpr int RIGHT_BOTTOM  = 3;
	pos - scr;
	int left = pos.x/MAP_CHIPSIZE,  right = (pos.x + hitBox.size.x)/MAP_CHIPSIZE;
	int  top = pos.y/MAP_CHIPSIZE, bottom = (pos.y + hitBox.size.y)/MAP_CHIPSIZE;

	_mapHitState hit;
	hit.pos = pos;

	Point checkPos[4];
	checkPos[0] = Point(left,  top);
	checkPos[1] = Point(left,  bottom);
	checkPos[2] = Point(right, top);
	checkPos[3] = Point(right, bottom);
	//デバッグ表示
	Rect box;
	for(auto i: step(4)) if( get(checkPos[i]) )
	{
		box = Rect(checkPos[i].x * MAP_CHIPSIZE, checkPos[i].y * MAP_CHIPSIZE, MAP_CHIPSIZE);
		ScopedRenderTarget2D a(mapTexture);
		Rect((checkPos[i].x*MAP_CHIPSIZE ), checkPos[i].y*MAP_CHIPSIZE, MAP_CHIPSIZE)
			.draw(HSV(90 * i, 1.0, 0.7));
	}
	//デバッグ表示部おわり


	bool hitLocate[4];
	for (auto i:step(4))
		hitLocate[i] = false;
	//横方向の当たり判定
	{
		//右に移動中
		if(speed.x>0)
		{
			if(get(checkPos[RIGHT_TOP]))
			{
				hit.pos.x = checkPos[RIGHT_TOP]   .x - hitBox.size.x;
				hit.right = true;
			}
			if(get(checkPos[RIGHT_BOTTOM]))
			{
				hit.pos.x = checkPos[RIGHT_BOTTOM].x - hitBox.size.x;
				hit.right = true;
			}
		}
		//左に移動中
		if(speed.x<0)
		{
			if(get(checkPos[LEFT_TOP]))
			{
				hit.pos.x = checkPos[LEFT_TOP]   .x;
				hit.left = true;
			}
			if(get(checkPos[LEFT_BOTTOM]))
			{
				hit.pos.x = checkPos[LEFT_BOTTOM].x;
				hit.left = true;
			}
		}
	}
	//縦方向の当たり判定
	{
		//上昇中
		if(speed.y<0)
		{
			if(get(checkPos[RIGHT_BOTTOM]))
			{
				hit.pos.y = checkPos[RIGHT_BOTTOM].y;
				hit.left = true;
			}
			if(get(checkPos[LEFT_BOTTOM]))
			{
				hit.pos.y = checkPos[LEFT_BOTTOM] .y;
				hit.left = true;
			}
		}
		//落下中
		if(speed.y>0)
		{
			if(get(checkPos[RIGHT_TOP]))
			{
				hit.pos.y = checkPos[RIGHT_TOP].y + hitBox.size.y;
				hit.left = true;
			}
			if(get(checkPos[LEFT_TOP]))
			{
				hit.pos.y = checkPos[LEFT_TOP] .y + hitBox.size.y;
				hit.left = true;
			}
		}
	}



	//スクロール目標=pos.x = hit.pos.x
	//最初にpos - scrされているのでscrを足す
	scr.x = (pos.x + scr.x) - hit.pos.x;
	return hit;
}
