Build and Install
---------------------------------------
Building and installation is done using meson and ninja.
For simpler usage a `configure` script and a `Makefile` are provided.

To see all configuration options, you can use either  
> `./configure --help` or `meson configure build`.

In the latter case the build directory must be prepared first by using
> `meson build`.

The text file `meson_options.txt` also contains some more information on the
project options.

The configuration by default installs in `/usr` and attempts to build the HTML
API documentation using `doxygen`.

To build the library do:
> `ninja -C build` or `make all`

To install the library do:
> `ninja -C build install` or `make install`

To run the tests do:
> `ninja -C build test` or `make test`
