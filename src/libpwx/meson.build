libpwx_internal_sources = files('''
        pwxlib.h
        pwx_basic.h
        pwx_containers.h
        pwx_helpers.h
        pwx_utils.h
        pwx_workers.cpp
        pwx_workers.h
'''.split())

_libpwx_headers = '''
        pwx_basic.h
        pwx_containers.h
        pwx_helpers.h
        pwx_utils.h
        pwx_workers.h
'''.split()

_libpwx_all_in_one_header = [ 'pwxlib.h' ]

libpwx_headers           += files(_libpwx_headers)
libpwx_all_in_one_header += files (_libpwx_all_in_one_header)

libpwx_sym = 'src/libpwx/pwxlib.sym'

libpwx_pc = configure_file(
        input : 'pwxlib.pc.in',
        output : 'pwxlib.pc',
        configuration : substs)
install_data(libpwx_pc,
             install_dir : pkgconfiglibdir)
