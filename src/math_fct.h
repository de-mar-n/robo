#pragma once

#include <iostream>
#include <stdio.h>
#include <experimental/optional>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


float inner_angle_coor(float px1, float py1, float px2, float py2, float cx1, float cy1);
float inner_angle(cv::Point p1, cv::Point p2, cv::Point c1);

void print(vector<Point> input);
void print_vect_angle(vector<float> input);
