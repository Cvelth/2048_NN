#include <fstream>
#include <iostream>
int main(int argc, char** argv) {
	if (argc < 2)
		return -1;
	if (strcmp(argv[1], "-help") == 0)
		std::cout << "Use a path to the Neural Network as the first parameter.\n";
	else
		std::cout << "Unsupported parameter was passed.\n";
	return 0;
}