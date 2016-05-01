n = 1
for i in range(16):
    print('base ', i, n)
    n+=1
for i in range(16):
    print('exp', i, n)
    print('exp i-0', i, n+1)
    print('i/0', i, n+2)
    print('i/1', i, n+3)
    print('i/2', i, n+4)
    print('i/3', i, n+5)
    print('i*0', i, n+6)
    print('i*1', i, n+7)
    print('i*2', i, n+8)
    print('i*3', i, n+9)
    n+=10	
