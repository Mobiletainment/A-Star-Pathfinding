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
#include "Game.h"
#include <algorithm>
#include <list>
using namespace std;

CL_GraphicContext *Game::gc;
GameState Game::currentState;

Game::Game(CL_DisplayWindow &window)
{
	gc = &window.get_gc();
	board = &Field::GetInstance();
	currentState = GameStateSetStartPoint;
}

Game::~Game(){
}

void Game::Run(CL_DisplayWindow &window){

	game_display_window = window;

	*gc = window.get_gc();

	quit = false;

	
	CL_Slot slot_key_down = window.get_ic().get_keyboard().sig_key_down().connect(this, &Game::OnKeyDown);
	CL_Slot mousePress = window.get_ic().get_mouse().sig_key_down().connect(this, &Game::OnMouseDown);

	CL_InputDevice mouse = window.get_ic().get_mouse();


	//Set up font
	CL_FontDescription system_font_desc;
	system_font_desc.set_typeface_name("courier new");
	CL_Font_System system_font(*gc, system_font_desc);
	system_font.set_font_metrics(CL_FontMetrics(7.0f));

	window.flip(0);
	
	while (!quit)
	{	
		gc->clear(CL_Colorf(20, 56, 108));
		
		board->Render(*gc); 

		switch (currentState)
		{
		case GameStateSetStartPoint:
			board->MoveStartPoint(mouse.get_x(), mouse.get_y());
			break;
		case GameStateSetEndPoint:
			board->MoveEndPoint(mouse.get_x(), mouse.get_y());
			break;
		case GameStateStartAndEndPointsSet:
			board->ApplyStartAndEndPoint();
			currentState = GameStateCalculatePath;
			break;
		case GameStateCalculatePath:
			CalculatePath();
			currentState = GameStateDrawPath;
			break;
		case GameStateDrawPath:
			for(int i = 0; i < pathRoute.size() - 1; i++)
			{
				CL_Draw::line(*gc, pathRoute[i], pathRoute[i+1], CL_Colorf::red);
			}
			break;
		default:
			break;
		}

		window.flip(0);
		CL_KeepAlive::process();
	}	
}

void Game::CalculatePath()
{
	QuadTile *start = board->GetStartTile();
	QuadTile *target = board->GetEndTile();

	list<QuadTile *> toDoList;
	list<QuadTile *> finalizedList;
	pathRoute.clear();

	QuadTile *currentNode = target;
	
	toDoList.push_front(currentNode);

	//beginning from the target, search all nodes till we've got the start
	while(currentNode->GetSpecialType() != FieldTypeSpecialPositionStart)
	{
		finalizedList.push_front(currentNode);
		toDoList.remove(currentNode);

		//3. Füge Nachfolger (=angrenzende Knoten) in ToDo Liste ein
		vector<QuadTile *> neighbours = currentNode->GetNeighbours();

		for (int i = 0; i < neighbours.size(); i++)
		{
			QuadTile *successorNode = neighbours[i];

			//3.1 Constraint checking
			if (successorNode->IsWalkable) //3.1.1 only walkable nodes
			{
				if (!IsNodeInList(successorNode, finalizedList)) //3.1.2 keine Elemente der Finalized-Liste (würde neben Redundanz auch Zyklen verursachen)
				{
					//3.1.3. wenn der Knoten bereits in der ToDoListe enthalten ist, 
					//ersetze ihn wenn die aktuellen Wegkosten günstiger sind. (GListenelement > GAktuel)
					if (IsNodeInList(successorNode, toDoList))
					{
						//very important optimization: take the way with the least costs
						float existingWayCosts = successorNode->GetWayCosts();
						float currentWayCosts = successorNode->DetermineWayCostsForSuccessor(currentNode);
						
						if (currentWayCosts < existingWayCosts)
						{
							//the point in the ToDo-List can be reached with less costs,
							//therefore we replace it with this path
							toDoList.remove(successorNode);
						}
						else
						{
							//if the path can be reached with lower costs than with this path,
							//we don't add it to the ToDo List
							continue;
						}
					}

					//calculate Costs and update route
					successorNode->UpdateCostsAndSuccessor(start, currentNode);

					//3.2 aktueller Knoten wird als Vorgänger in den eingefügten Knoten gespeichert
					toDoList.push_front(successorNode);
				}
			}
		}

		if (toDoList.size() == 0)
			break; //there's no path

		//4. wähle Knoten mit günstigstem F als neuen aktuellen Knoten.
		//Sort ToDo List by costs, beginning with lowest
		//sort(toDoList.begin(), toDoList.end(), CompareTileCosts);
		toDoList.sort(CompareTileCosts);
		currentNode = toDoList.front();
	}

	//save points for drawing the route and log costs reverse (from start to point)
	QuadTile *traverseNode = currentNode;

	float totalCostsStartToEnd = 0;
	int tilesWalked = -1;
	do 
	{
		totalCostsStartToEnd += traverseNode->LogCostsAndReturnTotal(traverseNode->GetSuccessor());
		CL_Pointf point(traverseNode->GetCenterPoint());
		pathRoute.push_back(point);
		traverseNode = traverseNode->GetSuccessor();
		++tilesWalked;
	} while (traverseNode != NULL);
	
	char costs[64];
	sprintf_s(costs, "Total Costs: %.2f, Tiles walked: %d\n", totalCostsStartToEnd, tilesWalked);
	CL_Console::write_line(costs);
}

bool Game::CompareTileCosts(QuadTile *first, QuadTile *second)
{
	return first->GetTotalCosts() < second->GetTotalCosts();
}

bool Game::IsNodeInList(QuadTile *node, list<QuadTile*> &list)
{
	if (find(list.begin(), list.end(), node) != list.end())
		return true;

	return false;
}

void Game::OnKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	if (key.id == CL_KEY_ESCAPE) quit = true;
	
	//check Movement (WSAD or Arrows)
	if (key.id == CL_KEY_A || key.id ==  37) //Left = A or Arrow Left
		Field::Enemy->Move(Left);
	else if (key.id == CL_KEY_D || key.id == 39) //Right = D or Arrow right
		Field::Enemy->Move(Right);
	else if (key.id == CL_KEY_W || key.id == 38) //Up = W or Arrow Up
		Field::Enemy->Move(Top);
	else if (key.id == CL_KEY_S || key.id == 40) //Down = S or Arrow Down
		Field::Enemy->Move(Bottom);	
}

void Game::OnMouseDown(const CL_InputEvent &key, const CL_InputState &state)
{
	if (currentState < GameStateStartAndEndPointsSet)
		currentState = (GameState)((int)currentState + 1);
}

void Game::RecalculatePath()
{
	if (currentState > GameStateCalculatePath)
		currentState = GameStateCalculatePath;
}

void Game::Update()
{

}