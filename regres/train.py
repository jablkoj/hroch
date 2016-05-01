#!/usr/bin/python

from random import randint, shuffle
from math import exp
import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import PolynomialFeatures
from sys import stdin

lines = open('lr-train','r').readlines()
output = open('lr-model','w')

X = []
y = []
A, B = 0, -1

shuffle(lines)

for line in lines[:-10000]:
    data = map(float, line.split())
    X.append(data[A:B])
    y.append(data[-1])

model = LogisticRegression(tol=0.0001)
model = model.fit(X,y)

coef = model.coef_[0]

print >>output, len(coef)
print >>output, ' '.join(map(str,coef))
print >>output, model.intercept_[0]


print(model.score(X,y))
print(sum(y)/float(len(y)))
print(coef)
print(model.intercept_)

predict = model.predict_proba(X)
samew = sum([(y[i]-predict[i][1])**2 for i in range(len(y))])
same = sum([(y[i]==(predict[i][1]>0.5)) for i in range(len(y))])
print(1.*samew/len(y), 1.*same/len(y))

X = []
y = []
for line in lines[-10000:]:
    data = map(float, line.split())
    X.append(data[A:B])
    y.append(data[-1])

predict = model.predict_proba(X)
samew = sum([(y[i]-predict[i][1])**2 for i in range(len(y))])
same = sum([(y[i]==(predict[i][1]>0.5)) for i in range(len(y))])
print(1.*samew/len(y), 1.*same/len(y))
