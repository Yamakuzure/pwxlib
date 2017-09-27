mathhelpers_sources = files('''
        CSinCosTable.cpp
        CSinCosTable.h
        MathHelpers.cpp
        MathHelpers.h
'''.split())

mathhelpers_headers = '''
        CSinCosTable.h
        MathHelpers.h
'''.split()

libpwx_headers += files(mathhelpers_headers)

libmathhelpers = static_library(
        'mathhelpers',
        mathhelpers_sources,
        cpp_args : ['-DPWX_EXPORTS'],
        include_directories : includes,
        install : false)
