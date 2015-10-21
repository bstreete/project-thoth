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
	# checkArgs

	# Get the base filename
	imageName=${imageName%.fits}

	# build decision forest if requested - Assumes that the images are already converted
	# input directory with pgm = original ppm = overlay
	
	##### Need some training images to pass before I can test

	# if [ ! -z $training ] ; then
	# 	buildForest
	# fi

	# Convert from fits to pgm for TextureCam -- put script in scripts directory for gimp?
	gimp -i -b convertFitsPgm $imageName.fits $imageName.pgm

	# classify the image features using TextureCam
	call "src/texturecam/tcclass $decision $imageName.pgm $imageName.ppm"

	# Run it through the blob detector to get the usable area  
	call "src/BlobDetector/BlobDetector -i $imageName.ppm -o ${imageName}_out.png -r $radius"
}

function buildForest {
	local string="src/texturecam/tctrain -o $decision"

	local images=$(ls -v ${training}*.pgm)
	echo images

	# If the directory is empty, quit
	if [ -z images ] ; then
		error "No .PGM images were found to classify. " "1"
	fi 

	# Break the string into an array of images to analyze
	set images
	for image in $images ; do 
		string="$string $image ${image%.pgm}.ppm"
	done

	echo $string
	read
}

# Handles the different possible command line arguments
function readArgs { 
	while getopts "i:r:d:t:h" o ; do
		case "${o}" in 
			i)	# Input image filename
				imageName=${OPTARG}
				;;
			r) 	# Safezone required radius
				radius=${OPTARG}
				;;
			d)	# Decision forest name
				decision=${OPTARG}
				;;
			t)	# Decision forest training set
				training=${OPTARG}
				;;
			h) 	# Help
				usage "0"
				;;
			*)	# Unrecognized option, print usage and exit
				usage "1"
				;;
		esac	# End of getopts cases
	done	# End of getopts loop
}

# Double checks the passed arguments to ensure they make sense or were set
function checkArgs { 

	if [ -z $imageName ] ; then
		error "No image specified." "1"
	fi 
	# elif [ ! -f $imageName ] ; then
	# 	error "Invalid image filename provided: $imageName." "1"
	# fi

	if [ -z $decision ] ; then
		error "No decision forest provided." "1"
	elif [ ! -f $decision ] ; then
		if [ -z $training ] ; then
			error "Invalid decision forest provided. Verify the path name." "1"
		fi
	fi 

	if [ $radius -le 0 ] ; then
		error "Invalid radius provided: $radius. Needs to be larger than 0." "1"
	fi
}

# Prints the usage/help information, then exits using the passed exit code
function usage {
	# Text between USAGE will be printed as it appears
	cat << USAGE 
usage: $0 -i filename -d filename [-t directory | -r radius]

    -i filename     Specifies the input image in .pgm format.

    -r radius       Specifies the required radius for the safezone. 
                    Expected to be greater than 0. Defaults to 10.

    -d filename     Specifies the decision forest filename created by 
                    TextureCam. Mandatory argument. 

    -t directory    Specifies the training image directory for creating
                    a new TextureCam decision forest. Optional. 

    -h              Prints the help/usage statement.
USAGE

	exit $1
}

# Prints $1 to stderr, then exits with $2
function error {
	echo >&2 $1
	exit $2
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