mathhelpers_sources = files('''
        CSinCosTable.cpp
        MathHelpers.cpp
'''.split())

mathhelpers_headers = '''
        pwxCSinCosTable.h
        pwxMathHelpers.h
'''.split()

libpwx_headers += files(mathhelpers_headers)

libmathhelpers = static_library(
        'mathhelpers',
        mathhelpers_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        install : false)
