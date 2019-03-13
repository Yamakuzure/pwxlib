wavecolor_sources = '''
        CWaveColor.cpp
'''.split()

wavecolor_headers = '''
        CWaveColor.h
'''.split()

libpwx_external_sources += files(wavecolor_sources)

install_headers(
        wavecolor_headers,
        subdir : 'pwxlib/pwx')

wavecolor_wrappers = '''
        PWaveColor
'''.split()
install_headers(
        wavecolor_wrappers,
        subdir : 'pwxlib')

libpwx_wrapper_list += files(wavecolor_wrappers)
