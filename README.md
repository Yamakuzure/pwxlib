PrydeWorX Library
=======================================
The PrydeWorX Library of C++ tools and utilities.

Welcome to the PrydeWorX Library README.
The pwxLib is a small library of tools and utilities and aims at making your
life as a C++ programmer easier.

The library is published under MIT license, which is included both at the
bottom of this file and in the dedicated LICENSE file.


Usage
---------------------------------------
The only thing needed to use the library is to place

> `#include <pwxlib.h>`

in any file you wish to use any of its workers, tools or utilities in.
Alternatively you can include the relevant header for the worker, tool or
utility you wish to use directly. The descriptions below also list the relevant
header you have to include if you do not wish to include this one-for-all
header.

Everything is placed in the namespace `pwx`.

### Advantages of including pwxlib.h
If you include this one-for-all header, you do not have to care about anything.
Everything is simply available through the `pwx` namespace.

### Advantages of including individual headers
Including `pwxlib.h` adds over 60k lines through the preprocessor. This is not
exactly the best method to increase compiler speed. All containers and many
tools are template based, therefore things tend to get lengthy.

Although many of these lines come from standard headers, it might be better to
only include what you really need.
To help you with this, `doxygen` is utilized by default by the build system to
create an HTML API documentation.  
Further more the overviews in this README show which header to include for what.

### Feeding you compiler/linker
To get the correct compiler and linker flags, you can utilize pkg-config. The
package is called pwxlib.  
For the correct compiler flags, use
> `pkg-config --cflags pwxlib`.

For the correct linker flags, use
> `pkg-config --libs pwxlib`.


Naming
---------------------------------------
The project is called "PrydeWorX Library", and the short form is "pwxLib".
But a library filename would be "libpwxlib.so", which just looks stupid, so the
second "lib" is simply omitted.

So while you include "pwxlib.h", you link against "libpwx". Luckily this is not
as bad as it looks, as you can omit the leading "lib", and the linker will do
the right thing anyway.


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


