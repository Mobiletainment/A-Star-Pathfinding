
#ifndef header_map
#define header_map

#include "QuadTile.h"
#include "Pacman.h"

using namespace std;

class Field
{
public:
	static Field &GetInstance(); //Singleton
	static Pacman *Enemy;
	int GetWidth() { return width; }
	int GetHeight() { return height; }	 
	static const int TileLength = 29;
	int GetTileWidth() { return TileLength; } 
	int GetTileHeight() { return TileLength; }
	int Turn;


	vector<vector<char>> Data; //the actual field
	void Render(CL_GraphicContext &gc);
	void MoveStartPoint(int x, int y);
	void MoveEndPoint(int x, int y);
	void ApplyStartAndEndPoint();
	~Field();
	QuadTile *GetStartTile() { return startTile; };
	QuadTile *GetEndTile() { return endTile; };
	static float GetDiagonalDistanceBetweenTiles(QuadTile *one, QuadTile *other);
private:
	Field( CL_GraphicContext &gc); //private constructor for singleton
	static Field *instance;
	vector<vector<QuadTile *>> tiles;
	CL_SpriteDescription tileSpriteDesc;
	CL_SpriteDescription overlaySpriteDesc;
	CL_SpriteDescription enemySpriteDesc;

	CL_GraphicContext &gc;
	QuadTile *startTile;
	QuadTile *endTile;
	QuadTile *startTileMarker;
	QuadTile *endTileMarker;
	void SetPositionOfSpecialTileToAlignWithFieldTile(QuadTile *tile, int &x, int &y);
	QuadTile *GetTileForPosition(int x, int y);
	int width, height;

};

#endif