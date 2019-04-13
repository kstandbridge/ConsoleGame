#include <iostream>
using namespace std;

#include "olcConsoleGameEngine.h"

class Demo : public olcConsoleGameEngine
{
public:
	Demo()
	{
		
	}


	bool OnUserCreate() override
	{
		m_fPlayerX = 10.0f;
		m_fPlayerY = 10.0f;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if(m_keys[VK_LEFT].bHeld) 
			m_fPlayerX -= 15.0f * fElapsedTime;
		if(m_keys[VK_RIGHT].bHeld) 
			m_fPlayerX += 15.0f * fElapsedTime;
		if(m_keys[VK_UP].bHeld) 
			m_fPlayerY -= 15.0f * fElapsedTime;
		if(m_keys[VK_DOWN].bHeld)
			m_fPlayerY += 15.0f * fElapsedTime;

		Fill(0, 0, m_nScreenWidth, m_nScreenHeight, L' ', 0);

		Fill((int)m_fPlayerX, (int)m_fPlayerY, (int)m_fPlayerX + 5, (int)m_fPlayerY + 5);

		return true;
	}

private :
	float m_fPlayerX;
	float m_fPlayerY;
};

int main()
{
	Demo game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();

	return 0;
}