Contributing
---------------------------------------
To contribute to the PrydeWorX Library, fork the current source code from
  [gitHub](https://github.com/Yamakuzure/pwxlib/).
Send a pull request for the changes you like.

If you want to report bugs, please do so at the pwxlib
  [issue tracker](https://github.com/Yamakuzure/pwxlib/issues)
on github.


Basics
---------------------------------------
> `#include <pwxlib/pwx_basic.h>`

This file adds all basic types, classes and functions that are used throughout
the library. The following few might be useful for you, too.

### CException
> `#include <pwxCException.h>`

This is a basic exception class with tracing functionality. It is meant to
provide a tracing exception to get as much information as possible.

To make the most out of this system, the file `pwxlib/pwx_macros.h` provides many
macros to try, catch and throw further exceptions with tracing information.

### CLockable
> `#include <pwxCLockable.h>`

This is a base class to make objects lockable via atomic_flag and lock counting.  
`CLockable` implements a recursive behavior. Every call to `lock()` by the
current lock owner is counted. To unlock, the same amount of calls of the
`unlock()` method is required.

There are two methods that might help out of tight spots, enabling otherwise
impossible techniques:

1. The method `clear_locks()` will completely unlock, no matter how often
   `lock()` was called.
2. Further a thread can ask the object with `lock_count()` how many locks it
   currently holds.

Please keep in mind, that your design might be flawed if you find yourself in a
situation in which you really need either method.

### CLockGuard
> `#include <pwxCLockGuard.h>`

This is a RAII lock guard to lock/unlock one, two or three objects within its
constructor/destructor.  
There are the following advantages when using this class instead of doing locks
directly:

1. Locking is done in the ctor, unlocking in the dtor automatically.
2. The class is exception free and can handle null pointers.
3. The guard can be assigned or copied, making overlapping locks easy to do.
4. If it is not possible to wait for the destructor, the lock(s) can be unlocked
   by resetting to nullptr.

### Debug logging
> `#include <pwx_debug.h>`

Besides many useful macros for debugging possible multi-threading issues, this
header also declares the `debug_log()` and `debug_err()` functions.

Everything in here does nothing unless `LIBPWX_DEBUG` or `PWX_THREADDEBUG` are
defined, so they can be used everywhere and stay out of the way in release
builds.

### General purpose macros
> `#include <pwx_macros.h>`

In here are a lot useful macros for various tasks. All of these are prefixed
with "`PWX_`". The four most prominent groups are the following:

1. `TRY`  
   These macros help with try-catch blocks.

2. `THROW`  
   Special macros for throwing exceptions.

3. `CATCH`  
   Special macros for catching exceptions.

   The specialty of these macros is to utilize the `CException` class and its
   derivatives to build stack traces.

4. `LOCK`  
   Many useful macros for locking, guarding and unlocking.

Please have a look at the header for more information.


Containers
---------------------------------------
> `#include <pwxlib/pwx_containers.h>`

This file adds all containers provided by the library. As all containers are
template based, you might wish to limit the inclusion to what you actually use.

**Important Note**: Please keep in mind that these containers where originally
designed, when C++11 wasn't even a dream. The standard containers back then
weren't good at handling pointers, and objects had to be copied into them.
Therefore the pwxLib containers are as minimal as possible, offering no special
features like iterators.

Further, any application using multiple threads to share a container had to
use a global lock via mutex or semaphore whenever a thread wanted to actually
work with the shared container.

For this reason all pwxLib containers do micro-locking using spinlocks by
themselves. Global "grand" locks are not needed.

The first reason for using the pwxLib containers is void these days, as we have
move semantics. The performance difference should be minimal.

But the second reason still holds. The C++ standard says for the standard
containers:  
>  "_if there is one writer, there shall be no more writers and no readers_"

Nevertheless you really should be absolutely sure, that you really need several
threads to be able to work with a shared container without imposing global
locks. And only then you should use the pwxLib containers, as you will surely
miss the extra features the standard containers offer; more so since C++14.

The containers are, in alphabetical order:

### TChainHash
> `#include <pwxTChainHash.h>`

A chained hash container for variable types.

### TDoubleList
> `#include  "pwxTDoubleList.h"`

A simple doubly linked list for variable types.

### TDoubleRing
> `#include  "pwxTDoubleRing.h"`

A simple doubly linked ring (head and tail are connected) for variable types.

### TOpenHash
> `#include  "pwxTOpenHash.h"`

An open hash container for variable types. This container features "Robin Hood
Hashing", which greatly reduces secondary clustering.

### TQueue
> `#include  "pwxTQueue.h"`

A queue container, pushes to tail, pops from head.

### TSet
> `#include  "pwxTSet.h"`

A set is a group of elements, where each element exists exactly once. Common set
arithmetics like detecting subsets, building intersections, differences and
unions are supported.

### TSingleList
> `#include  "pwxTSingleList.h"`

A simple singly linked list for variable types.

### TSingleRing
> `#include  "pwxTSingleRing.h"`

A simple singly linked ring (head and tail are connected) for variable types.

### TStack
> `#include  "pwxTStack.h"`

A stack container, pushes to tail, pops from tail.


Tools
---------------------------------------
> `#include <pwxlib/pwx_tools.h>`

Here you can find various tools helping with small tasks.

### Math Helpers
> `#include <pwxlib/pwxMathHelpers.h>`

This file adds some tools for various mathematical tasks like handling floating
point comparison, calculating distances and working with degrees.

Note: The code handling floating point comparison is inspired by
Bruce Dawson's article
"[Comparing Floating Point Numbers, 2012 Edition](http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/)"

1. template struct sFloatPoint  
   This template can be used to represent floating point numbers, supporting
   `float`, `double` and `long double`.

2. bool areAlmostEqual(lhs, rhs)  
   Instead of disabling warnings about floating point comparison, this function
   can be used to utilize the `sFloatPoint` struct template for a more exact
   comparison, supporting `float`, `double` and `long double`.

3. absDistance()  
   This function can be used to calculate the normalized distance of two points
   in 2D or 3D space.

4. degToRad()  
   Simple function to calculate the radians out of a given degree.

5. getNormalizedDegree()  
   Simple function to normalize any degree into 0 <= result < 360.

### Stream Helpers
> `#include <pwxlib/pwxStreamHelpers.h>`

This file adds some tools to make working with streams easier.

1. CFormat, CAdjLeft, CAdjRight  
   These are simple helper classes to format numerical output on streams.

2. cropShell()  
   Delete shell representations out of a string.

3. forwardTo()  
   This function will forward the given ifstream until a) eof() is reached, or
   b) the wanted value is found.

