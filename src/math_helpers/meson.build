mathhelpers_sources = files('''
        CSinCosTable.cpp
        MathHelpers.cpp
'''.split())

mathhelpers_headers = '''
        CSinCosTable.h
        MathHelpers.h
'''.split()

libmathhelpers = static_library(
        'mathhelpers',
        mathhelpers_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        install : false)

install_headers(
        mathhelpers_headers,
        subdir : 'pwxlib/pwx')

mathhelpers_wrappers = '''
        PMath
        PSinCos
'''.split()
install_headers(
        mathhelpers_wrappers,
        subdir : 'pwxlib')

libpwx_wrapper_list += files(mathhelpers_wrappers)
