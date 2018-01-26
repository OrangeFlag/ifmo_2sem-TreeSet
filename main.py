f = open("./cmake-build-debug/set.in", "w")
from random import randint
import string
mas_key = []
for i in range(40):
    x = str(randint(0, 10**9))
    mas_key.append(x)
command = ["a", "r", "e"]
for i in range(2000000):
    com = command[randint(0,len(command)-1)]
    com += " " +mas_key[randint(0, len(mas_key)-1)]
    f.write(com+"\n");
f.close()
