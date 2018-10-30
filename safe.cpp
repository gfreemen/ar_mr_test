#include "safe.h"
#include <iostream>


const int MIN_ROW_COL = 1;
const int MAX_ROW_COL = 1000000;
const int MIN_MIRROR = 0;
const int MAX_MIRROR = 200000;


bool isValidInput(int row, int col, int mir, int bmir)
{
	if (row < MIN_ROW_COL || row > MAX_ROW_COL || col < MIN_ROW_COL || col > MAX_ROW_COL
		|| mir < MIN_MIRROR || mir > MAX_MIRROR || bmir < MIN_MIRROR || bmir > MAX_MIRROR)
		return false;
	return true;
}


TcSafe::TcSafe(int row, int col, std::vector<TcPoint>& mirrors, std::vector<TcPoint>& back_mirrors)
	:d_row(row), d_col(col), d_currentPos(1, 0), d_exit(row, col + 1), d_beamDirection(BeamDirectionRight)
{
	for (std::vector<TcPoint>::iterator it = mirrors.begin(); it != mirrors.end(); ++it)
	{
		d_mirrors[*it] = MirrorOrientationDirect;
	}
	for (std::vector<TcPoint>::iterator it = back_mirrors.begin(); it != back_mirrors.end(); ++it)
	{
		d_mirrors[*it] = MirrorOrientationBack;
	}
}


int TcSafe::fullSecurityCheck(std::vector<TcPoint>& missing_mirrors)
{
	int result = CheckResultSecures;
	while (1)
	{
		move();

		if (outOfSafe())
		{
			if (d_currentPos == d_exit) result = CheckResultNotSecures;
			else result = CheckResultImpossible;
			break;
		}
		else
		{
			if (!changeBeamDirection())
			{
				// save current position in vec if it inside safe and mirror isn't here
				d_savedPath.push_back(std::pair<TcPoint, BeamDirection>(d_currentPos, d_beamDirection));
			}
		}
	}

	// try to find all possible missing mirrors cases
	if (result == CheckResultImpossible && d_savedPath.size())
	{
		std::vector<MirrorOrientation> mirrorOrientations;
		mirrorOrientations.push_back(MirrorOrientationDirect);
		mirrorOrientations.push_back(MirrorOrientationBack);

		for (std::vector<std::pair<TcPoint, BeamDirection> >::reverse_iterator it = d_savedPath.rbegin();
			it != d_savedPath.rend(); ++it)
		{
			TcPoint savedPos = d_currentPos = it->first;
			d_beamDirection = it->second;

			for (std::vector<MirrorOrientation>::iterator mit = mirrorOrientations.begin();
				mit != mirrorOrientations.end(); ++mit)
			{
				changeBeamDirection(*mit);
				d_mirrors[savedPos] = *mit;
				if (securityCheck() == CheckResultNotSecures)
				{
					missing_mirrors.push_back(savedPos);
					break;
				}

			}
			d_mirrors.erase(savedPos);
		}

		if (missing_mirrors.size())
			result = missing_mirrors.size();
	}

	return result;
}

CheckResult TcSafe::securityCheck()
{
	CheckResult result = CheckResultSecures;
	while (1)
	{
		move();

		if (outOfSafe())
		{
			if (d_currentPos == d_exit) result = CheckResultNotSecures;
			else result = CheckResultImpossible;
			break;
		}
		else
		{
			changeBeamDirection();
		}
	}

	return result;
}

bool TcSafe::outOfSafe()
{
	if (d_currentPos.d_x < MIN_ROW_COL || d_currentPos.d_x > d_row || 
		d_currentPos.d_y < MIN_ROW_COL || d_currentPos.d_y > d_col)
		return true;
	return false;
}

void TcSafe::move()
{
	switch (d_beamDirection) {
	case BeamDirectionUp:
		d_currentPos.d_x--;
		break;
	case BeamDirectionDown:
		d_currentPos.d_x++;
		break;
	case BeamDirectionRight:
		d_currentPos.d_y++;
		break;
	case BeamDirectionLeft:
		d_currentPos.d_y--;
		break;
	}
//	cout << "(" << d_currentPos.d_x << "/" << d_currentPos.d_y << ")";
} 

bool TcSafe::changeBeamDirection()
{
	std::map<TcPoint, MirrorOrientation>::iterator it = d_mirrors.find(d_currentPos);
	if (it != d_mirrors.end())
	{
		MirrorOrientation orientation = it->second;

		changeBeamDirection(orientation);

		return true;
	}

	return false;
}

void TcSafe::changeBeamDirection(MirrorOrientation orientation)
{
	switch (d_beamDirection)
	{
	case BeamDirectionUp:
		d_beamDirection = (orientation == MirrorOrientationDirect) ? BeamDirectionRight : BeamDirectionLeft;
		break;
	case BeamDirectionDown:
		d_beamDirection = (orientation == MirrorOrientationDirect) ? BeamDirectionLeft : BeamDirectionRight;
		break;
	case BeamDirectionRight:
		d_beamDirection = (orientation == MirrorOrientationDirect) ? BeamDirectionUp : BeamDirectionDown;
		break;
	case BeamDirectionLeft:
		d_beamDirection = (orientation == MirrorOrientationDirect) ? BeamDirectionDown : BeamDirectionUp;
		break;
	}
}
