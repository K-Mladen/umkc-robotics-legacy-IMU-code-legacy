
#model_orientation.py
#modeling orientation of our object in real time, given current orientation
#code for reading/displaying model taken from Brian Wolfe
#http://diydrones.ning.com/video/pic32-autopilot-attitude


#follow function
#follow a file like tail -f
#taken from David M. Beazley
#http://www.dabeaz.com/generators/
#http://www.dabeaz.com/generators/follow.py

import time #follow function
from visual import *

def follow(thefile):
    thefile.seek(0,2)   #seek relative to file's end, offset 0
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.1)
            continue
        yield line

if __name__ == '__main__':
    
    scene.title = "Phidget"
    scene.forward = (-10,-8,-4)
    scene.range=(1.5,1.5,1.5)
    distant_light()

    arrow( color=color.red, axis=(1,0,0), shaftwidth = 0.02, fixedwidth =1)
    arrow( color=color.green, axis=(0,1,0), shaftwidth = 0.02, fixedwidth =1)
    arrow( color=color.blue, axis=(0,0,1), shaftwidth = 0.02, fixedwidth =1)
    thing = box(length=.5, height = .05, width = .4, color= color.yellow)

    logfile = open("dataPoints/rotation_matrix.csv", "r")
    loglines = follow(logfile)
    rotMatrix = [[0,0,0],[0,0,0],[0,0,0]]
    for line in loglines:
        elems = line.strip().split(",")
        print elems
        index = 0
        for i in range(3):
            for k in range(3):
                rotMatrix[i][k] = float(elems[index])
                index = index +1
            
       
        #print rotMatrix
        for i in range(3):
            for k in range(3):
                print rotMatrix[i][k], " ",
            print 
        print "------------------"

        axis = (rotMatrix[0][0], -rotMatrix[1][0], -rotMatrix[2][0])
        up = (rotMatrix[0][2], -rotMatrix[1][2], -rotMatrix[2][2])

        thing.axis = axis
        thing.up = up

                


    
