#pragma once
#include "Character.h"
#include "Direction.h"
#include <list>
using namespace std;


class Pacman:public Character
{
protected:
		vector<CL_Sprite> highlyInfluencedTiles;
		vector<CL_Sprite> mediumInfluencedTiles;
		vector<QuadTile *>mediumInfluencedNeighbours;

public:
	Pacman(CL_GraphicContext& gc, QuadTile* startingTile, CL_SpriteDescription *spriteDesc);
	int GetInfluenceValueForTile(QuadTile *tile);
	void Update();
	void Render();
	~Pacman(void);
};
