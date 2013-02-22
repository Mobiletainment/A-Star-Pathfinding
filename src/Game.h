/*
 * Connect Four - with Alpha Beta Search Algorithm
 * Copyright (C) 2012 David Pertiller <http://pertiller.me>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "precomp.h"
#include "Field.h"
#include <functional>
#include <list>

enum GameState
{
	GameStateSetStartPoint = 0,
	GameStateSetEndPoint = 1,
	GameStateStartAndEndPointsSet = 2,
	GameStateCalculatePath = 3,
	GameStateDrawPath = 4
};

class Game
{ 
private:
	void OnKeyDown(const CL_InputEvent &key, const CL_InputState &state);
	void OnMouseDown(const CL_InputEvent &key, const CL_InputState &state);
	void Update();
	Field *board;
	bool quit;
	CL_DisplayWindow game_display_window;
	void CalculatePath();
	bool IsNodeInList(QuadTile *node, list<QuadTile*> &list);
	static bool CompareTileCosts(QuadTile *first, QuadTile *second);
	vector<CL_Pointf> pathRoute;
	static GameState currentState;
public:
	static CL_GraphicContext *gc;
	Game( CL_DisplayWindow &window);
	~Game();
	void Run(CL_DisplayWindow &window);
	void OnQuit() { quit = true; }
	static void RecalculatePath();
};
