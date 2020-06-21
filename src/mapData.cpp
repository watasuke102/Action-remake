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
void _mapData::checkMapHitState(Vec2 pos, Vec2 speed, Rect hitBox)
{
	constexpr int LEFT_TOP     = 0;
	constexpr int LEFT_BOTTOM  = 1;
	constexpr int RIGHT_TOP    = 2;
	constexpr int RIGHT_BOTTOM = 3;
	int left = pos.x/MAP_CHIPSIZE,  right = (pos.x + hitBox.size.x)/MAP_CHIPSIZE;
	int  top = pos.y/MAP_CHIPSIZE, bottom = (pos.y + hitBox.size.y)/MAP_CHIPSIZE;
	struct
	{
		bool  isActive;
		int   locate; //右上、左下など
		Point pos;
	}checkPos[4];
	checkPos[0].pos = Point(left,  top);
	checkPos[1].pos = Point(left,  bottom);
	checkPos[2].pos = Point(right, top);
	checkPos[3].pos = Point(right, bottom);
	Rect box;
	for(auto i: step(4))
	{
		if(get(checkPos[i].pos))
		{
			box = Rect(checkPos[i].pos.x * MAP_CHIPSIZE, checkPos[i].pos.y * MAP_CHIPSIZE, MAP_CHIPSIZE);
			{
				ScopedRenderTarget2D a(mapTexture);
				Rect((checkPos[i].pos.x*MAP_CHIPSIZE ), checkPos[i].pos.y*MAP_CHIPSIZE, MAP_CHIPSIZE)
					.draw(HSV(90 * i, 1.0, 0.7));
			}
			if(const auto points = box.intersectsAt(hitBox))
			{
				Print << U"POINT ACTIVE: " << i;
				for(const auto& point : points.value())
					Circle(point, 5).draw(Palette::Magenta);
			}
		}
	}
}
