<<<<<<< HEAD
# ACADnn
Accelerating Conventional Architectures for Deep Neural Networks
=======
ACADnn: Accelerating Conventional Architectures for Deep Neural Networks

The entire experiment was done using an open source gem5 simulator. The algorithm was coded in Python and is in the file named acadnn.py.
The workload we used for testing on the gem5 out of order core is the CPP implementation of Lenet. It is present here as Lenet.cpp.
The config directory has the files which tells the gem5 simulator about how the Out of Order core is connected to the memory system.
The various files present there has a CPU system which has a O3 model with L2 and without L2 cache. MyO3CPU is an extended version of 
the in-built CPU OoO model to add some extensions we wanted.
>>>>>>> master
