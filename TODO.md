List of current TODOs
=======================================

Porting:
---------------------------------------

These are the old parts that haven't been ported yet.

1. CFH (CCnfFileHandler.h)
2. MRF (CMemRingFactory.h)  
   Is this still needed?

General issues:
---------------------------------------

1. `test/cluster_check.cpp` : Make multi-threaded with background output queue. The single-threaded approach
   used right now is good for checking the speed, but that's not the real purpose of the program. 
