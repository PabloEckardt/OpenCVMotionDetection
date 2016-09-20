## Pablo Eckardt

## Open Source Computer Vision (OpenCV) based motion tracking.

## How to run: 
Note

This project repo is setup to run in linux with no future intentions of using it on windows.
The reason why is that this code is meant to be run in device like the raspberry pie.

OpenCv is a somewhat picky when it comes to building the code in Linux, but its fully compatible.
For that reason I have included a CMake to properly link the libraries.

[Additionally OpenCv needs to be installed in your computer before you run this program.](http://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html) Click the link to see the instructions.

How to run it:

Take the files to a directory where you want to build the program. 

- In your terminal type:

'cmake [/path/to/folder/]'

CMake will generate a makefile for you.

- Then build it using the make file.

'make'

- Run the program: 

'./motionTracking'

## The program explained.
[All the API for image processing functions:](http://docs.opencv.org/2.4/modules/imgproc/doc/imgproc.html)

This technique is a series of functions that take two images finds a difference and process the result into 
information we can compute. 

- Capture image 

- Turn it into a grey image to simplify the difference function

- Capture a second frame and do the same.

- Use **cv::absdiff** to find the differences between the two images. (whatever has moved in the video stream) and put the difference on a third one.

- Use **cv::threshold** to delete the noise and be left with with a simpler object in a black background. A higher sensitivity means that more and smaller changes get filtered out. A sensitivity that is too high will not
capture small details. Too low, and the program will be overwhelemed with small insignificant changes and vibrations on the camera. Blurry webcams require a high threshold. 

- Optional: use **cv::blur** and then **cv::threshold** again to eliminate granularity and small changes in environmental light.

- Find all the contours and put them in a sorted vector that is sorted by the size of the contour. These are the points that you just detected changing.

- Display or process data.  At this point every contour will have cartesian coordinates, and can be easily accessed since they are in a **Point** object.


## Demo: 

Note that this is not particularily great to track a face since a face has many points in which shadows change dramatically. In the future when I implement 100 + tracking points it will do a lot better.

(Me nodding my head)

![alt tag](https://github.com/PabloEckardt/OpenCVMotionDetection/blob/master/demo.png)

## To do:

- [x] make deliverable version
- [ ] Accept keyboard inputs to toggle threshold, grey, debugging windows.
- [ ] Dynamically create target points. (currently limited at 4).
- [ ] Implement dynamic threshold.
- [ ] Implement multi-threading. 






