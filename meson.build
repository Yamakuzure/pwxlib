project('pwxLib', 'cpp',
        version : '0.8.99',
        license : 'LGPLv2+',
        default_options: [
                'cpp_std=c++14',
                'prefix=/usr',
                'sysconfdir=/etc',
                'localstatedir=/var',
        ],
        meson_version : '>= 0.40',
       )

conf = configuration_data()
conf.set_quoted('PACKAGE_STRING',  meson.project_name() + ' ' + meson.project_version())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('PACKAGE_URL',     'https://pwxlib.sourceforge.net/')
m4_defines = []

# Set general directories
prefixdir = get_option('prefix')
if not prefixdir.startswith('/')
        error('Prefix is not absolute: "@0@"'.format(prefixdir))
endif
bindir = join_paths(prefixdir, get_option('bindir'))
libdir = join_paths(prefixdir, get_option('libdir'))
sysconfdir = join_paths(prefixdir, get_option('sysconfdir'))
includedir = join_paths(prefixdir, get_option('includedir'))
datadir = join_paths(prefixdir, get_option('datadir'))

pkgconfigdatadir = join_paths(datadir, 'pkgconfig')
pkgconfiglibdir = join_paths(libdir, 'pkgconfig')
pkgincludedir = join_paths(includedir, 'pwx')

docdir = '-'.join([join_paths(datadir, 'doc/pwxLib'),
                             meson.project_version()])

#####################################################################

substs = configuration_data()
substs.set('PACKAGE_URL',     'https://github.com/pwx/pwx')
substs.set('PACKAGE_VERSION', meson.project_version())
substs.set('prefix',          prefixdir)
substs.set('exec_prefix',     prefixdir)
substs.set('libdir',          libdir)
substs.set('includedir',      includedir)

#####################################################################

cxx = meson.get_compiler('cpp')
cxx_id = cxx.get_id()
pkgconfig = import('pkgconfig')
check_compilation_sh = find_program('meson/check_compilation.sh')

foreach arg : ['-Wextra',
               '-pedantic',
               '-Wpedantic',
               '-Werror=missing-prototypes',
               '-Werror=implicit-function-declaration',
               '-Werror=missing-declarations',
               '-Werror=return-type',
               '-Werror=incompatible-pointer-types',
               '-Werror=format=2',
               '-fdiagnostics-show-option',
               '-fvisibility=hidden',
               '-fstack-protector',
               '-fstack-protector-strong',
               '-fPIC',
               '-fPIE',
              ]
        if cxx.has_argument(arg)
                add_project_arguments(arg, language : 'cpp')
        endif
endforeach

