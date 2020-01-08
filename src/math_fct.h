#include <iostream>
#include <stdio.h>
#include <experimental/optional>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


float innerAngle(float px1, float py1, float px2, float py2, float cx1, float cy1);
