#!/usr/bin/env python3
import sys, re

print('#include <cstdio>')
for header in sys.argv[3:]:
    match = re.search('^.*/([^/]+)', header)
    if match:
        hfile = match.group(1)
        check = re.search('fwd', hfile)
        if not check:
            print('#include "{}"'.format(header.split('/')[-1]))

has_debug = sys.argv[2];

# Step one : Generate test functions
for line in open(sys.argv[1]):

    # Classes
    match = re.search('^ +pwx::(C[a-zA-Z0-9_]+)[:;]', line)
    if match:
        cstr = match.group(1)
        print("\nstatic void test_{}(void) {}".format(cstr, "{"))

        # The kind of ctor depends on type... obviously
        if cstr == "CException":
            print('\tpwx::{} test_obj("{}","{}","{}","{}","{}");'.format(
                cstr, "name", "what", "where", "func", "when"))

        elif cstr == "CLockGuard":
            print("\tpwx::CLockable lock_me;")
            print("\tpwx::{} test_obj(&lock_me);".format(cstr))

        elif cstr == "CSinCosTable":
            print("\tpwx::{} test_obj(2);".format(cstr))

        else:
            print("\tpwx::{} test_obj;".format(cstr))

        print("\tprintf(\"%16s: %p\\n\", \"{}\", (void*)&test_obj);".format(cstr))
        print("}")
        continue

    # Templates
    match = re.search('^ +pwx::(T[a-zA-Z0-9_]+)[:;]', line)
    if match:
        tstr  = match.group(1)
        check = re.search('Hash', line)
        elem  = re.search('T.*Element', line)

        print("\nstatic void test_{}(void) {}".format(tstr, "{"))

        if elem:
            print("\tint* data = new int(42);")

        if check:
            if elem:
                print("\tint key = 69;")
                print("\tpwx::{}<int,int> test_obj(key, data);".format(tstr))
            else:
                print("\tpwx::{}<int,int> test_obj;".format(tstr))

        elif elem:
            print("\tpwx::{}<int> test_obj(data);".format(tstr))

        else:
            print("\tpwx::{}<int> test_obj;".format(tstr))

        print("\tprintf(\"%16s: %p\\n\", \"{}\", (void*)&test_obj);".format(tstr))
        print("}")


# Step two : Generate function array
print('''
void* functions[][2] = {''')

for line in open(sys.argv[1]):
    # Functions
    match = re.search('^ +pwx::([a-z][a-zA-Z0-9_]+)[*:;]', line)

    if match:
        fname = match.group(1)

        # Skip members of the private_ sub namespace
        if fname == "private_":
            continue

        # Skip debug_* if debugging was disabled
        if has_debug != 'true' and re.search('debug', fname):
            continue

        tofn  = re.search('^to_[a-z_0-9]+$', fname)
        setfn = re.search('^set_[a-z]+$', fname)

        print("\t" + '''{''' + " (void*)\"{}\", ".format(fname), end='')

        # Here we have the problem, that some functions are overloaded,
        # templated or both.
        # We want all overloads to get checked. But further, the compiler
        # needs to know which of the overload/template to use.
        if fname == "are_locked":
            print("(void*)static_cast<{0}(*)({1},{1})>(&pwx::{2})".format(
                "bool", "const pwx::CLockable*", fname) + ''' },''')
            print("\t" + '''{''' + " (void*)\"{}\", ".format(fname), end='')
            print("(void*)static_cast<{0}(*)({1},{1},{1})>(&pwx::{2})".format(
                "bool", "const pwx::CLockable*", fname) + ''' },''')
        elif fname == "absDistance":
            print("(void*)static_cast<{0}(*)({0},{0},{0},{0})>(&pwx::{1})".format(
                "double", fname) + ''' },''')
            print("\t" + '''{''' + " (void*)\"{}\", ".format(fname), end='')
            print("(void*)static_cast<{0}(*)({0},{0},{0},{0},{0},{0})>(&pwx::{1})".format(
                "double", fname) + ''' },''')
        elif fname == "areAlmostEqual":
            print("(void*)static_cast<{0}(*)({1},{1})>(&pwx::{2})".format(
                "bool", "float", fname) + ''' },''')
            print("\t" + '''{''' + " (void*)\"{}\", ".format(fname), end='')
            print("(void*)static_cast<{0}(*)({1},{1})>(&pwx::{2})".format(
                "bool", "double", fname) + ''' },''')
            print("\t" + '''{''' + " (void*)\"{}\", ".format(fname), end='')
            print("(void*)static_cast<{0}(*)({1},{1})>(&pwx::{2})".format(
                "bool", "long double", fname) + ''' },''')
        elif fname == "degToRad":
            print("(void*)pwx::{}<double>,".format(fname) + ''' },''')
        elif fname == "getNormalizedDegree":
            print("(void*)pwx::{}<double>,".format(fname) + ''' },''')
        elif fname == "try_locks":
            print("(void*)static_cast<{0}(*)({1},{1})>(&pwx::{2})".format(
                "bool", "const pwx::CLockable*", fname) + ''' },''')
            print("\t" + '''{''' + " (void*)\"{}\", ".format(fname), end='')
            print("(void*)static_cast<{0}(*)({1},{1},{1})>(&pwx::{2})".format(
                "bool", "const pwx::CLockable*", fname) + ''' },''')
        elif fname == "unlock_all":
            print("(void*)static_cast<{0}(*)({1},{1})>(&pwx::{2})".format(
                "bool", "const pwx::CLockable*", fname) + ''' },''')
            print("\t" + '''{''' + " (void*)\"{}\", ".format(fname), end='')
            print("(void*)static_cast<{0}(*)({1},{1},{1})>(&pwx::{2})".format(
                "bool", "const pwx::CLockable*", fname) + ''' },''')
        elif fname == "readNextValue":
            print("(void*)pwx::{}<int>,".format(fname) + ''' },''')
        elif tofn:
            print("(void*)pwx::{}<char*>,".format(fname) + ''' },''')
        elif setfn:
            print("(void*)static_cast<{0}(*)({1},{1})>(&pwx::{2})".format(
                "pwx::TSet<int>*", "const pwx::TSet<int>* const", fname) + ''' },''')
            print("\t" + '''{''' + " (void*)\"{}\", ".format(fname), end='')
            print("(void*)static_cast<{0}(*)(const {0}&,const {0}&)>(&pwx::{1})".format(
                "pwx::TSet<int>", fname) + ''' },''')
        else:
            # This is the easy default for non-overloads.
            print("(void*)pwx::{}".format(fname) + ''' },''')


# Step three: Add global workers last to the array:
for line in open(sys.argv[1]):
    match = re.search('^ +pwx::(PAH|RNG|SCT)[:;]', line)
    if match:
        wname = match.group(1)
        print("\t" + '''{''' + " (void*)\"{}\", ".format(wname), end='')
        print("(void*)&pwx::{}".format(wname) + ''' },''')


# Step four : Call the test functions
print("{};\n\nint main(void) {}".format("}", "{"))
for line in open(sys.argv[1]):
    match = re.search('^ +pwx::([CT][a-zA-Z0-9_]+)[*:;]', line)
    if match:
        print("\ttest_{}();".format(match.group(1)))

# Step five : Add the function test loop
print('''
\tfor (unsigned i = 0; i < sizeof(functions)/sizeof(void*)/2; i++)
\t\tprintf("%16s: %p\\n", (const char*)functions[i][0], functions[i][1]);
\treturn 0;
}\n''')
