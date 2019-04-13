#include <iostream>
#include <string>
using namespace std;

#include "../ConsoleGameEngine/olcConsoleGameEngine.h"

struct sPoint2D
{
	float x;
	float y;
};

struct sSpline
{
	vector<sPoint2D> points;

	sPoint2D GetSplinePoint(float t, bool bLooped = false)
	{
		int p0, p1, p2, p3;
		if (!bLooped)
		{
			p1 = (int)t + 1; // floor(t) + 1
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -ttt + 2.0f * tt - t;
		float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
		float q3 = -3.0f * ttt + 4.0f * tt + t;
		float q4 = ttt - tt;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return { tx,ty };
	}

	sPoint2D GetSplineGradient(float t, bool bLooped = false)
	{
		int p0, p1, p2, p3;
		if (!bLooped)
		{
			p1 = (int)t + 1; // floor(t) + 1
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -3.0f * tt + 4.0f * t - 1;
		float q2 = 9.0f * tt - 10.0f * t;
		float q3 = -9.0f * tt + 8.0f * t + 1.0f;
		float q4 = 3.0f * tt - 2.0f * t;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return { tx,ty };
	}
};

class Splines : public olcConsoleGameEngine
{
public:
	Splines()
	{
		m_sAppName = L"Splines";
	}

private:
	sSpline path;
	int nSelectedPoint = 0;
	float fMarker = 0.0f;

protected:
	// Called by olcConsoleGameEngine
	virtual bool OnUserCreate() override
	{
		// path.points = { {10, 41 }, { 40, 41 }, {70, 41 }, { 100, 41 } };
		path.points = { { 10, 41 }, { 20, 41 }, { 30, 41 }, { 40, 41 }, { 50, 41 }, { 60, 41 }, { 70, 41 }, { 80, 41 }, { 90, 41 }, { 100, 41 }, };


		return true;
	}

	// Called by olcConsoleGameEngine
	virtual bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear Screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

		// Handle input
		if (m_keys[L'X'].bReleased)
		{
			nSelectedPoint++;
			if (nSelectedPoint >= path.points.size())
				nSelectedPoint = 0;
		}

		if (m_keys[L'Z'].bReleased)
		{
			nSelectedPoint--;
			if (nSelectedPoint < 0)
				nSelectedPoint = path.points.size() - 1;
		}

		if (m_keys[VK_LEFT].bHeld)
			path.points[nSelectedPoint].x -= 30.0f * fElapsedTime;

		if (m_keys[VK_RIGHT].bHeld)
			path.points[nSelectedPoint].x += 30.0f * fElapsedTime;

		if (m_keys[VK_UP].bHeld)
			path.points[nSelectedPoint].y -= 30.0f * fElapsedTime;

		if (m_keys[VK_DOWN].bHeld)
			path.points[nSelectedPoint].y += 30.0f * fElapsedTime;

		if (m_keys[L'A'].bHeld)
			fMarker -= 5.0f * fElapsedTime;

		if (m_keys[L'S'].bHeld)
			fMarker += 5.0f * fElapsedTime;

		if(fMarker >= (float)path.points.size())
			fMarker -= (float)path.points.size();

		if(fMarker < 0.0f)
			fMarker += (float)path.points.size();

		// Draw Spline
		for (float t = 0; t < (float)path.points.size(); t += 0.005f)
		{
			sPoint2D pos = path.GetSplinePoint(t, true);
			Draw(pos.x, pos.y);
		}


		// Draw Control Points
		for (int i = 0; i < path.points.size(); i++)
		{
			Fill(path.points[i].x - 1, path.points[i].y - 1, path.points[i].x + 2, path.points[i].y + 2, PIXEL_SOLID, FG_RED);
			DrawString(path.points[i].x, path.points[i].y, to_wstring(i));
		}

		// Highlight control point
		Fill(path.points[nSelectedPoint].x - 1, path.points[nSelectedPoint].y - 1, path.points[nSelectedPoint].x + 2, path.points[nSelectedPoint].y + 2, PIXEL_SOLID, FG_YELLOW);
		DrawString(path.points[nSelectedPoint].x, path.points[nSelectedPoint].y, to_wstring(nSelectedPoint));

		// Draw agent to demonstrate gradient
		sPoint2D p1 = path.GetSplinePoint(fMarker, true);
		sPoint2D g1 = path.GetSplineGradient(fMarker, true);
		float r = atan2(-g1.y, g1.x);
		DrawLine(5.0f * sin(r) + p1.x, 5.0f * cos(r) + p1.y, -5.0f * sin(r) + p1.x, -5.0f * cos(r) + p1.y, PIXEL_SOLID, FG_BLUE);

		return true;
	}
};

int main()
{
	Splines game;
	game.ConstructConsole(160, 80, 10, 10);
	game.Start();
	return 0;
}