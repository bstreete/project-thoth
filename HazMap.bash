#!/bin/bash

# This script will: 
# take the image name of a single .fits image
# convert it to a .pgm image
# use it as input for the texturecam program 
# run a blob detection program on the .ppm output of texture cam
# out puts the usable safe area of the image 
# saves a color image with the safe areas identified

# Assumes textureCam is installed 
# Assumes the image is in this folder
# Assumes openCV is installed 


echo Enter image name
read imageName

# convert image to pgm 

# classify the image features using TextureCam
./tcclass rocks.rf $imageName.pgm $imageName.ppm

  
