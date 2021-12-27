//
//  main.cpp
//
//  Created by Harrison Walls on 10/23/18.
//  Copyright © 2018 Harrison Walls. All rights reserved.

#include <iostream>
#include <opencv2/opencv.hpp>
#include "person.h"
#include <math.h>
#include <ctime>
#include <vector>

using namespace std;
using namespace cv;

// This function takes in two rectangles and checks if it is a person that was already tracked.
// It returns true or false.
bool is_person (Rect rect1, Rect rect2)
{
    Point top_left1 = rect1.tl();
    Point top_left2 = rect2.tl();

    int left1 = (pow((top_left1.x - top_left2.x), 2));
    int left2 = (pow((top_left1.y - top_left2.y), 2));
    if(sqrt(left1 + left2) <= 75)
    {
        return true;
    }
    else
        return false;
}

// This function calculates the distance traveled by two rectangles. If traveled 30 pixels then
// it will return true else it will return false.
bool not_standing_still(int x1, int x2)
{
    if(abs(x1 - x2) >= 20)
        return true;
    else
        return false;
}

// Gets the amout of time since the user has been updated
double duration_of_id(clock_t start_clock)
{
    double duration;
    duration = (double)(clock() - start_clock) / CLOCKS_PER_SEC;

    return duration;
}

int main (int argc, const char * argv[])
{
    // Vector that stores people
    vector<Person> people_vect;
    int count = 0;
    // CV_CAP_ANY
    // "/Users/harrisonwalls/Desktop/is-software/attempt4/attempt4/track_640.avi"
    // "/Users/harrisonwalls/Desktop/is-software/attempt4/attempt4/video1.avi"
    // "/Users/harrisonwalls/Desktop/is-software/attempt4/attempt4/video2.avi"
    VideoCapture cap(CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    if (!cap.isOpened())
        return -1;

    Mat img;
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    namedWindow("video capture", CV_WINDOW_AUTOSIZE);
    while(true)
    {
        cap >> img;
        if (!img.data)
            continue;

        vector<Rect> found, found_filtered;
        hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);

        // Throws out any rectangle that is inside another rectangle.
        size_t i, j, people;

        for (i=0; i<found.size(); i++)
        {
            Rect r = found[i];
            for (j=0; j<found.size(); j++)
                if (j!=i && (r & found[j])==r)
                    break;
            if (j==found.size())
                found_filtered.push_back(r);
        }

        // Draws each rectangle.
        for (i=0; i<found_filtered.size(); i++)
        {
            bool updated = false;
            Rect r = found_filtered[i];

            // Checks if the rectangle is a possible human by
            // checking if the width is less than the height.
            if(r.width < r.height)
            {
                // Draws a rectangle around objects that are human like.
                rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);


                // For each Person in people.
                for(people = 0; people < people_vect.size(); people++)
                {
                    // Checks if the rectangle is a person that was already tracked.
                    // If it is then it updates the person if not then it creates a person.
                    if(is_person(found_filtered[i], people_vect[people].last_rect) == true)
                    {
                        updated = true;
                        people_vect[people].appear_count++;
                        people_vect[people].last_rect = found_filtered[i];
                        people_vect[people].start = clock();
                        people_vect[people].recently_updated_id = true;
                        cout << "id: " << people_vect[people].person_id << " " << "Appear count: " << people_vect[people].appear_count <<endl;
                    }

                    // Checks if the person passed x position of 3 then checks if it was already committed
                    // 50 <= people_vect[h].last_rect.x) && (people_vect[h].last_rect.x <= 90) &&
                    if((people_vect[people].committed == false && people_vect[people].appear_count >= 9 && (not_standing_still(r.x, people_vect[people].last_x_position) == true)))
                    {
                        count++;
                        people_vect[people].appear_count++;
                        people_vect[people].committed = true;
                    }
                }

                // Checks if it was updated if not then it creates a new person and adds it to the vector people_vect.
                if(updated == false)
                {
                    Person people;
                    people.committed = false;
                    people.last_rect = found_filtered[i];
                    people.last_x_position = people.last_rect.x;
                    people.recently_updated_id = false;
                    people_vect.push_back(people);
                }
            }
            cout<< "People counted: " << count << endl;

            for(people = 0; people < people_vect.size();)
            {
            // create for loop
            // Checks if the last time the person was called is greater than a certain number then it
            // deletes that element from the vector.
                if(duration_of_id(people_vect[people].start) > 200.00 )
                {
                    cout << "ID WAS DELETED: " << people_vect[people].person_id << endl;
                    people_vect.erase(people_vect.begin() + (people));
                }
                else
                    people++;
            }
        }

        imshow("video capture", img);
        //cout << "printed frame" << endl;
        if (waitKey(1) == 27)
            break;
    }
    return 0;
}
