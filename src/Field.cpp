#include "precomp.h"
#include "Field.h"
#include "Game.h"

using namespace std;

Field* Field::instance = 0;
Pacman* Field::Enemy = 0;

//Map Initialization
Field::Field(CL_GraphicContext &gc) : width(54), height(33), gc(gc)
{	 
	Data = vector<vector<char>>(height,vector<char>(width));
	tiles.resize(height, vector<QuadTile *>(width));

	tileSpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/field_road.png"));
	tileSpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/field_gras.png")); 
	tileSpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/field_water.png"));
	tileSpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/field_unwalkable.png"));

	overlaySpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/point_start.png"));
	overlaySpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/point_target.png"));
	
	enemySpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/pacman.png"));
	enemySpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/field_enemyinfluence_high.png"));
	enemySpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/field_enemyinfluence_middle.png"));


	static int tileOffsetY = 0;
	int tilePosY = tileOffsetY;	

	//initialize the tiles
	for(int row = 0; row < height; ++row)
	{
		for(int column = 0; column < width; ++column)
		{
			int tilePosX = column * Field::GetTileWidth();

			QuadTile *tile = new QuadTile(new CL_Sprite(gc, tileSpriteDesc), tilePosX, tilePosY);
			tile->SetType(GetFieldTypeByInt((rand() % 4) + 1));
			
			//TODO:
			if(tile->GetType() == FieldTypeUnwalkable)
				tile->IsWalkable = false; 

			//assign neighbours
			if (column > 0)
				tile->SetLeft(tiles[row][column-1]);

			if (row > 0)
				tile->SetTop(tiles[row-1][column]);

			if (row > 0 && column > 0)
				tile->SetTopLeft(tiles[row-1][column-1]);

			if (row > 0 && column < width-1)
				tile->SetTopRight(tiles[row-1][column+1]);

			tiles[row][column] = tile; //add tile to array
		}

		tilePosY += Field::GetTileHeight();
	}

	startTileMarker = new QuadTile(new CL_Sprite(gc, overlaySpriteDesc), 0, 0);
	startTileMarker->GetSprite()->set_frame(0);
	
	endTileMarker = nullptr;

	//place enemy on a walkable tile
	QuadTile *enemyTile = tiles[height/2][width/2];
	enemyTile->SetType(FieldTypeRoad);
	enemyTile->IsWalkable = true;
	Enemy = new Pacman(gc, enemyTile, &enemySpriteDesc);

}

Field &Field::GetInstance() //Singleton
{
	if (!instance)
		instance =  new Field(*Game::gc);
	return *instance;
}

Field::~Field()
{
}

void Field::Render(CL_GraphicContext &gc)
{
	for(int x = 0; x < height; ++x)
	{	 
		for(int y = 0; y < width; ++y)
		{
			tiles[x][y]->Draw(gc);			
		}
	}

	Enemy->Render();

	startTileMarker->Draw(gc);

	if (endTileMarker != NULL)
		endTileMarker->Draw(gc);
}

void Field::MoveStartPoint(int x, int y)
{
	SetPositionOfSpecialTileToAlignWithFieldTile(startTileMarker, x, y);
}

void Field::MoveEndPoint(int x, int y)
{
	if (endTileMarker == NULL)
	{	
		endTileMarker = new QuadTile(new CL_Sprite(gc, overlaySpriteDesc), x, y);
		endTileMarker->GetSprite()->set_frame(1);
	}

	SetPositionOfSpecialTileToAlignWithFieldTile(endTileMarker, x, y);
}

void Field::ApplyStartAndEndPoint()
{
	//fixate start tile
	startTile = GetTileForPosition(startTileMarker->GetPositionX(), startTileMarker->GetPositionY());
	startTile->SetType(startTile->GetType(), FieldTypeSpecialPositionStart);

	//fixate end tile
	endTile = GetTileForPosition(endTileMarker->GetPositionX(), endTileMarker->GetPositionY());
	endTile->SetType(endTile->GetType(), FieldTypeSpecialPositionTarget);
}



void Field::SetPositionOfSpecialTileToAlignWithFieldTile(QuadTile *tile, int &x, int &y)
{
	int indexX = x / TileLength;
	int indexY = y / TileLength;

	if (indexX >= 0 && indexY >= 0 && indexX < width && indexY < height)
	{
		QuadTile *fieldTile = tiles[indexY][indexX];
		if (fieldTile->GetType() != FieldTypeUnwalkable)
			tile->SetPosition(fieldTile->GetPositionX(), fieldTile->GetPositionY());
	}
}

QuadTile* Field::GetTileForPosition(int x, int y)
{
	int indexX = x / TileLength;
	int indexY = y / TileLength;

	return tiles[indexY][indexX];
}

//heuristic: calculate distance by using diagonal as well as direct movement (diagonal preferred)
float Field::GetDiagonalDistanceBetweenTiles(QuadTile *one, QuadTile *other)
{
	int distanceX = one->GetPositionX() - other->GetPositionX();
	distanceX /= TileLength;

	int distanceY = one->GetPositionY() - other->GetPositionY();
	distanceY /= TileLength;

	if (distanceX < 0)
		distanceX *= -1;

	if (distanceY < 0)
		distanceY *= -1;

	int diagonalWalkCount;
	int directWalkCount;

	if (distanceX > distanceY)
	{
		diagonalWalkCount = distanceY;
		directWalkCount = distanceX - diagonalWalkCount;
	}
	else
	{
		diagonalWalkCount = distanceX;
		directWalkCount = distanceY - diagonalWalkCount;
	}

	return 1.41f * diagonalWalkCount + 1.0f * directWalkCount;
}