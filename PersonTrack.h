//
// Created by dilin on 5/11/17.
//

#ifndef PEOPLETRACKER_DEMO_PERSONTRACK_H
#define PEOPLETRACKER_DEMO_PERSONTRACK_H

#include<iostream>
#include <opencv2/opencv.hpp>

class PersonTrack
{
public:
    PersonTrack();
    ~PersonTrack();

private:
    int id;
    cv::KalmanFilter *kf;
    cv::Rect bbox;
    long age;
    long totalVisibleCount;
    long consecutivrInvisibleCount;
};


#endif //PEOPLETRACKER_DEMO_PERSONTRACK_H
