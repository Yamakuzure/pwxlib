generate_sym_test_py = find_program('generate-sym-test.py')


test_libpwx_sym_cpp = custom_target(
        'test-libelogind-sym.cpp',
        input : [libpwx_sym_path] + libpwx_headers,
        output : 'test-libelogind-sym.cpp',
        command : [generate_sym_test_py, libpwx_sym_path] + libpwx_headers,
        capture : true)

