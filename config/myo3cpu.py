from m5.objects import FUPool,O3CPU
from m5.objects.FuncUnitConfig import *
from m5.objects.FuncUnit import *

class MySimdUnit(SIMD_Unit):

   def __init__ (self, opLat):
      super(SIMD_Unit, self).__init__()
      if opLat != None:
       self.opList = [ OpDesc(opClass='SimdAdd'),
                OpDesc(opClass='SimdAddAcc'),
                OpDesc(opClass='SimdAlu'),
                OpDesc(opClass='SimdCmp'),
                OpDesc(opClass='SimdCvt'),
                OpDesc(opClass='SimdMisc'),
                OpDesc(opClass='SimdMult'),
                OpDesc(opClass='SimdMultAcc'),
                OpDesc(opClass='SimdShift'),
                OpDesc(opClass='SimdShiftAcc'),
                OpDesc(opClass='SimdDiv'),
                OpDesc(opClass='SimdSqrt'),
                OpDesc(opClass='SimdFloatAdd'),
                OpDesc(opClass='SimdFloatAlu'),
                OpDesc(opClass='SimdFloatCmp'),
                OpDesc(opClass='SimdFloatCvt'),
                OpDesc(opClass='SimdFloatDiv'),
                OpDesc(opClass='SimdFloatMisc'),
                OpDesc(opClass='SimdFloatMult',opLat=opLat),
                OpDesc(opClass='SimdFloatMultAcc'),
                OpDesc(opClass='SimdFloatSqrt'),
                OpDesc(opClass='SimdReduceAdd'),
                OpDesc(opClass='SimdReduceAlu'),
                OpDesc(opClass='SimdReduceCmp'),
                OpDesc(opClass='SimdFloatReduceAdd'),
                OpDesc(opClass='SimdFloatReduceCmp') ]

class MyFUPool(FUPool):

   def __init__ (self,opLat):
      super(FUPool, self).__init__()

      self.FUList = [ IntALU(), IntMultDiv(), FP_ALU(), FP_MultDiv(), ReadPort(),
                   MySimdUnit(opLat),PredALU(), WritePort(), RdWrPort(), IprPort() ]

      #for i in range (num_simd_units):
      #   self.FUList.append(SIMD_Unit())

class MyO3CPU(O3CPU):

    def __init__(self, opLat=None):
        super(O3CPU, self).__init__()

        self.fuPool = MyFUPool(opLat)
