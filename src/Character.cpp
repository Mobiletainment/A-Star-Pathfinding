#include "precomp.h"
#include "Character.h"


const float Character::_frameSize=29.0f;

Character::Character(CL_GraphicContext& gc, QuadTile* startingQuadTile, CL_SpriteDescription *spriteDesc)
{
	_gc=gc;

	sprite = new CL_Sprite(gc, *spriteDesc);
	this->spriteDesc = spriteDesc;
	currentTile=startingQuadTile;
	targetTile = NULL;
	walkSpeed = 29.0f;
	currentDirection = Right;
	nextDirection = None;
	Position=new CL_Pointf(currentTile->GetPositionX(), currentTile->GetPositionY());
	
	this->sprite= sprite;

	_startFrame = NULL;
	_endFrame = NULL;
	_currentFrame = NULL;
	_elapsedFrames = NULL;
}

void Character::SetFrame(int frame)
{
	SetFrames(frame, frame);
}

void Character::SetFrames(int startFrame, int endFrame)
{
	_startFrame=startFrame;
	_endFrame=endFrame;
	_currentFrame=_startFrame;
}

void Character::Update()
{
	CheckAnimation();

	QuadTile *neighbour = currentTile->GetNeighbor(nextDirection);

	if (neighbour == NULL)
		return;

	if (nextDirection != None && neighbour->IsWalkable) //do movement in next direction if possible
	{
		targetTile = neighbour;
		SetCurrentDirection(nextDirection);
	}
	else if (neighbour->IsWalkable)
	{
		targetTile = neighbour;
	}
}

void Character::SetCurrentDirection(Direction direction)
{
	//First, inverse rotation of current Direction, so that Pacman is looking to right direction again
	switch (currentDirection)
	{
	case Left:
		this->sprite->rotate(CL_Angle::from_degrees(-180.0f));
		break;
	case Right:
		//this->_sprite->rotate(CL_Angle::from_degrees(0.0f));
		break;
	case Top:
		this->sprite->rotate(CL_Angle::from_degrees(-270.0f));
		break;
	case Bottom:
		this->sprite->rotate(CL_Angle::from_degrees(-90.0f));
		break;
	}

	//From here, rotate the sprite (source = right direction) so it's looking in the correct direction
	switch (direction)
	{
	case Left:
		this->sprite->rotate(CL_Angle::from_degrees(180.0f));
		break;
	case Right:
		//this->_sprite->rotate(CL_Angle::from_degrees(0.0f));
		break;
	case Top:
		this->sprite->rotate(CL_Angle::from_degrees(270.0f));
		break;
	case Bottom:
		this->sprite->rotate(CL_Angle::from_degrees(90.0f));
		break;
	}

	currentDirection = direction; //finally, set the direction the character is looking now
	nextDirection = None;
}

Direction Character::GetCurrentDirection()
{
	return currentDirection;
}

QuadTile* Character::GetCurrentTile() const
{
	return currentTile;
}

void Character::MoveInDirection(const Direction &direction)
{
	switch (direction)
	{
	case Left:
		Position->x -= walkSpeed;
		break;
	case Right:
		Position->x += walkSpeed;
		break;
	case Top:
		Position->y -= walkSpeed;
		break;
	case Bottom:
		Position->y += walkSpeed;
		break;
	}
}

void Character::CheckAnimation()
{
	_elapsedFrames++;

	if(_elapsedFrames>=10)
	{
		_elapsedFrames=0;
		_currentFrame++;

		if(_currentFrame>_endFrame)
			_currentFrame=_startFrame;
	}
}

void Character::Render()
{
	sprite->draw(_gc,CL_Rectf(_currentFrame*_frameSize,0,_currentFrame*_frameSize+_frameSize,_frameSize),CL_Rectf(Position->x, Position->y, Position->x  + 29, Position->y+ 29));
}

void Character::Move(Direction direction)
{
	nextDirection = direction;
	
}

Character::~Character(void)
{
}
