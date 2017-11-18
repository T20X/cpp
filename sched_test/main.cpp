#include <iostream>
#include <vector>
#include <unistd.h>
#include <algorithm>

#define BILLION  1000;

using namespace std;

int main()
{
	std::cout << "started" << std::endl;

	std::vector<int> v;

	int x = 1;
	int *k = nullptr;

	while (x < 100000)
	{
		x++;

		// Calculate time taken by a request
		 struct timespec requestStart, requestEnd;
		clock_gettime(CLOCK_REALTIME, &requestStart);
		usleep(300);
		clock_gettime(CLOCK_REALTIME, &requestEnd);
		
	    // Calculate time it took
		 int accum = ( requestEnd.tv_sec * 1000000000 + requestEnd.tv_nsec - 
				       requestStart.tv_sec * 1000000000 -  requestStart.tv_nsec );

		 v.push_back(accum / 1000);

		 k = new int[50];

		 if (x % 1000 == 0)
			 sleep(5);
	}

	for_each(v.begin(), v.end(), [](int a){ printf( "%d\n", a ); }); 
	std::cout << "finished" << std::endl;
}

