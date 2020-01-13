#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <experimental/optional>
#include <stdlib.h>

#include "math_fct.h"

using namespace std;
using namespace cv;

enum VideoType
{
  DRAW_RIGHT_ONLY = 1,
  DRAW_LEFT_ONLY = 2,
  DRAW_LEFT_AND_RIGHT = 3
};

// Feet
Point foot_left;
Point foot_right;
// Knees
Point knee_left;
Point knee_right;
// Hips
Point hip_left;
Point hip_right;
// Middle of the hips
Point middle_hips;
// Shoulders
Point shoulder_left;
Point shoulder_right;
// Head
Point head;

/*vector<Point> find_nearest_plot(vector<vector<Point>> contours)
{
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  auto max = contours[0];
  for (int i = 1; i < contours.size(); ++i)
  {
    if (contourArea(max) < contourArea(contours[i]))
      max = contours[i];
  }
  return max;
}*/
