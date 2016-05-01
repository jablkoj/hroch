#!/usr/bin/python

from random import randint, choice
from math import exp
import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import PolynomialFeatures
from sys import stdin

lines = open('lr-train','r').readlines()
model = open('lr-model','r').readlines()

coef = map(float, model[1].split())
inter = float(model[2])

o = randint(0,len(lines)-50)

for i in range(50):
    line = map(float, choice(lines).split())
    line = map(float, lines[o+i].split())
    x = line[:-1]
    y = line[-1]
    value = sum([coef[j]*x[j] for j in range(len(coef))])+inter
    prob = 1./(1.+exp(-value))
    print y, value, prob
