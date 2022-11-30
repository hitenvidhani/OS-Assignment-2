import subprocess 
subprocess.call(["gcc", "P1.c", "-o","P1.out", "-pthread"])
subprocess.call(["gcc", "P2.c", "-o","P2.out", "-pthread"])
# subprocess.call("gcc schedulercopy.c")
for i in range(1,50):
    for j in range(1,5):
        p1 = "./P1.out 50 50 50 in1.txt in2.txt out.txt "+ str(i) +" 1"
        tmp=subprocess.call(p1.split()) 
        print("printing result")
        print(tmp)
        p2 = "./P2.out 50 50 50 in1.txt in2.txt out.txt 1 1"
        tmp=subprocess.call(p2.split()) 
        print("printing result")
        print(tmp)