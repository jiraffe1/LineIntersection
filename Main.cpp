//Thanks to javidx9 (absolute legend) for olcPixelGameEngine!

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

//Returns if two lines don't intersect
const olc::vi2d NO_INTERSECTION = olc::vi2d(INFINITY, INFINITY);

//Line coordinates
olc::vi2d la1(50, 50);
olc::vi2d la2(50, 150);

olc::vi2d lb1(10, 100);
olc::vi2d lb2(140, 100);

//These functions are probably already defined

float max(float a, float b)
{
	return a > b ? a : b;
}

float min(float a, float b)
{
	return a < b ? a : b;
}

double dist(float x1, float y1, float x2, float y2) 
{
	//Pythagorean distance
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool roughlyEquals(float a, float b)
{
	//Used to adjust for number type conversion inaccuracy
	return (max(a, b) - min(a, b)) < 0.001f;
}

// Override base class with your custom functionality
class Canvas : public olc::PixelGameEngine
{
public:
	Canvas()
	{
		// Name your application
		sAppName = "Line Intersection";
	}
	
public:
	bool OnUserCreate() override
	{	
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame

		Clear(olc::BLACK);

		DrawLine(la1.x, la1.y, la2.x, la2.y, olc::RED);
		DrawLine(lb1.x, lb1.y, lb2.x, lb2.y, olc::BLUE);
		
		olc::vi2d intersectionPoint = Intersect(la1, la2, lb1, lb2);
		
		if (intersectionPoint != NO_INTERSECTION)
		{
			//if there is an intersection, draw it
			DrawCircle(intersectionPoint.x, intersectionPoint.y, 5);
		}

		int mouseX = GetMouseX();
		int mouseY = GetMouseY();
		bool mouseDown = GetMouse(0).bHeld;

		if (dist(mouseX, mouseY, la1.x, la1.y) < 10 && mouseDown)
		{
			la1.x = mouseX;
			la1.y = mouseY;
		}
		else if (dist(mouseX, mouseY, la2.x, la2.y) < 10 && mouseDown)
		{
			la2.x = mouseX;
			la2.y = mouseY;
		}
		else if (dist(mouseX, mouseY, lb1.x, lb1.y) < 10 && mouseDown)
		{
			lb1.x = mouseX;
			lb1.y = mouseY;
		}
		else if (dist(mouseX, mouseY, lb2.x, lb2.y) < 10 && mouseDown)
		{
			lb2.x = mouseX;
			lb2.y = mouseY;
		}
		return true;
	}

	

	bool pointOnLine(float x, float y, float m, float c)
	{
		//Is the point (x, y) on the line y = mx + c?
		//Basically check if y = mx + c
		//I don't think this is actually necessary
		return roughlyEquals(y ,m * x + c);
	}

	olc::vi2d Intersect(olc::vi2d a1, olc::vi2d a2, olc::vi2d b1, olc::vi2d b2)
	{
		const float extremelySmallValue = 0.0001f;

		//Calculate where lines a and b intersect, return NO_INTERSECTION if they do not intersect
		//ALSO for lines that are exactly perpendicular it throws a hissy fit at the moment so we add a bit extra so there are no infinities and NANs
		//First we need to find each line's formula (y = mx + c)
		/*
		am (gradient of a)
		ac (y-intercept of a)
		bm (gradient of b)
		bc (y-intercept of b)
		*/

		//I don't think casting them all to floats does anything
		float am = ((float)a2.y + extremelySmallValue - (float)a1.y + extremelySmallValue) / ((float)a2.x + extremelySmallValue - (float)a1.x + extremelySmallValue);
		float bm = ((float)b2.y + extremelySmallValue - (float)b1.y + extremelySmallValue) / ((float)b2.x + extremelySmallValue - (float)b1.x + extremelySmallValue);

		//Now to find the y-intercepts
		float ac = a1.y - a1.x * am;
		float bc = b1.y - b1.x * bm;

		//solve am+ac = bm+bc as long as they are not parallel

		if (am != bm)
		{
			//left side xs
			float lsx = am;
			//left side numbers
			float lsn = ac;
			//right side xs
			float rsx = bm;
			//right side numbers
			float rsn = bc;

			//move lsn to rsn
			rsn -= lsn;
			lsn = 0;
			//move rsx to lsn
			lsx -= rsx;
			rsx = 0;
			//divide both sides by lsx
			rsn = rsn / lsx;
			lsx = 1;

			//answer is now in rsn
			float rx = rsn;
			float ry = rsn * am + ac;
	
			bool wba = rx > min(a1.x, a2.x) && rx < max(a1.x, a2.x) || roughlyEquals(rx, (a1.x + a2.x)/2);
			bool wbb = rx > min(b1.x, b2.x) && rx < max(b1.x, b2.x) || roughlyEquals(rx, (b1.x + b2.x) / 2);

			if (pointOnLine(rx, ry, am, ac) && pointOnLine(rx, ry, bm, bc) && wba && wbb)
			{
				//Make sure the intersection is actually on both of the lines
				return olc::vi2d((int)rx, (int)ry);
			}
			else 
			{
				return NO_INTERSECTION;
			}
		}
		else 
		{
			return NO_INTERSECTION;
		}
	}
};



int main()
{
	Canvas window;
	if (window.Construct(200, 200, 4, 4)) 
	{
		window.Start();
	}
		
	return 0;
}
