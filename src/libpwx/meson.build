libpwx_internal_sources = files('''
        pwx.h
'''.split())

_libpwx_headers = '''
        pwx.h
'''.split()

libpwx_headers += files(_libpwx_headers)

libpwx_pc = configure_file(
        input : 'libpwx.pc.in',
        output : 'libpwx.pc',
        configuration : substs)
install_data(libpwx_pc,
             install_dir : pkgconfiglibdir)