if cxx.compiles('
   #include <ctime>
   #include <cinttypes>
   typedef uint64_t usec_t;
   usec_t now(clockid_t clock);
   int main(void) {
           struct timespec now;
           return 0;
   }
', name : '-Werror=shadow with local shadowing')
        add_project_arguments('-Werror=shadow', language : 'cpp')
endif

if cxx_id == 'clang'
        foreach arg : ['-Wno-typedef-redefinition',
                       '-Wno-gnu-variable-sized-type-not-at-end',
                      ]
                if cxx.has_argument(arg,
                                   name : '@0@ is supported'.format(arg))
                        add_project_arguments(arg, language : 'cpp')
                endif
        endforeach
endif

link_test_cxx = files('meson/link_test.cpp')

# --as-needed and --no-undefined are provided by meson by default,
# run mesonconf to see what is enabled
foreach arg : ['-Wl,-z,relro',
               '-Wl,-z,now',
               '-pie',
              ]

        have = run_command(check_compilation_sh,
                           cxx.cmd_array(), '-x', 'cpp', arg,
                           '-include', link_test_cxx).returncode() == 0
        message('Linking with @0@ supported: @1@'.format(arg, have ? 'yes' : 'no'))
        if have
                add_project_link_arguments(arg, language : 'cpp')
        endif
endforeach

if get_option('buildtype') != 'debug'
        foreach arg : ['-ffunction-sections',
                       '-fdata-sections',
                       '-fexceptions']
                if cxx.has_argument(arg,
                                   name : '@0@ is supported'.format(arg))
                        add_project_arguments(arg, language : 'cpp')
                endif
        endforeach

        foreach arg : ['-Wl,--gc-sections']
                have = run_command(check_compilation_sh,
                                   cxx.cmd_array(), '-x', 'cpp', arg,
                                   '-include', link_test_cxx).returncode() == 0
                message('Linking with @0@ supported: @1@'.format(arg, have ? 'yes' : 'no'))
                if have
                        add_project_link_arguments(arg, language : 'cpp')
                endif
        endforeach
endif

#####################################################################
# compilation result tests

conf.set('__SANE_USERSPACE_TYPES__', true)

decl_headers = '''
#include <cuchar>
'''
foreach decl : ['char16_t',
                'char32_t',
               ]

        # We get -1 if the size cannot be determined
        have = cxx.sizeof(decl, prefix : decl_headers) > 0
        conf.set('HAVE_' + decl.underscorify().to_upper(), have)
endforeach

#####################################################################

doxygen = find_program('doxygen', required : false)
mkdir_p = 'mkdir -p $DESTDIR/@0@'

############################################################

# Set debug options
annotations = get_option('annotations')
debug       = get_option('debug')
graphite    = get_option('graphite')
profile     = get_option('profile')
spinlocks   = get_option('spinlocks')
threaddebug = get_option('threaddebug')
yielding    = get_option('yielding')

if profile
        annotations = false
        debug       = false
        threaddebug = false
        add_project_arguments('-pg', language : 'cpp')
        add_project_link_arguments('-pg', language : 'cpp')
endif

if annotations
        debug = true
        conf.set('PWX_ANNOTATIONS', true)
else
	if yielding
	        conf.set('PWX_USE_FLAGSPIN_YIELD', true)
	        spinlocks = true
	endif
        if spinlocks
                conf.set('PWX_USE_FLAGSPIN', true)
	endif
endif

if threaddebug
        debug = true
        conf.set('PWX_THREADDEBUG', true)
endif

if debug
        conf.set('LIBPWX_DEBUG', true)
        foreach arg : ['-ggdb3',
                       '-Og']
                if cxx.has_argument(arg,
                                   name : '@0@ is supported'.format(arg))
                        add_project_arguments(arg, language : 'cpp')
                endif

                have = run_command(check_compilation_sh,
                                   cxx.cmd_array(), '-x', 'c', arg,
                                   '-include', link_test_cxx).returncode() == 0
                message('Linking with @0@ supported: @1@'.format(arg, have ? 'yes' : 'no'))
                if have
                        add_project_link_arguments(arg, language : 'cpp')
                endif
        endforeach
endif

#####################################################################

threads = dependency('threads')

#####################################################################

want_tests    = get_option('tests')
install_tests = get_option('install-tests')

if want_tests != 'true'
        install_tests = 'false'
endif

tests = []

#####################################################################

config_h = configure_file(
        output : 'config.h',
        configuration : conf)

includes = include_directories('src/arg_handler',
                               'src/basic',
                               'src/container',
                               'src/functions',
                               'src/libpwx',
                               'src/math_helpers',
                               'src/random',
                               'src/wavecolor')

add_project_arguments('-include', 'config.h', language : 'cpp')

libpwx_headers = []

subdir('src/basic')
subdir('src/functions')
subdir('src/math_helpers')
subdir('src/random')
subdir('src/container')
subdir('src/arg_handler')
subdir('src/wavecolor')
subdir('src/libpwx')

libpwx_sym_path = '@0@/@1@'.format(meson.current_source_dir(), libpwx_sym)
libpwx = shared_library(
        'pwx',
        basic_sources,
        funcs_sources,
        math_helpers_sources,
        random_sources,
        container_sources,
        arg_sources,
        wavecolor_sources,
        libpwx_internal_sources,
        version : meson.project_version(),
        include_directories : includes,
        cpp_args : ['-DPWX_EXPORTS'],
        link_args : ['-shared'],
#        link_args : ['-shared',
#                     '-Wl,--version-script=' + libpwx_sym_path],
        dependencies : [threads],
        link_depends : libpwx_sym,
        install : true,
        install_dir : libdir)

install_headers(
        libpwx_headers,
        subdir : 'pwx')

############################################################

# binaries that have --help and are intended for use by humans.
public_programs = []

subdir('test')
#subdir('tools/drawwave')
#subdir('doc')

############################################################

test_libpwx_sym = executable(
        'test-libpwx-sym',
        test_libpwx_sym_cpp,
        include_directories : includes,
        link_with : [libpwx],
        install : false)
test('test-libpwx-sym',
     test_libpwx_sym)

############################################################

install_data('README',
             'NEWS',
             'LICENSE.GPL2',
             'LICENSE.LGPL2.1',
             install_dir : docdir)

############################################################

status = [
        '@0@ @1@'.format(meson.project_name(), meson.project_version()),

        'prefix:                            @0@'.format(prefixdir),
        'sysconf dir:                       @0@'.format(sysconfdir),
        'includedir:                        @0@'.format(pkgincludedir),
        'lib dir:                           @0@'.format(libdir),
        'bin dir:                           @0@'.format(bindir),
        'data dir:                          @0@'.format(datadir)]

message('\n         '.join(status))
