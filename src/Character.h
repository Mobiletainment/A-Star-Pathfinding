#include "precomp.h"
#include "QuadTile.h"
#include <string>
#include "Direction.h"

using namespace std;


class Character
{
	private:

		static const float _frameSize;
		int _currentFrame;
		int _elapsedFrames;
		void CheckAnimation();
	protected:
		CL_GraphicContext _gc;
		int _startFrame;
		int _endFrame;
		float walkSpeed;
		QuadTile* currentTile;
		QuadTile* targetTile;
		CL_Sprite* sprite;
		CL_SpriteDescription* spriteDesc;
		Direction currentDirection;
		Direction nextDirection;
		void SetFrame(int frame);
		void SetFrames(int startFrame, int endFrame);
		virtual void SetCurrentDirection(Direction direction);
		void MoveInDirection(const Direction &direction);
	public:
		CL_Pointf* Position;
		Direction GetCurrentDirection();
		QuadTile *GetCurrentTile() const;
		Character::Character(CL_GraphicContext& gc, QuadTile* startingQuadTile, CL_SpriteDescription *spriteDesc);
		virtual void Update();
		virtual void Render();
		void Move(Direction direction);
		~Character(void);
	};
