want_html  = get_option('html')
want_latex = get_option('latex')
want_man   = get_option('man')

result  = run_command(find_src_sh)
sources = result.stdout().strip().split('\n')

doxsub = configuration_data()
doxsub.set('PACKAGE_NAME',    substs.get('PACKAGE_NAME'))
doxsub.set('PACKAGE_VERSION', substs.get('PACKAGE_VERSION'))
doxsub.set('PACKAGE_SOURCES', ' '.join(sources))
doxsub.set('INCLUDE_PATHS',   '../' + ' ../'.join(include_paths))
doxsub.set('OUTPUT_PATH',     '@0@/doc'.format(meson.build_root()))
doxsub.set('WANT_HTML',       want_html.to_string( 'YES', 'NO'))
doxsub.set('WANT_LATEX',      want_latex.to_string('YES', 'NO'))
doxsub.set('WANT_MAN',        want_man.to_string(  'YES', 'NO'))

doxyfile = configure_file(
        input : 'doxyfile.in',
        output : 'doxyfile',
        configuration : doxsub)

run_target('docs', command: [doxygen, doxyfile])
