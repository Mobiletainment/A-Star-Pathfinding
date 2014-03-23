A* Pathfinding Example
=============

What is it?
------------
[A*](http://en.wikipedia.org/wiki/Pathfinding#A.2A_Algorithm) is a pathfinding algorithm commonly used in video games. In this example, you choose the start and end point of the route first. The algorithm then finds the path with the least costs between these points, considering that each tile has different costs or may be even unwalkable. Furthermore, you're able to navigate Pacman accross the field. Paths that cross Pacman's near surroundings (i.e. within Pacman's influence radius) experience increased costs and may get more expensive than a longer way around Pacman. The algorithm therfore dynamically re-evaluates pathes that cross Pacman's influence radius and adapts the route as necessesary.

![Screenshot](/docs/Screenshot.png?raw=true)

How to play?
------------
* Begin by setting the start point. Move your mouse over the territory and click on the desired field. Note: black fields are considered unwalkable and can't be chosen as start or end point.
* Next, position the end point by clicking on the desired field
* The path with the lowest cost is calculated. The cost is determined by a heuristic considering the distance (horizontal or vertical moves count 1, diagonal moves 1.41), the cost multiplier of the field (1 = Street, 2 = Gras, 3 = Water) and the influence of the enemy (Pacman)
* Use the arrow keys to navigate Pacman through the scene. The cheapest path is recalculated immediately on every move. This is because paths passing through the surrounding of Pacman have increased costs and may therefore be more expensive than a long way round

Requirements
------------
What you need for compiling:
* Visual C++ or rather Visual Studio 2010 or above
* DirectX SDK June 2010 (http://www.microsoft.com/en-us/download/details.aspx?id=6812)
* ClanLib Game SDK (http://clanlib.org/wiki/Download)

How to compile
------------
1. Set up ClanLib by following these instructions: http://clanlib.org/wiki/MainDocs:Visual_C%2B%2B_on_Windows
2. Get the source code and open the project in Visual Studio
3. In the menu bar, click on VIEW and open the PROPERTY MANAGER
4. In the Property Manager Window, expand the tree and double click "Microsoft.CPP.Win32.user" under "Debug | Win32"
5. You will see the Property Pages Window. Click VC++ Directories
6. Edit the Include Directories and add the include directory of your DirectX SDK, e.g. "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include", as well as the include directory of ClanLib.
7. Click okay and edit the Library Directories. Add the Lib Directories of your DirectX SDK, e.g. "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86" and "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64"
8. Also add the lib\win32 Directory of ClanLib
9. Close the Property Pages Window and repeat steps 4-9 but with "Microsoft.CPP.Win32.user" under "Release | Win32"
10. Hit Build :)

License
------------
(The MIT License)

Copyright (c) 2012 David Pertiller <david@pertiller.net>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