4. ltrim()  
   Delete all newline, carriage return, tabs and spaces at the beginning of a
   string.

5. makeTemp()  
   This method is a wrapper to use mkstemp() with ofstream. Furthermore it
   ensures, that the template you provide is safe.

6. rtrim()  
   Delete all newline, carriage return, tabs and spaces at the end of a string.

7. readNextValue()  
   This function extracts a possibly spearated value from an ifstream.

8. skipLineBreak()  
   Skip the following line break(s) and carriage return(s) on an ifstream.

9. tabToSpace()  
   Convert all tabs in a string to spaces.

10. to_&lt;type&gt;()  
   This is a group of templated functions to convert into various other types.
   Please have a look in the header or the HTML documentation, as the list is
   rather long.

11. trim()  
   Delete all newline, carriage return, tabs and spaces at the beginning and at
   the end of a string.


Utilities
---------------------------------------
> `#include <pwxlib/pwx_utils.h`

Currently only one utility class exists, but more are planned.

### CWaveColor
> `#include <pwxlib/pwxCWaveColor.h>`

This is a utility class to work with the frequencies any RGB color is made of.

The main idea of this class is, to build an instance out of RGB values. The
resulting frequencies can be modified by various effects, like Doppler or
gravitation, and then mixed back into a resulting RGB color on demand.

Instead of using RGB colors, an empty instance can be used and filled with RGB
colors or set to a specific wavelength or frequency.

Important: Wavelengths are considered to be nanometers and frequencies are
considered to be gigahertz by the methods of this class.


Workers
---------------------------------------
> `#include <pwxlib/pwx_workers.h`

When starting your program, the library will instantiate the following global
workers. You can use them from anywhere in your program.

### pwx::PAH - The [P]rogram [A]rgument [H]andler
> `#include <pwxlib/pwx_worker_PAH.h`

This is a handler for command line arguments.
The usage is quite simple.

You can instantiate your own instance using the `CArgHandler` class yourself.
> `#include <pwxLib/pwxCArgHandler.h>`

#### PAH usage overview
Call `addArg()` for each argument your program should support. Use
`addPassthrough()` if command line arguments must be preserved for later
distribution to another program.

Be aware, though, that passed arguments are malloc'd C-Strings that you have to
free yourself.

Call `parseArgs(argc, argv)` to have them applied to your arguments. With
`getErrorCount()` the number of errors encountered can be retrieved.

`getError(nr)` returns the error number for error number `nr`, `getErrorStr(nr)`
returns a string with an error text.

`getHelpArg(arg)` returns a string with the short and/or long argument and
parameter if needed.  
`getHelpDesc(arg)` returns a string with the argument descriptions.  
`getHelpStr(arg, length)` returns a string with both the short and/or long
argument plus parameter and description. This string is formatted using the
found maximum lengths of short arguments, long arguments and parameter
names according to the given line length. If the resulting string is too long,
it will line break.

Finally `clearArgs()` frees all allocated memory.

#### Supported types:
The system directly supports `bool`, `[u]int{8,16,32,64}_t`, `float`,
`[long] double` and `std::string` targets.

If a different type is to be handled, a callback function must be installed that
converts a `const char*` parameter into the target type and handles the
processing.

Further it is advised to use a callback function if an argument should be able
to receive and store more than one parameter.

For this reason there are two different kinds of the `addArg()` function. One to
set a target type and a target pointer, the other to install a callback function.

#### Adding a target pointer using addArg()
There are some basic tests to ensure that the `arg_target` and the `arg_type`
make sense. If they do not, bad things may happen, at least the argument might
not do what you expect. This condition is tested with an assert.

Both the short argument and the long argument must be unique. If a given
argument is already known to the handler, it will be ignored! This condition is
tested with an assertion.

