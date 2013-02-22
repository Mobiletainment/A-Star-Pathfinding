#include "precomp.h"
#include "Pacman.h"
#include "Game.h"

Pacman::Pacman(CL_GraphicContext& gc, QuadTile* startingTile, CL_SpriteDescription* spriteDesc): Character(gc, startingTile, spriteDesc)
{
	SetFrames(3,4);
	
	highlyInfluencedTiles.resize(8);
	mediumInfluencedTiles.resize(16);

	
	for (int i = 0; i < 8; i++)
	{
		CL_Sprite spriteInfluenceHigh(gc, *spriteDesc);
		spriteInfluenceHigh.set_frame(1);
		highlyInfluencedTiles[i] = spriteInfluenceHigh;
	}

	for (int i = 0; i < 16; i++)
	{
		CL_Sprite spriteInfluenceMedium(gc, *spriteDesc);
		spriteInfluenceMedium.set_frame(2);
		mediumInfluencedTiles[i] = spriteInfluenceMedium;
	}

	mediumInfluencedNeighbours = startingTile->GetNeighboursOfNeighbours(); //cache middle influenced neighbours

	CL_Timer* updateTimer=new CL_Timer(); //Create Timer for periodic Game Loop call with 60FPS
	updateTimer->func_expired().set(this, &Pacman::Update);
	updateTimer->start((unsigned int)(1000.0f/60.0f), true);
}

void Pacman::Update()
{
	Character::Update();

	if (targetTile != NULL && targetTile != currentTile)
	{
		//don't do the movement if the neighbour-of-neighbours ring would run out of bounds
		if (targetTile->GetNeighbor(GetCurrentDirection())->GetNeighbor(GetCurrentDirection()) == NULL)
			return;

		//move to new tile
		CL_Pointf targetPosition(targetTile->GetPositionX(), targetTile->GetPositionY());

		if (targetPosition.x < Position->x) //Walk left
		{
			Position->x -= walkSpeed;
		}
		else if (targetPosition.x > Position->x) //Walk Right
		{
			Position->x += walkSpeed;
		}
		else if (targetPosition.y < Position->y) //Walk Up
		{
			Position->y -= walkSpeed;
		}
		else if (targetPosition.y > Position->y) //Walk Down
		{
			Position->y += walkSpeed;
		}

		if (targetPosition.x == Position->x && targetPosition.y == Position->y)
		{
			currentTile = targetTile;
		}

		mediumInfluencedNeighbours = currentTile->GetNeighboursOfNeighbours(); //cache middle influenced neighbours
		//trigger re-calculation of path
		Game::RecalculatePath();
	}
}

void Pacman::Render()
{
	Character::Render();
	//draw influenced tiles
	vector<QuadTile *> neighbours = currentTile->GetNeighbours();
	for (int i = 0; i < neighbours.size(); i++)
	{
		highlyInfluencedTiles[i].draw(_gc, neighbours[i]->GetPositionX(), neighbours[i]->GetPositionY());
	}

	for (int i = 0; i < mediumInfluencedNeighbours.size(); i++)
	{
		QuadTile* outerNeighbour = mediumInfluencedNeighbours[i];
		mediumInfluencedTiles[i].draw(_gc, outerNeighbour->GetPositionX(), outerNeighbour->GetPositionY());
	}

}

Pacman::~Pacman(void)
{
}

int Pacman::GetInfluenceValueForTile(QuadTile *tile)
{
	if (find(currentTile->GetNeighbours().begin(), currentTile->GetNeighbours().end(), tile) != currentTile->GetNeighbours().end())
		return 8;
	else if (find(mediumInfluencedNeighbours.begin(), mediumInfluencedNeighbours.end(), tile) != mediumInfluencedNeighbours.end())
		return 4;
	else if (this->currentTile == tile)
		return 16;
	else
		return 0;
}