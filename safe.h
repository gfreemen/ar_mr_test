#pragma once

#include <iostream>
#include <vector>
#include <map>

using namespace std;


extern bool isValidInput(int row, int col, int mir, int bmir);

class TcSafe;

//
class TcPoint
{
	friend class TcSafe;
public:
	TcPoint(int x, int y) :
		d_x(x), d_y(y) {}
	TcPoint(const TcPoint& p) :
		d_x(p.d_x), d_y(p.d_y) {}
	~TcPoint() {}

	friend bool operator==(const TcPoint& a, const TcPoint& b);
	friend bool operator!=(const TcPoint& a, const TcPoint& b);
	friend bool operator<(const TcPoint& a, const TcPoint& b);

	void print()
	{
		cout << "(" << d_x << "/" << d_y << ")";
	}

private:
	int d_x;
	int d_y;
};

inline bool operator==(const TcPoint& a, const TcPoint& b)
{
	return (a.d_x == b.d_x && a.d_y == b.d_y);
}

inline bool operator!=(const TcPoint& a, const TcPoint& b)
{
	return (a.d_x != b.d_x || a.d_y != b.d_y);
}

inline bool operator<(const TcPoint& a, const TcPoint& b)
{
	return (a.d_x < b.d_x || (a.d_x == b.d_x && a.d_y < b.d_y));
}


enum MirrorOrientation {
	MirrorOrientationBack = -1,  // '\'
	MirrorOrientationDirect = 1, // '/'
};

enum BeamDirection {
	BeamDirectionUp = 0,
	BeamDirectionDown = 1,
	BeamDirectionLeft = 2,
	BeamDirectionRight = 3,
};

enum CheckResult {
	CheckResultImpossible = -1, // is not opened with or without inserting mirror 
	CheckResultNotSecures = 0,  // is opened without inserting mirror 
	CheckResultSecures = 1      //
};

//
class TcSafe
{
public:
	TcSafe(int row, int col, std::vector<TcPoint>& mirrors, std::vector<TcPoint>& back_mirrors);
	~TcSafe() {}

	int fullSecurityCheck(std::vector<TcPoint>& missing_mirrors);

private:
	CheckResult securityCheck();
	void move(); // just move to next position from current
	bool changeBeamDirection();
	void changeBeamDirection(MirrorOrientation orientation);
	bool outOfSafe();

	int d_row;
	int d_col;
	std::map<TcPoint, MirrorOrientation> d_mirrors;

	// safe coordinate system starts from (1, 1)
	TcPoint d_currentPos; // current position, init by (1, 0)
	TcPoint d_exit;

	BeamDirection d_beamDirection; // current beam direction

	std::vector<std::pair<TcPoint, BeamDirection> > d_savedPath;
};