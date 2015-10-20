#!/bin/bash

# This script will: 
# take the image name of a single .fits image
# convert it to a .pgm image
# use it as input for the texturecam program 
# run a blob detection program on the .ppm output of texture cam
# out puts the usable safe area of the image 
# saves a color image with the safe areas identified

# Have to dump the scripts into a gimp script directory
# Assumes textureCam is compiled in ./src/texturecam
# Assumes Gimp is installed
# Assumes OpenCV 3.0.0 is installed

# Set the default inputs
radius=10

function main {

	# Pass the arguments into the readArgs function
	
	readArgs "$@"
	checkArgs

	# Get the base filename
	imageName=${imageName%.fits}

	# build decision forest if requested - Assumes that the images are already converted
	# input directory with pgm = original ppm = overlay

	# otherwise, use the specified decision tree 

	# Convert from fits to pgm for TextureCam -- put script in scripts directory for gimp?
	# gimp -i -b convertFitsPgm $imageName.fits $imageName.pgm

	# classify the image features using TextureCam
	call "src/texturecam/tcclass rocks.rf $imageName.pgm $imageName.ppm"

	# Run it through the blob detector to get the usable area  
	call "src/BlobDetector/BlobDetector -i $imageName.ppm -o ${imageName}_out.png -r $radius"
}

# Handles the different possible command line arguments
function readArgs { 
	while getopts "i:r:h" o ; do
		case "${o}" in 
			i)	# Input image filename
				imageName=${OPTARG}
				;;
			r) 	# Safezone required radius
				radius=${OPTARG}
				;;
			h) 	# Help
				usage
				exit 0
				;;
			*)	# Unrecognized option, print usage and exit
				usage
				exit 1
				;;
		esac	# End of getopts cases
	done	# End of getopts loop
}

# Double checks the passed arguments to ensure they make sense or were set
function checkArgs { 

	if [ -z $imageName ] ; then
		echo "No filename provided."
		usage
		exit 0
	fi 

	# if [ ! -f $imageName ] ; then
	# 	echo "Invalid image filename provided: $imageName."
	# 	exit 1
	# fi

	if [ $radius -le 0 ] ; then
		echo "Invalid radius provided: $radius. Needs to be larger than 0."
		exit 1
	fi
}

# Prints the usage/help information
function usage {
	# Text between USAGE will be printed as it appears
	cat << USAGE 
usage: $0 -i filename -r radius

	-i filename	Specifies the input image in .pgm format.

	-r radius	Specifies the required radius for the safezone. 
			Expected to be greater than 0. Defaults to 10.

	-h 		Prints the help/usage statement.
USAGE
}

# Wrapper for calling functions that will catch errors and exit 
function call {
	$@

	if [ $? -ne 0 ] ; then
		exit 1
	fi
}

main "$@"
exit 0