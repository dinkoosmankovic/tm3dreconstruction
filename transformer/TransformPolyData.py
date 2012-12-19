#!/usr/bin/env python

import sys
import vtk
from vtk import *


def WritePointsToFile(file_name, source):
    f = open(file_name, "w")
  
    pointCount = source.GetNumberOfPoints()
    for index in range(pointCount):
        point = [0,0,0]
        source.GetPoint(index, point)
        s = str(point[0])+" "+str(point[1])+" "+str(point[2])+"\n"
        f.write(s)
    f.close()

def ReadTransformationMatrix(file_name):
	f = open(file_name, "r")
	array = f.readlines()[-1]
	list_array = array.split(" ")
	transformation_matrix = vtkMatrix4x4()
	elements = [float(i) for i in list_array]
	elements = elements[0:16]
	#print elements
	k = 0
	for i in range(0,4):
		for j in range(0,4):
			transformation_matrix.SetElement(i,j,elements[k])
			k += 1  
	return transformation_matrix
	
def ReadTransformationMatrix2(file_name):
	f = open(file_name, "r")
	array = f.readlines()[-1]
	list_array = array.split(" ")
	print list_array[0:16]
	transformation_matrix = vtkMatrix4x4()
	elements = [float(i) for i in list_array[0:16]]
	
	r11 = elements[0]
	r12 = elements[1]
	r13 = elements[2]
	
	r21 = elements[4]
	r22 = elements[5]
	r23 = elements[6]
	
	r31 = elements[8]
	r32 = elements[9]
	r33 = elements[10]
	
	R = [r11,r12,-r13,r21,r22,-r23,r31,r32,-r33]
	
	# Rotation matrix formated for right handed coordinate system
	
	k = 0
	for i in range(0,3):
		for j in range(0,3):
			transformation_matrix.SetElement(i,j,R[k])
			k += 1
	
	tx = elements[12]
	ty = elements[13]
	tz = elements[14]
	
	return transformation_matrix,tx,ty,tz

# ============ create source points ==============
print "Usage: \n\n# ./TransformPolyData.py input output"
print "input file name should be scanXXX.vtp with scanXXX.frames"
print "Importing First Scan"

reader1 = vtk.vtkXMLPolyDataReader()
reader1.SetFileName(sys.argv[1])
reader1.Update()

print "Points read from the 1. file\n"
 
temp_file_name = sys.argv[1]
#print temp_file_name.split(".")
path = temp_file_name.split(".")

file_name = path[-2]
file_name = file_name[0:]

print file_name
#transformation_matrix = ReadTransformationMatrix2(file_name+".frames")
R,tx,ty,tz = ReadTransformationMatrix2(file_name+".frames")
#print transformation_matrix

transform_polydata = vtkTransform()
transform_polydata.PostMultiply()
transform_polydata.Identity()
#transform_polydata.Translate(tx,ty,tz)
transform_polydata.SetMatrix(R)

translate_polydata = vtkTransform()
translate_polydata.Translate(tx,-ty,-tz)

#print transform_polydata.GetMatrix()
#print translate_polydata.GetMatrix()

icpTransformFilter1 = vtk.vtkTransformPolyDataFilter()
icpTransformFilter1.SetInput(reader1.GetOutput())
icpTransformFilter1.SetTransform(transform_polydata)
icpTransformFilter1.Update()

icpTransformFilter2 = vtk.vtkTransformPolyDataFilter()
icpTransformFilter2.SetInput(icpTransformFilter1.GetOutput())
icpTransformFilter2.SetTransform(translate_polydata)
icpTransformFilter2.Update()

 
#transformedSource = icpTransformFilter.GetOutput()
#WritePointsToFile(file_name + ".transformed", transformedSource)

writer = vtk.vtkXMLPolyDataWriter()
writer.SetInputConnection(icpTransformFilter2.GetOutputPort())
writer.SetFileName(sys.argv[2])
writer.Write()




