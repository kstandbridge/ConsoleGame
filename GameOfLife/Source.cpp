#include <iostream>
using namespace std;

#include "../ConsoleGameEngine/olcConsoleGameEngine.h"

class GameOfLife : public olcConsoleGameEngine
{
public:
	GameOfLife()
	{
		m_sAppName = L"Game Of Life";
	}

	~GameOfLife()
	{
		if(m_output)
		{
			delete[] m_output;
			m_output = nullptr;
		}

		if(m_state)
		{
			delete[] m_state;
			m_state = nullptr;
		}
	}

private:
	int* m_output;
	int* m_state;

protected:
	// Called by olcConsoleGameEngine
	virtual bool OnUserCreate() override
	{
		m_output = new int[ScreenWidth() * ScreenHeight()];
		m_state = new int[ScreenWidth() * ScreenHeight()];
		memset(m_output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
		memset(m_state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

		auto set = [&](int x, int y, wstring s)
		{
			int p = 0;
			for(auto c : s)
			{
				m_state[y*ScreenWidth() + x + p] = c == L'#' ? 1 : 0;
				p++;
			}
		};

		// Random start
		for (int i = 0; i < ScreenWidth()*ScreenHeight(); i++)
			m_state[i] = rand() % 2;

		// R-Pentonimo
		// set(80, 50, L"..##.");
		// set(80, 51, L".##..");
		// set(80, 52, L"..#..");

		// Gosper Glider Gun
		// set(60, 45, L"........................#............");
		// set(60, 46, L"......................#.#............");
		// set(60, 47, L"............##......##............##.");
		// set(60, 48, L"...........#...#....##............##.");
		// set(60, 49, L"##........#.....#...##...............");
		// set(60, 50, L"##........#...#.##....#.#............");
		// set(60, 51, L"..........#.....#.......#............");
		// set(60, 52, L"...........#...#.....................");
		// set(60, 53, L"............##.......................");

		// Infinite Growth
		// set(20, 50, L"########.#####...###......#######.#####");

		return true;
	}

	// Called by olcConsoleGameEngine
	virtual bool OnUserUpdate(float fElapsedTime) override
	{
		// this_thread::sleep_for(50ms);

		auto cell = [&](int x, int y)
		{
			return m_output[y * ScreenWidth() + x];
		};

		
		// Store output state
		for (int i = 0; i < ScreenWidth()*ScreenHeight(); i++)
			m_output[i] = m_state[i];

		// Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
		// Any live cell with two or three live neighbors lives on to the next generation.
		// Any live cell with more than three live neighbors dies, as if by overpopulation.
		// Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
		// B3S23 / Born if exactly 3 neighbors, survives if 2 or 3 living neighbors, dies otherwise

		for (int x = 1; x < ScreenWidth() - 1; x++)
			for (int y = 1; y < ScreenHeight() - 1; y++)
			{
				// Count the neighbors
				int nNeighbors = cell(x - 1, y - 1) + cell(x - 0, y - 1) + cell(x + 1, y - 1) +
								 cell(x - 1, y + 0) +            0           + cell(x + 1, y + 0) +
								 cell(x - 1, y + 1) + cell(x + 0, y + 1) + cell(x + 1, y + 1);

				if(cell(x,y) == 1) // Living cell
					m_state[y*ScreenWidth() + x] = nNeighbors == 2 || nNeighbors == 3; // 2 or 3 neighbors, you live!
				else // Dead cell
					m_state[y*ScreenWidth() + x] = nNeighbors == 3; // Exactly 3 neighbors? You're born again/reproduction

				if(cell(x, y) == 1) // Living cell
					Draw(x, y, PIXEL_SOLID, FG_WHITE);
				else // Dead cell
					Draw(x, y, PIXEL_SOLID, FG_BLACK);
			}

		return true;
	}
};

int main()
{
	// Use olcConsoleGameEngine derived app
	GameOfLife game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();

	return 0;
}