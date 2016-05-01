#!/usr/bin/python3

lines = open('lr-train','r').readlines()
model = open('lr-model','r').readlines()

means = [
sum([float(line.split()[i]) for line in lines]) / len(lines)
for i in range(len(lines[0].split()))
]

coef = list(map(float, model[1].split()))
inter = float(model[2])
k = coef + [inter]

for i in range(len(k)):
    print('%3d %10.7lf %10.7lf      %10.7lf' % (i+1, k[i], means[i], k[i]*means[i]))
