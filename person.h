//
//  person.h
//
//  Created by Harrison Walls on 10/30/18.
//  Copyright © 2018 Harrison Walls. All rights reserved.
//

#ifndef person_h
#define person_h

#include <ctime>
#include <stdexcept>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

static int next_id = 0;

class Person
{
  public:
    Person()
    {
        committed = false;
        next_id++;
        person_id = next_id;
        appear_count = 0;
        recently_updated_id = false;
        start = 0;
    }
    int appear_count;
    int person_id;
    Rect last_rect;
    int last_x_position;
    bool committed;
    bool recently_updated_id;
    clock_t start;
};

#endif
