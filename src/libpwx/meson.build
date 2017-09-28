libpwx_internal_sources = files('''
        pwx.h
        pwx_basic.h
        pwx_containers.h
        pwx_helpers.h
        pwx_utils.h
        pwx_workers.cpp
        pwx_workers.h
'''.split())

_libpwx_headers = '''
        pwx.h
        pwx_basic.h
        pwx_containers.h
        pwx_helpers.h
        pwx_utils.h
        pwx_workers.h
'''.split()

_libpwx_all_in_one_header = [ 'pwx.h' ]

libpwx_main_headers      += files(_libpwx_headers)
libpwx_all_in_one_header += files (_libpwx_all_in_one_header)

libpwx_sym = 'src/libpwx/libpwx.sym'

libpwx_pc = configure_file(
        input : 'libpwx.pc.in',
        output : 'libpwx.pc',
        configuration : substs)
install_data(libpwx_pc,
             install_dir : pkgconfiglibdir)
