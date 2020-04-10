#pragma once

//マップのデータ管理
class _mapData
{
private:
	Grid<int> map;
	Vec2 playersp;

public:
	Vec2 scr;
	_mapData() { init(0, 0); }
	void init(int i,int j){ map = Grid<int>(i, j, 0); }

	//get
	void CheckHitState();
	inline  int width() { return map.width(); }
	inline  int height() { return map.height(); }
	inline  int size() { return map.size_bytes(); }
	inline Vec2 getPlayerSpeed() { return playersp; }
	inline int get(int y, int x)
	{
		if ( (y<0||y>map.height()) || (x<0 || x>map.width()) )
			return 0;
		return map[y][x];
	}
	//set
	void setPlayerSpeed(Vec2 a) { playersp = a; }
	void set(int y, int x, int n)
	{
		if (y<0 || y>map.height() || x<0 || x>map.width())
			return;
		map[y][x] = n;
	}
};

class _player
{
	private:
		_mapData* map;
		Texture pic;
		Vec2 pos, speed;
		RectF hitBox;

		//ジャンプ
		bool wasSpacePressed;
		int jumpCnt, spacePressedFrame;

		//ダッシュ
		int dashingTime;
		int dashSp; //1→右、-1→左
		bool dashing;

	public:
		_player() { init(NULL);}
		void init(_mapData*);
		void update();
		void draw();

		void jump();
		void dash();
		void move();
};

///////////////////////////////////
// シーン

class _gameMain
{
private:
	_player player;

	_mapData map;
	RenderTexture mapTexture;
public:
	_gameMain() { init(); }
	void init();
	void update();
	void draw();

	void mapDraw();
};