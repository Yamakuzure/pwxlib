want_html  = get_option('html')
doxygen    = find_program('doxygen', required : want_html)

result  = run_command(find_src_sh)
sources = result.stdout().strip().split('\n')
outdir  = '@0@/doc'.format(meson.build_root())

doxsub = configuration_data()
doxsub.set('PACKAGE_NAME',    substs.get('PACKAGE_NAME'))
doxsub.set('PACKAGE_VERSION', substs.get('PACKAGE_VERSION'))
doxsub.set('PACKAGE_SOURCES', ' '.join(sources))
doxsub.set('INCLUDE_PATHS',   '../' + ' ../'.join(include_paths))
doxsub.set('OUTPUT_PATH',     outdir)
doxsub.set('WANT_HTML',       want_html.to_string( 'YES', 'NO'))
doxsub.set('DOX_FOOTER',      '@0@/@1@'.format(meson.current_source_dir(), 'doxfooter.html'))
doxsub.set('DOX_LAYOUT',      '@0@/@1@'.format(meson.current_source_dir(), 'doxylayout.xml'))
doxsub.set('DOX_STYLE',       '@0@/@1@'.format(meson.current_source_dir(), 'doxstyle.css'))

doxyfile = configure_file(
        input : 'doxyfile.in',
        output : 'doxyfile',
        configuration : doxsub)

doc_gen = custom_target('doc_gen',
           capture          : true,
           build_by_default : want_html,
           command          : [doxygen, doxyfile],
           depend_files     : [doxyfile],
           install          : false,
           output           : 'doxygen.log' )

if want_html
        install_subdir('@0@/html'.format(outdir),
                     install_dir : docdir)
endif
