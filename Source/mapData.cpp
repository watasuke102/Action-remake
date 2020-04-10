#include "main.h"

void _mapData::init()
{
	scr.set(0, 0);
	mapTexture = RenderTexture(width()*MAP_CHIPSIZE, height()*MAP_CHIPSIZE, Palette::Black);
	ScopedRenderTarget2D a(mapTexture);

	//読み込み
	const CSVData csv(U"Data/map.csv");
	int heightCSV = csv.columns(0);
	int  widthCSV = csv.rows();
	map = Grid<int>(heightCSV, widthCSV);

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
	origin = mapTexture;
}

void _mapData::update()
{
	if( scr.x > 0) scr.x = 0;
	if( scr.x < WINDOW_X-textureWidth())
		scr.x = WINDOW_X-textureWidth();
	Print << U"scr={}"_fmt(scr);
}

void _mapData::draw()
{
	mapTexture.draw(scr);
	mapTexture = origin;
}


inline int _mapData::get(int y, int x)
{
	if ( (y<0||y>map.height()) || (x<0 || x>map.width()) )
		return 0;
	return map[y][x];
}
void _mapData::set(int y, int x, int n)
{
	if (y<0 || y>map.height() || x<0 || x>map.width())
		return;
	map[y][x] = n;
}

///////////////////////////////////////////
//当たり判定

//プレイヤー上下左右の、マップとの接触情報を取得（mapHitState型）
//Vec2（位置）、Rect（サイズ）、Vec2（速度）
_mapHitState _mapData::checkMapHitState(Vec2 pos, Vec2 speed, Rect hitBox)
{
	_mapHitState hit;
	Size size(hitBox.size);
	int left = pos.x/MAP_CHIPSIZE, center = (pos.x + size.x/2)/MAP_CHIPSIZE,  right = (pos.x + size.x)/MAP_CHIPSIZE;
	int  top = pos.y/MAP_CHIPSIZE, middle = (pos.y + size.y/2)/MAP_CHIPSIZE, bottom = (pos.y + size.y)/MAP_CHIPSIZE;

	hit.pos = pos;
	//////////////////////////////
	//横方向
	{
		//左に移動中
		if(speed.x<0)
			if(get(middle,left))
			{
				hit.left  = true;
				hit.pos.x = left*MAP_CHIPSIZE + MAP_CHIPSIZE;
			}
		//右に移動中
		if(speed.x>0)
			if(get(middle,right))
			{
				hit.right = true;
				hit.pos.x = right*MAP_CHIPSIZE - size.x;
			}
	}
	////////////////////////////
	//縦方向
	{
		//上昇中
		if(speed.y>0)
			//左上、真上、右上いずれかに接触していた場合
			if(get(top,left) || get(top,center) || get(top,right))
			{
				hit.top   = true;
				hit.pos.y = top*MAP_CHIPSIZE + size.y;
			}
		//落下中
		if(speed.y<0)
			if(get(bottom,left) || get(bottom,center) || get(bottom,right))
			{
				hit.bottom = true;
				hit.pos.y  = bottom*MAP_CHIPSIZE - size.y;
			}
	}
	//当たり判定終わり
	///////////////////////////

	return hit;
}
/**
/縦方向
{
		//上昇中
		if(speed.y>0)
		{
			//マップチップの境界をまたいでいないなら
			if((int)hit.pos.x % MAP_CHIPSIZE==0)
			{
				//真上
				if(checkHit(center,top))
				{
					hit.top = true;
					hit.pos.y = top+MAP_CHIPSIZE;
				}
			}
			else
			{
				//左上、右上
				if(checkHit(left,top) || checkHit(right,top))
				{
				}
			}
		}
		//落下中
		if(speed.y<0)
		{
			//マップチップの境界をまたいでいないなら
			if((int)hit.pos.x % MAP_CHIPSIZE==0)
			{
				//真下
				if(checkHit(center,bottom))
			}
			else
			{
				//左下、右下
				if(checkHit(left,bottom) || checkHit(right,bottom))
				{
				}
			}
		}
}
/**/