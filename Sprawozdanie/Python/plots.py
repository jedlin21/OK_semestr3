#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Nov 15 20:57:07 2018

@author: i
"""

import numpy as np
import glob
import matplotlib.pyplot as plt


def importData(fileName):
    my_data = np.genfromtxt(fileName, delimiter=';')
    return my_data


def strojeniePlot(strojenieData = './strojenie_test.txt'):    
    plotableData = importData(strojenieData)
    
    plotableStrojenie = dict()
    for x in range(1, 11):
        plotableStrojenie[x] = dict()
        for y in [250,500,750,1000]:
            plotableStrojenie[x][y] = list()
        
    for row in plotableData:
        plotableStrojenie[int(row[0])][int(row[1])].append(row[3])
        
    for x in range(1, 11):
        for y in [250,500,750,1000]:
            plotableStrojenie[x][y] =  np.mean(plotableStrojenie[x][y])
            
    X = range(1,11)
    Y = dict()
    for instances in [250,500,750,1000]:
        Y[instances] = list()
    for rangeValue in plotableStrojenie:        
        for instances in [250,500,750,1000]:
            Y[instances].append(plotableStrojenie[rangeValue][instances])
            
    for key in Y.keys():
        plt.title("Seeking for GRASP list size", fontsize=18)
        plt.xlabel("GRASP list size", fontsize=17)
        plt.ylabel("Distance", fontsize=17)
        plt.plot(X, Y[key], ".-")
    plt.legend(("250","500","750","1000"), loc='best', title="Instances")
    plt.show()


def TrucksPlot(trucskDirec = './Trucks.txt'):
    plotableData = importData(trucskDirec)
    
    Instances = list()
    XTrucks = list()
    XDistance = list()
    for row in range(len(plotableData)):
        Instances.append(int(plotableData[row][0]))
        XTrucks.append(int(plotableData[row][1]))
        XDistance.append(plotableData[row][2])
    
    plt.title("Distances", fontsize=18)
    plt.xlabel("Number of instances", fontsize=17)
    plt.ylabel("Distance", fontsize=17)
    plt.plot(Instances, XDistance , ".-")
    plt.show()
    
    plt.title("Truck number", fontsize=18)
    plt.xlabel("Number of instances", fontsize=17)
    plt.ylabel("Number of trucks", fontsize=17)
    plt.yticks(range(0,22, 2))
    plt.plot(Instances, XTrucks , ".-")
    plt.show()
    
    
def TimePlot(timeDirec = './time_test.txt'):
    plotableData = importData(timeDirec)
    
    instances = list()
    time = list()
    
    #Make time dictionary
    timeDict = dict()
    for row in plotableData:
        timeDict[int(row[0])] = list()
    #Add values to timeDict
    for row in plotableData:
        timeDict[int(row[0])].append(row[1])
    #Average
    
    for key in timeDict.keys():
        instances.append(key)
        time.append(np.mean(timeDict[key]))
    
    plt.title("Time", fontsize=18)
    plt.xlabel("Number of instances", fontsize=17)
    plt.ylabel("Time [s]", fontsize=17)
    plt.plot(instances, time , ".-")
    plt.show()


if __name__ == '__main__':
    strojeniePlot()
    TrucksPlot()
    TimePlot()
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
            