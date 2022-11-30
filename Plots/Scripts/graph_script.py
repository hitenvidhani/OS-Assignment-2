# %%
import subprocess
import pandas as pd
import numpy as np
import csv

# %%
LONG_LONG_MAX =  9223372036854775807

#CHANGE FOR DIMS HERE
n = 375
m = 800 
m2 = 476

#f =  open("Matrix_out.txt")
mat = np.random.randint(0,1000,size=(n,m))
print(mat)
np.savetxt("matrix1.txt",mat,fmt='%i')

mat2 = np.random.randint(0,1000,size=(m,m2))
print(mat2)
np.savetxt("matrix2.txt",mat2,fmt='%i')

matRes = np.matmul(mat,mat2)
print(matRes)
print(matRes.shape)
np.savetxt("matrixres.txt",matRes,fmt='%i')

# %%
subprocess.call(["gcc", "P1.c", "-o","P1.out", "-pthread"])
subprocess.call(["gcc", "P2.c", "-o","P2.out", "-pthread"])


# subprocess.call("gcc schedulercopy.c")

f1 = open('datap1.csv', 'w')
f2 = open('datap2.csv', 'w')

writer1 = csv.writer(f1)
writer2 = csv.writer(f2)

header1 = ['dim1', 'dim2', 'dim3', 'num_threads', 'time (ns)']
header2 = ['dim1', 'dim2', 'dim3', 'num_threads', 'time (ns)']

writer1.writerow(header1)
writer2.writerow(header2)

f1.close()
f2.close()

# %%

#FOR THREADS CHANGE HERE
for i in range(1, 100):
    p1 = "./P1.out "+str(n)+" "+str(m)+" "+str(m2)+" "+"matrix1.txt matrix2.txt out.txt "+ str(i) +" 1"
    tmp=subprocess.call(p1.split()) 
    print("printing result")
    #print(tmp)
    p2 = "./P2.out "+str(n)+" "+str(m)+" "+str(m2)+" "+"matrix1.txt matrix2.txt out.txt 1 "+ str(i)
    tmp=subprocess.call(p2.split()) 
    print("printing result")
    #print(tmp)
# #dataP1.txt

# %%
df = pd.read_csv("datap1.csv")

#df = df.groupby('num_threads').min('time(ns)')
#print(df.head)

import matplotlib.pyplot as plt

ax = plt.axes()
ax.scatter(df.index,df['time (ns)'])
ax.plot(df.index,df['time (ns)'])

z = np.polyfit(df.index, df['time (ns)'], 2)
p = np.poly1d(z)
ax.plot(df.index, p(df.index))

ax.set_xlabel('No. of Threads')
ax.set_ylabel('Time')
ax.set_title("P1 execution time vs No of threads for dim "+str(n)+ " "+str(m) +" "+str(m2))

# %%
df = pd.read_csv("datap2.csv")

#df = df.groupby('num_threads').min('time(ns)')
#print(df.head)

import matplotlib.pyplot as plt
ax = plt.axes()
ax.scatter(df.index,df['time (ns)'])
ax.plot(df.index,df['time (ns)'])

z = np.polyfit(df.index, df['time (ns)'], 2)
p = np.poly1d(z)
ax.plot(df.index, p(df.index))

ax.set_xlabel('No. of Threads')
ax.set_ylabel('Time')
ax.set_title("P2 execution time vs No of threads for dim "+str(n)+ " "+str(m) +" "+str(m2))

# %%
