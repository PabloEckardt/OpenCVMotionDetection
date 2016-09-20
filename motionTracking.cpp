
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
using namespace cv;



//to_string is buggy in c++11
template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

//bounding rectangle of the object, we will use the center of this as its position.
Rect boundingBox = Rect(0,0,0,0);


//int to string helper function


void scan(Mat thresholdImage, Mat &cameraFeed){
    //notice how we use the '&' operator for the cameraFeed. This is because we wish
    //to take the values passed into the function and manipulate them, rather than just working with a copy.
    bool detected=false;
    Mat temp;
    thresholdImage.copyTo(temp);
    vector< vector<Point> > contours;
    // vec4i is a data strcture called vec, that contains 4 integers.
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    //findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
    // the last two vectors are necessary to call findcountours as defined here:
    //http://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=findcontours#findcontours
    findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );
    if(contours.size()>0)detected=true;
    else detected = false;
    // the largest detected vectors are put at the end of the vector
    int x, y;
    if(detected){
        int numContours = contours.size();

        vector<Point> largestContourAt = contours.at(numContours - 1);
        boundingBox = boundingRect(largestContourAt);
        x = boundingBox.x+boundingBox.width/2;
        y = boundingBox.y+boundingBox.height/2;
        circle(cameraFeed,Point(x,y),10,Scalar(0,255,0),2);
        putText(cameraFeed," Point : (" + to_string(x)+","+to_string(y)+")",Point(x,y),1,1,Scalar(255,255,255),1);
        //ideally we want as many tracking points. But so far this implementation
        //doesn't have dynamic allocation yet.

        if (numContours>1){
            vector<Point> secondLargestContour = contours.at(numContours - 2);
            boundingBox = boundingRect(secondLargestContour);
            x = boundingBox.x+boundingBox.width/2;
            y = boundingBox.y+boundingBox.height/2;
            circle(cameraFeed,Point(x,y),10,Scalar(0,255,0),2);
            putText(cameraFeed," Point : (" + to_string(x)+","+to_string(y)+")",Point(x,y),1,1,Scalar(255,255,255),1);
        }
        if (numContours>2){
            vector<Point> thirdLargestContour = contours.at(numContours - 3);
            boundingBox = boundingRect(thirdLargestContour);
            x = boundingBox.x+boundingBox.width/2;
            y = boundingBox.y+boundingBox.height/2;
            circle(cameraFeed,Point(x,y),10,Scalar(0,255,0),2);
            putText(cameraFeed," Point : (" + to_string(x)+","+to_string(y)+")",Point(x,y),1,1,Scalar(255,255,255),1);
        }
        if (numContours>3){
            vector<Point> fourthLargestcontour = contours.at(numContours - 4);
            boundingBox = boundingRect(fourthLargestcontour);
            x = boundingBox.x+boundingBox.width/2;
            y = boundingBox.y+boundingBox.height/2;
            circle(cameraFeed,Point(x,y),10,Scalar(0,255,0),2);
            putText(cameraFeed," Point : (" + to_string(x)+","+to_string(y)+")",Point(x,y),1,1,Scalar(255,255,255),1);
        }
    }


}
int main(){
    bool detected = false;
    bool debugMode = true;
    bool trackingEnabled = true;
    bool pause = false;
    //the two frames we will take from the feed to compare.
    Mat frame1,frame2;
    //frames become greyimages
    Mat grayImage1,grayImage2;
    //greayscale images are scanned for differences resulting in differenceImage
    Mat differenceImage;
    //threshold image will remove the noise by making the image have only 2 tones.
    Mat thresholdImage;
    VideoCapture capture;
    //windows
    namedWindow("Threshold" );

    // this line can be modified to work with video files rather than a camera feed.
    // if the system has several cameras, capture.open(#) would take different
    // feed from different cameras depending on its parameter.
    // look here for more information http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html
    capture.open(0);
    // fail if the driver won't load.
    if(!capture.isOpened()){
        cout<<"Failed to open Default camera.\n";
        getchar();
        return -1;
    }

    while(1){
        //Capture two images and make them black and white.
        capture.read(frame1);
        cv::cvtColor(frame1,grayImage1,COLOR_BGR2GRAY);
        capture.read(frame2);
        cv::cvtColor(frame2,grayImage2,COLOR_BGR2GRAY);
        cv::absdiff(grayImage1,grayImage2,differenceImage);
        /*
        apply threshold function to the difference image to eliminate
        any value that may have subly been caught by cv::absdiff
        the third argument is the sensitivity of the function.
        basically given that this is a binary threshold where everything
        will be black or white anything before the third argument will be given
        a value of 0. Make notice of the third argument because it will most
        likely require tweaking at different levels of light in the scene.

        In my oppinion: the worse the webcam is, the threshold needs to be higher
        and this will probably cause small objects not to be detected.

        A better webcam can afford to use a lower threshold because it will
        less granularity which this program takes as change in the scence.

        A good webcam / video could have as little as 10 threshold and it could
        detect fast objects. Since my laptop has a terrible webca, I'll set it at 40

        */
        cv::threshold(differenceImage,thresholdImage,50,255,THRESH_BINARY);
        cv::blur(thresholdImage,thresholdImage,cv::Size(15,15));
        cv::threshold(differenceImage,thresholdImage,50,255,THRESH_BINARY);

        if(debugMode==true){
            //show the difference image and threshold image
            cv::imshow("grey",grayImage1);
            cv::imshow("Threshold", thresholdImage);
        }else{
            //if not in debug mode, destroy the windows so we don't see them anymore
            cv::destroyWindow("grey");
            cv::destroyWindow("Threshold");
        }
        //use blur() to smooth the image, remove possible noise and
        //increase the size of the object we are trying to track. (Much like dilate and erode)
        //threshold again to obtain binary image from blur output

        //if tracking enabled, search for contours in our thresholded image
        if(trackingEnabled ){
            scan(thresholdImage,frame1);
        }
        //show our captured frame
        imshow("Frame1",frame1);
        //check to see if a button has been pressed.
        //this 10ms delay is necessary for proper operation of this program
        //if removed, frames will not have enough time to referesh and a blank
        //image will appear.
        if(waitKey(10)){
        // no keyboard input handler
        }

}

return 0;

}