Either of the arguments `arg_short` or `arg_long` can be nullptr, but not both.
If both are set to nullptr, the method does nothing! This condition is tested
with an assertion.

If the creation of an argument target instance fails, a `pwx::CException` with
the name "ArgTargetCreationFailed" is thrown. Argument targets are organized
using `pwx::TChainHash`. If the creation of a hash element fails, the thrown
exception has the name "ElementCreationFailed".

If you need to pass arguments to a called process, add the marker separating the
command line arguments from the called process arguments with the method
`addPassthrough()` and not `addArg()`.

#### Adding a callback function using addArg()
Here the same rules apply. The callback function is supposed to work as follows:
The callback function will receive the long argument, if set, otherwise the
short argument as a first parameter. The second parameter will be the command
line parameter(s) that follow(s) the argument as a `const char*`.


### pwx::RNG - The [R]andom [N]-Value [G]enerator
> `#include <pwxlib/pwx_worker_RNG.h`

This worker can be used to produce unique or pseudo random numbers, hashes and
names.

You can instantiate your own instance using the `CRandom` class yourself.
> `#include <pwxLib/pwxCRandom.h>`

The following sets of functions are available.

#### `random()`
These return random numbers as `int32_t`, `int64_t`, `float`, `double` and
`long double`. They can be used with one or to two arguments to get results
between those two or from zero to the one argument.

#### `hash()`
Hash functions for integer arguments, mostly taken from
[Robert Jenkins](http://www.burtleburtle.net/bob/hash/index.html) and
[Thomas Wang](http://www.cris.com/~Ttwang/tech/inthash.htm) and then adapted.  
The hash functions for strings and floating point numbers are of my own
invention. The result is always an `uint32_t`.

#### noise()
These are not the classic Perlin noise functions, but simple wrappers that
transform `hash()` results into a -1.0 to 1.0 `double` range.

#### simplex()
This set of functions produce pseudo random numbers using Simplex Noise in 2D,
3D and 4D.  
The Simplex Noise algorithms have been invented by Ken Perlin. The actual
implementation was inspired by the works of
[Stefan Gustavson](http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf)

#### rndName()
A method that returns a random name built by combining random letters into
syllables.  
**Important**: The returned name is a malloc'd C-String. You are responsible to
free it after usage!


### pwx::SCT - The [S]ine-/[C]osine-[T]able
> `#include <pwxlib/pwx_worker_SCT.h`

This is exactly what it says, a simple "Sine-/Cosine-Table".

*Note*: If you want the initial precision to be anything else than the default
of -1, define `PWX_INITIAL_SCT_PRECISION` to the desired value when compiling
pwxlib.

You can instantiate your own instance using the `CSinCosTable` class yourself.
> `#include <pwxLib/pwxCSinCosTable.h>`

Calculating sine and cosine values does not take much time nowadays as FPUs get
stronger every other day. On the other hand, if you need these values for
on-the-fly calculations of something CPU-consuming like the display of 3D
objects, this little bit of CPU/FPU resources might better be saved.
Sin-/Cos-Tables with a precision of 3 use about 5.49 MiB RAM (total), and this
is not very much either.

Tests show, that a Sin-/Cos-Tables with a precision of 3, meaning 2 x 360,000
values in two arrays, do not differ from on-the-fly calculation until the (worst
case!) 6th digit. (Normally the first 7-8 digits are equal, which should be
enough for most applications.

The default precision, however, is -1. There will be no pre-defined tables, but
all sine and cosine values calculated on-the-fly. You still do not have to care
about the range of your angles, and do not need to transform angles to radians.

#### sin()
Return the sine of a given angle.

#### cos()
Return the cosine of a given angle.

#### sincos()
Get both sine and cosine of a given angle at once.

#### setPrecision()
Set a new precision. The default is 3. Set this to -1 to enable life
calculation.  
Please be aware, that changing the precision means a recalculation of the sine
and cosine arrays. Switching between -1 (life calculation) and the initial value
does not trigger a re-initialization of the tables.

#### getPrecision()
Get the current precision.


LICENSE
---------------------------------------
MIT License

Copyright (c) 2007-2019 Sven Eden

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
