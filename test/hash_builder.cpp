#include "hash_builder.h"

int32_t main(int32_t argc, char** argv)
{
	int32_t result = EXIT_SUCCESS;

	if (argc != 3) {
		cerr << "Usage:\n  " << argv[0] << " <destination directory> <number of lines>" << endl;
		return EXIT_FAILURE;
	}

	std::string destdir = argv[1];
	while (destdir.size() && (destdir[destdir.size() - 1] == '/'))
		destdir.erase(destdir.size() - 1);

	// check first argument
	if (!destdir.size()) {
		cerr << "No valid destination directory given!" << endl;
		return EXIT_FAILURE;
	} else if (!pwx_file_exists(destdir.c_str())) {
		cerr << "output path \"" << destdir << "\" does not exist!" << endl;
		return EXIT_FAILURE;
	} else if (!pwx_file_isW(destdir.c_str())) {
		cerr << "output path \"" << destdir << "\" is not writable!" << endl;
		return EXIT_FAILURE;
	}

	// check second argument
	int32_t argMax = pwx::to_int32(argv[2]);
	if (!(argMax > 0)) {
		cerr << "Number \"" << argv[2] << "\" is no legal number (" << argMax << ")" << endl;
		return EXIT_FAILURE;
	}


	int32_t maxInt16  = static_cast<int32_t>(std::numeric_limits<int16_t>::max())
					  - static_cast<int32_t>(std::numeric_limits<int16_t>::lowest());
	int32_t maxUInt16 = static_cast<int32_t>(std::numeric_limits<uint16_t>::max());


	cout << "Building hash lists in \"" << destdir << "\"." << endl;

	// --- int16_t ---
	//-----------------
	string destfile = destdir + "/hash_int16.csv";
	result = build_hash_list_num<int16_t>(destfile, maxInt16 < argMax ? maxInt16 : argMax);

	// --- uint16_t ---
	//------------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_uint16.csv";
		result = build_hash_list_num<uint16_t>(destfile, maxUInt16 < argMax ? maxUInt16 : argMax);
	}

	// --- int32_t ---
	//-----------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_int32.csv";
		result = build_hash_list_num<int32_t>(destfile, argMax);
	}

	// --- uint32_t ---
	//------------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_uint32.csv";
		result = build_hash_list_num<uint32_t>(destfile, argMax);
	}

	// --- int64_t ---
	//-----------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_int64.csv";
		result = build_hash_list_num<int64_t>(destfile, argMax);
	}

	// --- uint64_t ---
	//------------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_uint64.csv";
		result = build_hash_list_num<uint64_t>(destfile, argMax);
	}

	// --- float ---
	//---------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_float.csv";
		result = build_hash_list_num<float>(destfile, argMax);
	}

	// --- double ---
	//----------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_double.csv";
		result = build_hash_list_num<double>(destfile, argMax);
	}

	// --- long double ---
	//---------------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_long_double.csv";
		result = build_hash_list_num<long double>(destfile, argMax);
	}

	// --- char ---
	//--------------
	if (EXIT_SUCCESS == result) {
		destfile = destdir + "/hash_string.csv";
		result = build_hash_list_char(destfile, argMax);
	}


	return result;
}
