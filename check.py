f = open("./cmake-build-debug/set.in", "r")
fout = open("./cmake-build-debug/setpy.out", "w")
s = set()
for line in f:
    com, op = line.split(" ")
    op = int(op)

    if com[0] == 'a':
        s.add(op)
    elif com[0] == 'e':
        fout.write (str(int(op in s)) + '\n')
    elif com[0] == 'r':
        fout.write( str(int(op in s)) + '\n')
        s.discard(op)
fout.close()
