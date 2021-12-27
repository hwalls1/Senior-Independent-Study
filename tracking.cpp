//
//  main.cpp
//  object 2.0
//
//  Created by Harrison Walls on 10/15/18.
//  Copyright © 2018 Harrison Walls. All rights reserved.
//
// OpenCV Version 3.4.3

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

using namespace std;
using namespace cv;

//Tracks a yellow ball using color seperation
/* Hue values:
 Yellow: 21-30
 Blue: 75-130
 Violet: 130-160
 Orange: 0-21
 Green: 38-75
 Red: 160-179
 */

int main() {

    VideoCapture capWebcam(0);

    // Checks is webcam is activated if not output an error message
    if (capWebcam.isOpened() == false)
    {
        cout << "error: Webcam connect unsuccessful\n";
        return(0);
    }

    // Input image
    Mat imgOriginal;
    // HSV Image
    Mat hsvImg;
    // Thresh Image
    Mat threshImg;

    vector<Vec3f> v3fCircles;

    char CheckForEscKey = 0;

    // Set Hue
    int lowH = 75;
    int highH = 130;

    // Set Saturation
    int lowS = 200;
    int highS = 255;

    // Set Value
    int lowV = 102;
    int highV = 225;

    // Tracks until esc key is pressed or webcam is not open
    while (CheckForEscKey != 27 && capWebcam.isOpened()) {
        bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);
        if (!blnFrameReadSuccessfully || imgOriginal.empty()) {
            cout << "error: frame can't read";
            break;
        }

        // Convert Original Image to HSV Thresh Image
        cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);

        inRange(hsvImg, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), threshImg);

        //Blur Effect
        GaussianBlur(threshImg, threshImg, Size(3, 3), 0);
        // Dilate Filter Effect
        dilate(threshImg, threshImg, 0);
        // Erode Filter Effect
        erode(threshImg, threshImg, 0);

        // algorithm for detecting circles
        HoughCircles(threshImg,v3fCircles,CV_HOUGH_GRADIENT,2,threshImg.rows / 4,100,50,10,800);

        // Draws a red circle around ball
        for (int i = 0; i < v3fCircles.size(); i++) {

            // draw red circle around object detected
            circle(imgOriginal,
                   Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),(int)v3fCircles[i][2], 3);
        }

        // Names windows
        namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
        namedWindow("threshImg", CV_WINDOW_AUTOSIZE);

        // Creates thresh hold window
        //Hue (0 - 179)
        createTrackbar("LowH", "threshImg", &lowH, 179);
        createTrackbar("HighH", "threshImg", &highH, 179);

        //Saturation (0 - 255)
        createTrackbar("LowS", "threshImg", &lowS, 255);
        createTrackbar("HighS", "threshImg", &highS, 255);

        //Value (0 - 255)
        createTrackbar("LowV", "threshImg", &lowV, 255);
        createTrackbar("HighV", "threshImg", &highV, 255);

        // show windows
        imshow("imgOriginal", imgOriginal);
        imshow("threshImg", threshImg);

        CheckForEscKey = waitKey(1);
    }

    return(0);
}