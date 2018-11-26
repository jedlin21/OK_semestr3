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
        plt.title("Strojenie parametru 'Długość listy GRASP'", fontsize=18)
        plt.xlabel("Długość listy GRASP [n]", fontsize=17)
        plt.ylabel("Dystans", fontsize=17)
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
    
    plt.title("Zależność dystansu od liczby instancji", fontsize=18)
    plt.xlabel("Liczba instancji [n]", fontsize=17)
    plt.ylabel("Dystans", fontsize=17)
    plt.plot(Instances, XDistance , ".-")
    plt.show()
    
    plt.title("Zależność liczby użytych ciężarówek od liczby instancji", fontsize=18)
    plt.xlabel("Liczba instancji [n]", fontsize=17)
    plt.ylabel("Liczba ciężarówek", fontsize=17)
    plt.yticks(range(0,22, 2))
    plt.plot(Instances, XTrucks , ".-")
    plt.show()

def TrucksComparePlot(trucskDirec = './Trucks.txt',trucskRandDirec = './randfilenumber.txt'):
    plotableData = importData(trucskDirec)
    
    Instances = list()
    XTrucks = list()
    XDistance = list()
    for row in range(len(plotableData)):
        Instances.append(int(plotableData[row][0]))
        XTrucks.append(int(plotableData[row][1]))
        XDistance.append(plotableData[row][2])
    
    plotableDataR = importData(trucskRandDirec)
    XTrucksR = list()
    XDistanceR = list()
    for row in range(len(plotableDataR)):
        XTrucksR.append(int(plotableDataR[row][1]))
        XDistanceR.append(plotableDataR[row][2])
    
    plt.title("Zależność dystansu od liczby instancji", fontsize=18)
    plt.xlabel("Liczba instancji [n]", fontsize=17)
    plt.ylabel("Dystans", fontsize=17)
    plt.plot(Instances, XDistance , ".-")
    plt.plot(Instances, XDistanceR , ".-")
    plt.legend(("GRASP","Random"), loc='best') 
    plt.show()
    
    plt.title("Zależność liczby użytych ciężarówek od liczby instancji", fontsize=18)
    plt.xlabel("Liczba instancji [n]", fontsize=17)
    plt.ylabel("Liczba ciężarówek", fontsize=17)
    plt.yticks(range(0,100, 10))
    plt.plot(Instances, XTrucks , ".-")
    plt.plot(Instances, XTrucksR , ".-")
    plt.legend(("GRASP","Random"), loc='best') 
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
    
    plt.title("Zależność czasu działania od liczby instancji", fontsize=18)
    plt.xlabel("Liczba instancji [n]", fontsize=17)
    plt.ylabel("Czas [s]", fontsize=17)
    plt.plot(instances, time , ".-")
    plt.show()
    
    combined = list()
    for i in range(len(time)):
        combined.append([instances[i], time[i]])
    np.savetxt("time.csv", combined, delimiter=";")

def TimePlotCompare(timeDirec = './time_test.txt', timeDirecR = 'timerand.txt'):
    plotableData = importData(timeDirec)
    plotableDataR = importData(timeDirecR)
    
    instances = list()
    time = list()
    timeR = list()
    
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
    
        
             #Make time dictionary RADNOM
    timeDictR = dict()
    for row in plotableDataR:
        timeDictR[int(row[0])] = list()
    #Add values to timeDict
    for row in plotableDataR:
        timeDictR[int(row[0])].append(row[1])
        
    for key in timeDictR.keys():
        timeR.append(np.mean(timeDictR[key]))
    #Average
    
    plt.title("Zależność czasu działania od liczby instancji", fontsize=18)
    plt.xlabel("Liczba instancji [n]", fontsize=17)
    plt.ylabel("Czas [s]", fontsize=17)
    plt.plot(instances, time , ".-")
    plt.plot(instances, timeR , ".-")
    plt.legend(("GRASP","Random"), loc='best')
    plt.show()

if __name__ == '__main__':
    strojeniePlot()
    TrucksComparePlot("./filesumtime.txt")
    TimePlot("timerand.txt")
    TimePlotCompare()
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
            