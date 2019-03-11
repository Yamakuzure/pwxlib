libpwx_internal_sources = files('''
        pwxlib.h
        pwx_worker_PAH.cpp
        pwx_worker_RNG.cpp
        pwx_worker_SCT.cpp
'''.split())

_libpwx_headers = '''
        basic.h
        containers.h
        tools.h
        utils.h
        pwx_worker_PAH.h
        pwx_worker_RNG.h
        pwx_worker_SCT.h
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
