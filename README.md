# project-thoth
Automated terrain feature detection for the [OSIRIS-REx NASA mission](http://www.asteroidmission.org/).

For up to date information, please visit the [project wiki](https://github.com/bstreete/project-thoth/wiki).
### System Requirements

This software is intended for a Unix like environment that has a bash shell. It was developed and tested on Ubuntu and Mac OSX machines. It is expected to run correctly in similar environments. 

#### Software Dependencies

[GIMP](www.gimp.org) - The GNU Image Manipulation Program version X.X.X or later is needed for converting the inputs from FITS format. GIMP requires a handful of other software that is listed on its website. 

[TextureCam](https://github.com/davidraythompson/texturecam) - TextureCam is required to properly classify the terrain in the images. TextureCam is self contained and only needs to be downloaded and compiled to run. 

[OpenCV](opencv.org) - OpenCV version 3.0.0 or later is required to calculate the size of safe sampling areas. OpenCV relies on a number of additional software libraries that are detailed on its installation page.  

C++ - This project uses the C++11 standard. 

#### Project Members

Courtney Akin

Carina Johnson

Justin Odle

Brayden Streeter
