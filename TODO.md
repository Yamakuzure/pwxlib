List of current TODOs
=======================================

Porting:
---------------------------------------

These are the old parts that haven't been ported yet.

1. CFH (CCnfFileHandler.h)


General issues:
---------------------------------------

1. `test/cluster_check.cpp` : Make multi-threaded with background output queue. The single-threaded approach
   used right now is good for checking the speed, but that's not the real purpose of the program. 
2. All the preprocessor magic, introduced to allow detailed thread logging, is no longer needed and should be
   removed. tsan is more than enough for the task.
3. The next release should be buildable using Visual Studio 2017+
4. When the pwxLib was initially started, Unit Tests weren't a thing. However, for such a library Unit Tests
   are perfect. `test_lib` should really be split up into nice and convenient Unit Tests.


Important issues:
---------------------------------------

1. After test_lib being free of any tsan errors and warnings, torture should be used to improve thread safety
   even further.
2. Thread safety has been purposely overdone and has to be reduced to what really is needed now, so the
   (currently not so great) performance improves again.
3. All elements and containers should support move semantics. We're at C++17 after all!


Planned features:
---------------------------------------
1. CFH from CCnfFileHandler.h has to be ported.
2. CSimpleTextureCreator : A simple first version should be included.
3. CArgHandler should allow adding arguments using initializer lists. The current way is ... uhm... not nice.
