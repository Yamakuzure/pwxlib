#include "hash_builder.h"

int32_t main(int32_t argc, char** argv)
{
	int32_t result = EXIT_SUCCESS;

	if (argc != 2) {
		cerr << "Usage:\n  " << argv[0] << " <destination directory>" << endl;
		return EXIT_FAILURE;
	}

	string destdir = argv[1];



	cout << "Building hash lists in \"" << destdir << "\"." << endl;

	return result;
}
