#include "network.h"

int main() {
	test_network();
	printf("\n------------------------------------------------------\n                  *Algorithm Comparisons*\n");

	compare_algorithms("devices_10000_avgdegree_2.3_large_network.txt");
	return 0;
}