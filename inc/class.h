#pragma once

//マップのデータ管理
class _mapData
{
private:
	Grid<int> map;
	RenderTexture mapTexture;
	RenderTexture origin;

public:
	Vec2 scr;
	_mapData() { init(); }
	void init();
	void update();
	void draw();

	//get
	inline int get(int, int);
	inline int width() { return map.width(); }
	inline int height() { return map.height(); }
	inline int size() { return map.size_bytes(); }
	inline int textureWidth() { return mapTexture.width(); }

	//set
	void set(int, int, int);

	//当たり判定
	bool checkHit(int,int);
	_mapHitState checkMapHitState(Vec2, Vec2, Rect);
};

class _player
{
	private:
		_mapData* map;
		Texture pic;
		Vec2 pos, speed;
		RectF hitBox;
		String debug;

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

	//void mapDraw();
};