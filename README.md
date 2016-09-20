## Pablo Eckardt

## Open Source Computer Vision (OpenCV) based motion tracking.

## How to run: 
note{

This project repo is setup to run in linux with no future intentions of porting to windows.
The reason why is that this code is meant to be run in device like raspberry pies.

OpenCv is a little tricky when it comes to building the code in Linux but its fully compatible.
It requires a CMake to properly link the libraries.

[Additionally OpenCv needs to be installed in your computer before you run this program.](http://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html)
}

Take the files to a directory where you want to build the program. 

- in your terminal type cmake [/path/to/folder/]

CMake will generate a makefile for you.

- type make in the same folder.

run

- ./motionTracking

## The program explained.
[All the API for image processing functions:](http://docs.opencv.org/2.4/modules/imgproc/doc/imgproc.html)

This technique is a series of functions that take two images finds a difference and process the result into 
information we can compute. 

-  Capture image 

-  turn it into a grey image to simplify a difference function

- capture a second frame and do the same.

- use **cv::absdiff** to find the differences between the two images. (whatever has moved in the video stream)

- use **cv::threshold** to delete the noise and remain with a simpler object in a black background. A higher sensitivity means that more and smaller changes get filtered out. A sensitivity that is too high will not
capture small details. Too low and the program will be overwhelemed with small insignificant changes and vibrations on the camera. Blurry webcams require a high threshold. 

- Optional use **cv::blur** and then **cv::threshold** To eliminate granularity and slight changes in light.

- Find all the contours and put them in a sorted vector. Sorted by their size. These are the points that detected change.

- display or process data.


## Demo: 

This is not particularily great to track a face since a face has many points in which shadows change dramatically

(me nodding my head)

![alt tag](https://github.com/PabloEckardt/OpenCVMotionDetection/blob/master/demo.png)






