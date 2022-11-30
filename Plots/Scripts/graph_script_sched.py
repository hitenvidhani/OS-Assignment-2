# %%
import subprocess
import pandas as pd
import numpy as np
import csv


# %%
def matGen(n, m, m2):

    LONG_LONG_MAX =  9223372036854775807

    #change values here!
    # n = 50
    # m = 50
    # m2 = 50

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
subprocess.call(["gcc", "group21_assignment2.c", "-o","sched.out"])


# subprocess.call("gcc schedulercopy.c")

f1 = open('datasched.csv', 'w')

writer1 = csv.writer(f1)

header1 = ['dim1', 'dim2', 'dim3', 'TAT(ns)', 'wait(ns)', 'switching_ovhd(ns)']

writer1.writerow(header1)

f1.close()

# %%
i = j = k = 1500
matGen(i, j, k)

# %%

execstring = "./sched.out " + str(i) + " " + str(j) + " " + str(k) + " matrix1.txt matrix2.txt out.txt"
tmp = subprocess.call(execstring.split())

# %%
for i in range(1, 1000, 10):
    for j in range(1, 1000, 10):
        for k in range(1, 1000, 10):
            matGen(i, j, k)
            execstring = "./sched.out " + str(i) + " " + str(j) + " " + str(k) + " matrix1.txt matrix2.txt out.txt 8 8"
            tmp = subprocess.call(execstring.split())


# %%
# for i in range(1, 50):
#     for j in range(1,6):
#         p1 = "./P1.out "+str(n)+" "+str(m)+" "+str(m2)+" "+"matrix1.txt matrix2.txt out.txt "+ str(i) +" 1"
#         tmp=subprocess.call(p1.split()) 
#         print("printing result")
#         #print(tmp)
#         p2 = "./P2.out "+str(n)+" "+str(m)+" "+str(m2)+" "+"matrix1.txt matrix2.txt out.txt 1 "+ str(i)
#         tmp=subprocess.call(p2.split()) 
#         print("printing result")
#         #print(tmp)

# # #dataP1.txt

# %%
df = pd.read_csv("datasched.csv")

# df = df.groupby('num_threads').mean('time(ns)')
print(df.head)
