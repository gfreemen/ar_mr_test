// ar_ml_test.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>

#include "safe.h"

using namespace std;

int main()
{
	int r, c, m, n;
	int x, y;
	std::vector<TcPoint> mirrors, back_mirrors, missing_mirrors;

	cout << "\nEnter row col mirrors backmirrors:";

	// cin returns false when a character 
	// is entered
	while (1)
	{
		std::cin >> r >> c >> m >> n;

		// validation 
		if (!isValidInput(r, c, m, n))
		{
			cout << "Input not valid !!\n";

			return 1;
		}
		else
		{
			int mcounter = 0;
			while (mcounter < m + n)
			{
				cout << "Enter row col:";
				std::cin >> x >> y;
				if (mcounter++ < m) mirrors.push_back(TcPoint(x, y));
				else back_mirrors.push_back(TcPoint(x, y));
			}

			TcSafe safe(r, c, mirrors, back_mirrors);
			int result = safe.fullSecurityCheck(missing_mirrors);
			cout << "\nSecurity check:" << result << "";
			if (missing_mirrors.size())
			{
				for (std::vector<TcPoint>::iterator it = missing_mirrors.begin();
					it != missing_mirrors.end(); ++it)
				{
					cout << " ";
					it->print();
				}
			}
			cout << "\n";
			mirrors.clear();
			back_mirrors.clear();
			missing_mirrors.clear();
		}
	}


    return 0;
}

