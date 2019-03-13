stream_helpers_sources = '''
        StreamHelpers.cpp
'''.split()

stream_helpers_headers = '''
        StreamHelpers.h
'''.split()

libpwx_external_sources += files(stream_helpers_sources)

install_headers(
        stream_helpers_headers,
        subdir : 'pwxlib/pwx')

stream_helpers_wrappers = '''
        PStreamHelpers
'''.split()
install_headers(
        stream_helpers_wrappers,
        subdir : 'pwxlib')

libpwx_wrapper_list += files(stream_helpers_wrappers)
