#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Nov  3 21:42:34 2018

@author: i
"""

vector = [
[0   ,  0   ,     0 ,         0 ,       0      ,     100    ,      0],
 [   1    , 0    ,    10  ,       10   ,    200   ,      300   ,      10],
  [  2   ,  0     ,   20 ,        10    ,   10   ,       120    ,     10],
   [   3    , 0    ,    15  ,       10   ,    20   ,      300   ,      10],
  [  4   ,  0     ,   17 ,        10    ,   10   ,       120    ,     10],
   [   5    , 0    ,    50  ,       10   ,    10   ,      300   ,      10],
  [  6   ,  0     ,   60 ,        10    ,   10   ,       120    ,     10],
   [ 7  ,   0      ,  30,         10     ,  20  ,        180     ,    10]]

capacity = 30
xMagazine = 0
yMagazine = 0

tracks = list()
def addTrack(tracksList):
    tracksList.append([[], ])
    

for shop in vector:
    shop.append(0)

if __name__ == '__main__':
    #iterate through all shops
    while True:
        #check if there is any shop free 
        allServedFlag = True
        for shop in vector:
            if shop[7] == 0:
                allServedFlag = False
        if allServedFlag:
            break
            
        for shop in vector[1:]:
            if 