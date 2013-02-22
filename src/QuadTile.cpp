#include "precomp.h"
#include "QuadTile.h"
#include "Field.h"

QuadTile::QuadTile(CL_Sprite *sprite, int x, int y) : sprite(sprite), posX(x), posY(y)
{
	left = NULL;
	right = NULL;
	top = NULL;
	bottom = NULL;
	topLeft = NULL;
	topRight = NULL;
	bottomLeft = NULL;
	bottomRight = NULL;
	neighbours = NULL;
	diagonalNeighbours = NULL;
	successor = NULL;
	totalWayCosts = 0;
	specialType = FieldTypeSpecialPositionNone;
	IsWalkable = true;
}

FieldType QuadTile::GetType()
{
	return type;
}

void QuadTile::SetType(FieldType fieldType)
{
	this->type = fieldType;
	int typeInt = GetIntByFieldType(fieldType);
	
	this->sprite->set_frame(typeInt-1);				
}

void QuadTile::SetType(FieldType fieldType, FieldTypeSpecial markSpecial)
{
	SetType(fieldType);
	this->specialType = markSpecial;
}

void QuadTile::SetPosition(int x, int y)
{
	this->posX = x;
	this->posY = y;
}

void QuadTile::Draw(CL_GraphicContext &gc)
{
	sprite->draw(gc, posX, posY);
}

CL_Sprite* QuadTile::GetSprite()
{
	return this->sprite;
}

void QuadTile::SetLeft(QuadTile* left)
{
	this->left = left;
	left->right = this;
}

void QuadTile::SetRight(QuadTile* right)
{
	this->right = right;
	right->left = this;
}

void QuadTile::SetTop(QuadTile* top)
{
	this->top = top;
	top->bottom = this;
}

void QuadTile::SetBottom(QuadTile* bottom)
{
	this->bottom = bottom;
	bottom->top= this;
}

void QuadTile::SetTopLeft(QuadTile* topLeft)
{
	this->topLeft = topLeft;
	topLeft->bottomRight = this;
}

void QuadTile::SetTopRight(QuadTile* topRight)
{
	this->topRight = topRight;
	topRight->bottomLeft = this;
}

void QuadTile::SetBottomLeft(QuadTile* bottomLeft)
{
	this->bottomLeft = bottomLeft;
	bottomLeft->topRight = this;
}

void QuadTile::SetBottomRight(QuadTile* bottomRight)
{
	this->bottomRight = bottomRight;
	bottomRight->topLeft = this;
}

vector<QuadTile *> QuadTile::GetNeighboursOfNeighbours()
{
	vector<QuadTile *> outerNeighbours(16);

	QuadTile *start = topLeft->GetNeighbor(TopLeft);

	outerNeighbours[0] = start;

	for(int i = 1; i <= 4; i++)
		outerNeighbours[i] = outerNeighbours[i-1]->GetNeighbor(Right);

	for (int i = 5; i <=8; i++)
		outerNeighbours[i] = outerNeighbours[i-1]->GetNeighbor(Bottom);

	for (int i = 9; i <=12; i++)
		outerNeighbours[i] = outerNeighbours[i-1]->GetNeighbor(Left);

	for (int i = 13; i <=15; i++)
		outerNeighbours[i] = outerNeighbours[i-1]->GetNeighbor(Top);

	return outerNeighbours;
}

vector<QuadTile *> &QuadTile::GetNeighbours()
{
	if (neighbours == NULL)
	{
		neighbours = new vector<QuadTile*>();	
	
		if (left != NULL)
			neighbours->push_back(left);
		if (top != NULL)
			neighbours->push_back(top);
		if (right != NULL)
			neighbours->push_back(right);
		if (bottom != NULL)
			neighbours->push_back(bottom);

		vector<QuadTile *> diagonals = GetDiagonalNeighbours();
		neighbours->insert(neighbours->end(), diagonals.begin(), diagonals.end());
	}

	return *neighbours;
}

vector<QuadTile *> &QuadTile::GetDiagonalNeighbours()
{
	if (diagonalNeighbours == NULL)
	{
		diagonalNeighbours = new vector<QuadTile*>();	

		if (topLeft != NULL)
			diagonalNeighbours->push_back(topLeft);
		if (topRight != NULL)
			diagonalNeighbours->push_back(topRight);
		if (bottomLeft != NULL)
			diagonalNeighbours->push_back(bottomLeft);
		if (bottomRight != NULL)
			diagonalNeighbours->push_back(bottomRight);
	}

	return *diagonalNeighbours;
}
	

QuadTile::~QuadTile(void)
{
}

void QuadTile::UpdateCostsAndSuccessor(QuadTile *target, QuadTile *successor)
{
	this->successor = successor;

	//H: Heuristik (distance calculation to start)
	float distanceToStart = Field::GetDiagonalDistanceBetweenTiles(target, this);

	totalWayCosts = DetermineWayCostsForSuccessor(successor);

	int influence = Field::Enemy->GetInfluenceValueForTile(this);

	totalCosts = totalWayCosts + distanceToStart + influence;
}

float QuadTile::DetermineWayCostsForSuccessor(QuadTile *successor)
{
	//for the way, we not only take the distance (direct neighbour = 1, diagonal neigbour = 1.4) into account,
	//but also the Type of this tile. The more impassible the type is, the higher the costs
	float wayCostFactor = this->GetWaypointDirectionFactor(successor);
	float wayCost = wayCostFactor * this->GetType();
	return wayCost + successor->GetWayCosts();
}

float QuadTile::LogCostsAndReturnTotal(QuadTile *successor)
{
	if (successor == NULL)
		return 0;

	char output[96];
	float wayCostFactor = this->GetWaypointDirectionFactor(successor);
	float wayCost = wayCostFactor * successor->GetType();
	int influence = Field::Enemy->GetInfluenceValueForTile(this);
	sprintf_s(output, "Direction Factor: %.2f, Type Factor: %d, Total: %.2f + %d Influence", wayCostFactor, successor->GetType(), wayCost, influence);
	CL_Console::write_line(output);
	return wayCost + influence;
}


float QuadTile::GetTotalCosts()
{
	return totalCosts;
}


float QuadTile::GetWaypointDirectionFactor(QuadTile* neighbour)
{
	float wayCostFactor = 1.0f;

	if (find(GetDiagonalNeighbours().begin(), GetDiagonalNeighbours().end(), neighbour) != GetDiagonalNeighbours().end())
		wayCostFactor = 1.41f; //diagonals have a length of sqrt(2)

	return wayCostFactor;
}

CL_Pointf QuadTile::GetCenterPoint()
{
	return CL_Pointf(posX + Field::TileLength/2, posY + Field::TileLength/2);
}

QuadTile* QuadTile::GetNeighbor(Direction direction)
{
	switch (direction)
	{
	case Direction::Left:
		return left;
	case Direction::Top:
		return top;
	case Direction::Right:
		return right;
	case Direction::Bottom:
		return bottom;
	case Direction::TopLeft:
		return topLeft;
	case Direction::TopRight:
		return topRight;
	case Direction::BottomLeft:
		return bottomLeft;
	case Direction::BottomRight:
		return bottomRight;
	default:
		return NULL;
	}
}