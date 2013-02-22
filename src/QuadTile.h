#pragma once
#include "FieldType.h"
#include "Direction.h"
#include <vector>

using namespace std;

class QuadTile
{
private:
	FieldType type;
	FieldTypeSpecial specialType;
	CL_Sprite *sprite;
	int posX;
	int posY;
	QuadTile *left;
	QuadTile *right;
	QuadTile *top;
	QuadTile *topLeft;
	QuadTile *topRight;
	QuadTile *bottom;
	QuadTile *bottomLeft;
	QuadTile *bottomRight;
	vector<QuadTile *> *neighbours;
	vector<QuadTile *> *diagonalNeighbours;

	//cost calculation
	QuadTile *successor;
	float totalWayCosts;
	float totalCosts;
public:
	QuadTile(CL_Sprite *sprite, int posX, int posY);
	~QuadTile(void);
	void SetType(FieldType type);
	void SetType(FieldType fieldType, FieldTypeSpecial markSpecial);
	FieldType GetType();
	FieldTypeSpecial GetSpecialType(){ return specialType;};
	void Draw(CL_GraphicContext &gc);
	CL_Sprite* GetSprite();
	void SetPosition(int x, int y);
	int GetPositionX() { return posX; };
	int GetPositionY() { return posY; };
	CL_Pointf GetCenterPoint();
	
	//neighbour handling
	void SetLeft(QuadTile* left);
	void SetRight(QuadTile* right);
	void SetTop(QuadTile* top);
	void SetBottom(QuadTile* bottom);
	void SetTopLeft(QuadTile *topLeft);
	void SetTopRight(QuadTile *topRight);
	void SetBottomLeft(QuadTile *bottomLeft);
	void SetBottomRight(QuadTile *bottomRight);
	vector<QuadTile *> &GetNeighbours();
	vector<QuadTile *> &GetDiagonalNeighbours();

	vector<QuadTile *> GetNeighboursOfNeighbours();
	QuadTile* GetNeighbor(Direction direction);
	bool IsWalkable;
	void UpdateCostsAndSuccessor(QuadTile *target, QuadTile *successor); //F: F=G+H
	float GetTotalCosts();
	float GetWayCosts() { return totalWayCosts; };
	float DetermineWayCostsForSuccessor(QuadTile *successor);
	float LogCostsAndReturnTotal(QuadTile *successor);
	QuadTile *GetSuccessor() { return successor; };
	float GetWaypointDirectionFactor(QuadTile* neighbour);
};

