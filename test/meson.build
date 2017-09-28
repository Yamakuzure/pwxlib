want_test     = get_option('tests')
want_torture  = get_option('torture')
install_tests = get_option('install-tests')

if not want_test
        install_tests = false
endif

generate_sym_test_py = find_program('generate-sym-test.py')

path = run_command('sh', ['-c', 'echo "$PATH"']).stdout()
test_env = environment()
test_env.set('PATH', path)
test_env.prepend('PATH', meson.build_root())

test_libpwx_sym_cpp = custom_target(
        'test-libpwx-sym.cpp',
        input : [libpwx_sym_path] + libpwx_all_in_one_header,
        output : 'test-libpwx-sym.cpp',
        command : [generate_sym_test_py, libpwx_sym_path] + libpwx_all_in_one_header,
        capture : true)

if want_test
	tests += [

		[['test_cluster'],
		[files('''
			cluster_check.cpp
			cluster_check.h'''.split())],
		['manual'],
		[]],

		[['test_hash'],
		[files('''
			hash_builder.cpp
			hash_builder.h'''.split())],
		['manual'],
		[]],

		[['test_name'],
		[files('''
			namegen.cpp'''.split())],
		['manual'],
		[]],

		[['test_lib'],
		[files('''
			destroy.cpp
			destroy.h
			env.h
			test_lib.cpp
			test_lib.h
			testColor.cpp
			testColor.h
			testHash.h
			testListRing.h
			testPAH.cpp
			testPAH.h
			testRNG.cpp
			testRNG.h
			testRNG_func.cpp
			testRNG_func.h
			testSCT.cpp
			testSCT.h
			testSet.h
			testSpeed.h
			testStackQueue.h
			testThreads.h'''.split())],
		['manual'],
		[threads]]
	]
endif

if want_torture
	tests += [
		[['torture'],
		[files('''
			torture.cpp
			torture.h'''.split())],
		['manual'],
		[threads]]
	]
endif
