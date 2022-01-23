# Top Script that launches the ACADnn algorithm. Launches sequential simulations, can be converted to parallel sims if needed.
# File Name: ACADnn.py
# Date: Dec 18th, 2021
# Authors: Selvaraj Anandaraj, Alisha Handa

import os
import re

# Number of elements you consider based on tightness
stride_range = 2

# Parameter that mentions the tightness about the routing congestion near the memory.
associativity = True  

# Parameter that mentions the transistor size and area o
area = True

# Parameter memory size tightness
memory_size = True 

# Parameter to weigh power also
Power = True

# The code below is only for stride is 2
###########################################################################
#Since stride range is 2, we have only 2 settings for each configuration
tight_l1d_size = ["64kB","128kB"]
loose_l1d_size = ["128kB","256kB"]
tight_l1d_assoc = ["2","4"]
loose_l1d_assoc = ["1","2"]
tight_l1d_taglat = ["2","4"]
loose_l1d_taglat = ["1","2"]
tight_l2_size = ["256kB","512kB"]
loose_l2_size = ["512kB","1024kB"]
tight_mul_oplat = ["2","4"]
loose_mul_oplat = ["1","2"]

l1d_size = tight_l1d_size if memory_size else loose_l1d_size
l1d_assoc = tight_l1d_assoc if associativity else loose_l1d_assoc
l1d_taglat = tight_l1d_taglat if area else loose_l1d_taglat
l2_size = tight_l2_size if memory_size else loose_l2_size
mul_oplat = tight_mul_oplat if area else loose_mul_oplat

##########################################################################

def execute (l2_present, l1d_size, l1d_assoc, l1d_taglat, l2_size, mul_oplat):

   if l2_present == True:
    string = "build/X86/gem5.opt configs/learning_gem5/part1/o3_with_l2.py --l1d_size=\"" + str(l1d_size) + "\" --l1d_assoc=\"" + str(l1d_assoc) +  "\" --l1d_tag_lat=\"" + str(l1d_taglat) + "\" --SimdFloatMul_OpLatency=\""+ str(mul_oplat) + "\" --l2_size=\"" + str(l2_size) + "\""
    config_tag = str(l1d_size) + "_" + str(l1d_assoc) + "_" + str(l1d_taglat) + "_" + str(l2_size) + "_" + str(mul_oplat)
    os.system(string)

    file_handle = open("m5out/stats.txt","r")
    lines = file_handle.readlines()

    string = ""

    for line in lines:
       if (line.find('simTicks') != -1):
          m = re.search(r"\d",line)
          iterator = m.start()

          while line[iterator] != " ":
           string = string + line[iterator]
           iterator = iterator + 1

          performance = int(string)

       string = ""

       if (line.find('0.averagePower') != -1):
          m = re.search(r"\d\d",line)
          iterator = m.start()

          while line[iterator] != " ":
           string = string + line[iterator]
           iterator = iterator + 1

          total_average_power = float(string)

       string = ""

       if (line.find('1.averagePower') != -1):
          m = re.search(r"\d\d",line)
          iterator = m.start()

          while line[iterator] != " ":
           string = string + line[iterator]
           iterator = iterator + 1

          total_average_power = total_average_power + float(string)
   else:

    config_tag = str(l1d_size) + "_" + str(l1d_assoc) + "_" + str(l1d_taglat) + "_l2_absent_" + str(mul_oplat)
    string = "build/X86/gem5.opt configs/learning_gem5/part1/o3_without_l2.py --l1d_size=\"" + str(l1d_size) + "\" --l1d_assoc=\"" + str(l1d_assoc) +  "\" --l1d_tag_lat=\"" + str(l1d_taglat) + "\" --SimdFloatMul_OpLatency=\""+ str(mul_oplat) + "\""
    os.system(string)

    file_handle = open("m5out/stats.txt","r")
    lines = file_handle.readlines()

    string = ""

    for line in lines:
       if (line.find('simTicks') != -1):
          m = re.search(r"\d",line)
          iterator = m.start()

          while line[iterator] != " ":
           string = string + line[iterator]
           iterator = iterator + 1

           performance = int(string)

       string = ""

       if (line.find('0.averagePower') != -1):
          m = re.search(r"\d\d",line)
          iterator = m.start()

          while line[iterator] != " ":
           string = string + line[iterator]
           iterator = iterator + 1

          total_average_power = float(string)

       string = ""

       if (line.find('1.averagePower') != -1):
          m = re.search(r"\d\d",line)
          iterator = m.start()

          while line[iterator] != " ":
           string = string + line[iterator]
           iterator = iterator + 1

          total_average_power = total_average_power + float(string)  
   return performance, total_average_power, config_tag

