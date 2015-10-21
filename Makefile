# User installs OpenCV 3.0+ and the GIMP
# Makefile will download and make TextureCam and the BlobDetector

# Makefile will copy the conversion scripts into $HOME/opt/local/share/gimp/2.0/scripts
	# Mac uses $HOME/opt/local/share/gimp/2.0/scripts
	# Linux uses $HOME/.gimp-version/scripts

all: textureCam blobDetector scripts

# Downloads and compiles texturecam in /src/texturecam
textureCam: 
	cd src
	wget https://github.com/davidraythompson/texturecam/archive/master.zip
	unzip master.zip
	mv texturecam-master texturecam
	cd texturecam 
	make
	cd ..

# Compiles the blob detector in src/BlobDetector
blobDetector:
	cd BlobDetector
	cmake .
	make
	cd ../

# Copies the image conversion script into the appropriate gimp location
scripts: 
	# This is taken from Dr. Hartman's makefile in OS that handled Mac/Linux
	ifeq ($(shell uname),Darwin)	# Mac
		dir=$HOME/opt/local/share/gimp/*/scripts
		cp convertFitsPgm.scm $dir
	else	# Linux
		dir=$HOME/.gimp*/scripts
		cp convertFitsPgm.scm $dir
	endif
