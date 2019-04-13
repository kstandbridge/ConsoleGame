#include <iostream>
#include <stack>
using namespace std;

#include "../ConsoleGameEngine/olcConsoleGameEngine.h"

class Maze : public olcConsoleGameEngine
{
public:
	Maze()
	{
		m_sAppName = L"MAZE";
	}

private:
	int m_nMazeWidth;
	int m_nMazeHeight;
	int* m_maze;

	enum
	{
		CELL_PATH_N = 0x01,
		CELL_PATH_E = 0x02,
		CELL_PATH_S = 0x04,
		CELL_PATH_W = 0x08,
		CELL_VISITED = 0x10,
	};

	int m_nVisitedCells;

	stack<pair<int, int>> m_stack;

	int m_nPathWidth;

protected:
	virtual bool OnUserCreate() override
	{
		// Maze parameters
		m_nMazeWidth = 40;
		m_nMazeHeight = 25;

		m_maze = new int[m_nMazeWidth * m_nMazeHeight];
		memset(m_maze, 0x00, m_nMazeWidth * m_nMazeHeight * sizeof(int));
		m_nPathWidth = 3;

		// Choose a starting cell
		int x = rand() % m_nMazeWidth;
		int y = rand() % m_nMazeHeight;
		m_stack.push(make_pair(x, y));
		m_maze[y * m_nMazeWidth + x] = CELL_VISITED;
		m_nVisitedCells = 1;

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) override
	{
		// Slow down for animation
		// this_thread::sleep_for(10ms);


		auto offset = [&](int x, int y)
		{
			return (m_stack.top().second + y) * m_nMazeWidth + (m_stack.top().first + x);
		};


		// Do Maze Algorithm
		if (m_nVisitedCells < m_nMazeWidth * m_nMazeHeight)
		{
			// Step 1: Create a set of the unvisited neighbors

			vector<int> neighbors;

			// North Neighbor
			if (m_stack.top().second > 0 && (m_maze[offset(0, -1)] & CELL_VISITED) == 0)
				neighbors.push_back(0);
			// East Neighbor
			if (m_stack.top().first < m_nMazeWidth - 1 && (m_maze[offset(1, 0)] & CELL_VISITED) == 0)
				neighbors.push_back(1);
			// South Neighbor
			if (m_stack.top().second < m_nMazeHeight - 1 && (m_maze[offset(0, 1)] & CELL_VISITED) == 0)
				neighbors.push_back(2);
			// West Neighbor
			if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
				neighbors.push_back(3);


			// Are there any neighbors available?
			if (!neighbors.empty())
			{
				// Choose one available neighbor at random
				int next_cell_dir = neighbors[rand() % neighbors.size()];

				// Create a path between the neighbor and the current cell
				switch (next_cell_dir)
				{
				case 0: // North
					m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
					m_maze[offset(0, 0)] |= CELL_PATH_N;
					m_stack.push(make_pair((m_stack.top().first + 0), (m_stack.top().second - 1)));
					break;

				case 1: // East
					m_maze[offset(1, 0)] |= CELL_VISITED | CELL_PATH_W;
					m_maze[offset(0, 0)] |= CELL_PATH_E;
					m_stack.push(make_pair((m_stack.top().first + 1), (m_stack.top().second + 0)));
					break;

				case 2: // South
					m_maze[offset(0, 1)] |= CELL_VISITED | CELL_PATH_N;
					m_maze[offset(0, 0)] |= CELL_PATH_S;
					m_stack.push(make_pair((m_stack.top().first + 0), (m_stack.top().second + 1)));

					break;

				case 3: // West
					m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
					m_maze[offset(0, 0)] |= CELL_PATH_W;
					m_stack.push(make_pair((m_stack.top().first - 1), (m_stack.top().second + 0)));
					break;

				}

				m_nVisitedCells++;
			}
			else
			{
				// No available neighbors so backtrack
				m_stack.pop();
			}
		}





		// === DRAWING STUFF ===

		// Clear Screen
		Fill(0, 0, m_nScreenWidth, m_nScreenHeight, L' ');

		// Draw Maze
		for (int x = 0; x < m_nMazeWidth; x++)
		{
			for (int y = 0; y < m_nMazeHeight; y++)
			{
				// Each cell is inflated by m_nPathWidth, so fill it in
				for (int py = 0; py < m_nPathWidth; py++)
					for (int px = 0; px < m_nPathWidth; px++)
					{
						if (m_maze[y * m_nMazeWidth + x] & CELL_VISITED)
							Draw(x * (m_nPathWidth + 1) + px, y * (m_nPathWidth + 1) + py, PIXEL_SOLID, FG_WHITE); // Draw Cell
						else
							Draw(x * (m_nPathWidth + 1) + px, y * (m_nPathWidth + 1) + py, PIXEL_SOLID, FG_BLUE); // Draw Cell
					}

				// Draw passageways between cells
				for (int p = 0; p < m_nPathWidth; p++)
				{
					if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_S)
						Draw(x * (m_nPathWidth + 1) + p, y * (m_nPathWidth + 1) + m_nPathWidth); // Draw South Passage

					if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_E)
						Draw(x * (m_nPathWidth + 1) + m_nPathWidth, y * (m_nPathWidth + 1) + p); // Draw East Passage
				}
			}
		}

		// Draw Unit - the top of the stack
		for( int py = 0; py < m_nPathWidth; py++)
			for(int px = 0; px < m_nPathWidth; px++)
				Draw(m_stack.top().first * (m_nPathWidth + 1) + px, m_stack.top().second * (m_nPathWidth + 1) + py, 0x2588, FG_GREEN); // Draw Cell

		return true;
	}
};


int main()
{
	// Use olcConsoleGameEngine derived app
	Maze game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();

	return 0;
}