# Irrespective of memory size we can knock of L2 based on need, use the mean parameters for assoc, tag lat and mul oplat
count = 0

if memory_size == True:
   for i in range (stride_range):
    perf_with_l2,_,__ = execute(True,tight_l1d_size[i], 2, 2, "256kB", 1)  
    perf_without_l2,_,__ = execute(False,tight_l1d_size[i], 2, 2, "256kB", 1)  
    if perf_with_l2 >= perf_without_l2:
     count = count + 1
else:
   for i in range (stride_range):
    perf_with_l2,_,__ = execute(True,loose_l1d_size[i], 2, 2, "256kB", 1)  
    perf_without_l2,_,__ = execute(False,tight_l1d_size[i], 2, 2, "256kB", 1)  
    if perf_with_l2 >= perf_without_l2:
     count = count + 1

# Only if both the strides had better perf use L2
if count == 2:
 l2_present = 0
else:
 l2_present = 1
 print("L2 Cache is present!")

perf_dict = {}
power_dict = {}
l1d_size_dict = {}

if l2_present == 1:
   
   for i in range (stride_range):
    for j in range (stride_range):
     for k in range (stride_range):
      for l in range (stride_range):
       for m in range (stride_range):
        perf, power, config = execute (True,l1d_size[i],int(l1d_assoc[j]),int(l1d_taglat[k]),l2_size[l],int(mul_oplat[m]))
        perf_dict.update({config:perf})
        power_dict.update({config:power})
        l1d_size_dict.update({config:l1d_size[i]})
else:

   for i in range (stride_range):
    for j in range (stride_range):
     for k in range (stride_range):
      for l in range (stride_range):
        perf, power, config = execute (False,l1d_size[i],int(l1d_assoc[j]),int(l1d_taglat[k]),"",int(mul_oplat[l]))
        perf_dict.update({config:perf})
        power_dict.update({config:power})
        l1d_size_dict.update({config:l1d_size[i]})


#print(l1d_size_dict)
#print(perf_dict)
#print(power_dict)

#Find the top 5 performers in terms of performance or power
local_dict = dict(sorted(perf_dict.items(), key=lambda item: item[1]))

iterator = 0
sorted_perf_dict = {}
for i in (local_dict):
  if iterator < 5:
   sorted_perf_dict.update({i:local_dict[i]})
   iterator = iterator + 1

#print(sorted_perf_dict)

local_dict = dict(sorted(power_dict.items(), key=lambda item: item[1]))

iterator = 0
sorted_power_dict = {}
for i in (local_dict):
  if iterator < 5:
   sorted_power_dict.update({i:local_dict[i]})
   iterator = iterator + 1

#print(sorted_power_dict)

#Find the right guy to get benefits based on L1 cache size
iterator = 0
if Power == False:
 #For power and area based scores
 for key in sorted_perf_dict:
  if iterator > 0:
   if l1d_size_dict[key] < min_size:
    min_size = l1d_size_dict[key]
  if iterator == 0:
   min_size = l1d_size_dict[key]
  iterator = iterator + 1

 score = 100
 score_dict = {}
 for key in sorted_perf_dict:
  if l1d_size_dict[key] == min_size:
   score_dict.update({key:(score + 7)})   #More benefit for a smaller area cache
  else:
   score_dict.update({key:(score - 9)})   #Higher loss for a higher area cache 
  score = score - 5
 
else:
 #For the performance and area based scores
 for key in sorted_power_dict:
  if iterator > 0:
   if l1d_size_dict[key] < min_size:
    min_size = l1d_size_dict[key]
  if iterator == 0:
   min_size = l1d_size_dict[key]
  iterator = iterator + 1

 score = 100
 score_dict = {}
 for key in sorted_power_dict:
  if l1d_size_dict[key] == min_size:
   score_dict.update({key:(score + 7)})   #Benefit for a smaller area cache
  else:
   score_dict.update({key:(score - 9)})   #Higher loss for a higher area cache 
  score = score - 5

#score_dict will have the final results
score_dict = dict(sorted(score_dict.items(), key=lambda item: item[1]))
print(score_dict)
