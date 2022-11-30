import subprocess
import pandas as pd
import numpy as np
import csv
import MatrixGen

#Assign the dimensions
i = 500
j = 500
k = 500

#Assign file names
input1 = "matrix1.txt"
input2 = "matrix2.txt"
output = "out.txt"

#Generating matrices (if needed)
MatrixGen.genMatrix(i,j,k)

#Compiles P1 c code
subprocess.call(["gcc", "P1.c", "-o","P1.out", "-pthread"])

#Compiles P2 c code
subprocess.call(["gcc", "P2.c", "-o","P2.out", "-pthread"])

#Compiles the scheduler c code
subprocess.call(["gcc", "group21_assignment2.c", "-o","sched.out"])

#Saves the command to be run from terminal as a string
execstring = "./sched.out " + str(i) + " " + str(j) + " " + str(k) + " " + input1 + " " + input2 + " " + output
#eample string : ./sched.out 50 50 50 in1.txt in2.txt out.txt
print("Executing...")
print("Command executed : " + execstring)

#Makes the string into a list for subprocess.call to execute it
subprocess.call(execstring.split())
