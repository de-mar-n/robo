#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <experimental/optional>
#include <stdlib.h>


using namespace std;
using namespace cv;

#define GO_FORWARD      48
#define GO_LEFT         49
#define GO_RIGHT        50
#define STOP            51

// 354 pixel = 3cm
#define MARGE             200


vector<Point> find_nearest_plot(vector<vector<Point>> contours)
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
}

void print_directive(int directive)
{
  if (directive == GO_LEFT)
    cout << "<<<<<<<<<   LEFT" << endl;
  else if (directive == GO_RIGHT)
    cout << "                 >>>>>>>>>   RIGHT" << endl;
  else if (directive == GO_FORWARD)
    cout << "CONTINUE" << endl;
  else
    cout << "STOP" << endl;
}

/**
  Return :
        -1 when the car must go forward
        0 when the car must go left
        1 when the car must go right
        2 when the car must stop
**/
int compute_distance(vector<vector<Point>> contoursRED, vector<vector<Point>> contoursYELLOW, Mat img)
{
    // 1 RED - 1 YELLOW
    if (!contoursRED.empty() && !contoursYELLOW.empty())
    {
      vector<Point> nearestRED = find_nearest_plot(contoursRED);
      vector<Point> nearestYELLOW = find_nearest_plot(contoursYELLOW);
      // Draw a line between the plots
      line(img, nearestRED[0], nearestYELLOW[0], Scalar(255, 0, 0), 5);

      // Determine RIGHT or LEFT
      auto line_middle = (nearestRED[0].x + nearestYELLOW[0].x) / 2; // Middle of the line between the 2 nearest plot
      //line_middle.y = (nearestRED[0].y + nearestYELLOW[0].y) / 2;
      auto frame_middle = img.rows / 2;

      if (line_middle > frame_middle + MARGE)
        return GO_RIGHT;
      else if (line_middle < frame_middle - MARGE)
        return GO_LEFT;
      else
        return GO_FORWARD;
    }
    // Only RED
    else if (!contoursRED.empty() && contoursYELLOW.empty())
    {
      return GO_RIGHT;
    }
    // Only YELLOW
    else if (contoursRED.empty() && !contoursYELLOW.empty())
    {
      //arrowedLine(img, nearestRED[0], nearestYELLOW[0], Scalar(255, 0, 0),5);
      return GO_LEFT;
    }
    // 0 RED - 0 YELLOW
    else
      return STOP;
}


int find_plots(char *inputVideo)
{
  int result = 0;
    // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
  VideoCapture cap(inputVideo);

  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  Mat hsv;
  for (;;)
  {
    Mat frame;
    cap >> frame; // get a new frame from camera

    //Converting image from BGR to HSV color space.
    Mat hsv, gray_frame;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    cvtColor(frame, gray_frame, COLOR_RGB2GRAY);

  Mat mask1,mask2,mask3;
  // Creating masks to detect the upper and lower red color
  // Red got 2 inRange because it's a major color while yellow is a minor
  inRange(hsv, Scalar(0, 120, 70), Scalar(10, 255, 255), mask1);
  inRange(hsv, Scalar(170, 120, 70), Scalar(180, 255, 255), mask2);

  // Creating masks to detect the upper and lower yellow color.
  inRange(hsv, Scalar(20, 150, 150), Scalar(50, 255, 255), mask3);

  // Generating the final red mask
  mask1 = mask1 + mask2;

  // Reconize the figure of the red plot
  Mat kernel = Mat::ones(3,3, CV_32F);
  morphologyEx(mask1,mask1,cv::MORPH_OPEN,kernel);
  morphologyEx(mask1,mask1,cv::MORPH_DILATE,kernel);

  // Reconize the figure of the yellow plot
  morphologyEx(mask3,mask3,cv::MORPH_OPEN,kernel);
  morphologyEx(mask3,mask3,cv::MORPH_DILATE,kernel);


  Mat res1, res2, res3, final_output;
  /*// creating an inverted mask to segment out the cloth from the frame
  bitwise_not(mask1,mask2); // for the res1 only
  // creating image: red plots go in black
  bitwise_and(frame,frame,res1,mask2);*/

  // creating image: black background and red plot
  bitwise_and(frame,frame,res2,mask1);

  // creating image: black background and yellow plots
  bitwise_and(frame,frame, res3,mask3);


// -----------------------
  Mat canny_output;
  int thresh = 100;
  int max_thresh = 255;
  RNG rng(12345);
  vector<vector<Point>> contoursRED;
  vector<vector<Point>> contoursYELLOW;
  vector<Vec4i> hierarchy;

  /// Detect RED plot using canny
  Canny( res2, canny_output, thresh, thresh*2, 3 );
  findContours(canny_output, contoursRED, hierarchy,  CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  // Detect YELLOW plot using canny
  Canny( res3, canny_output, thresh, thresh*2, 3 );
  findContours(canny_output, contoursYELLOW, hierarchy,  CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  // Merge res2 (for red plot) and res3 (for yellow plot)
  addWeighted(res2,1,res3,1,0,final_output);
  // Compute distance between 2 nearest plots from 2 diffrent colors
  result = compute_distance(contoursRED, contoursYELLOW, final_output);
  print_directive(result);
// ------------------------------

    imshow("Result", final_output);
    if(waitKey(30) >= 0) break;
  }
  return result;

}

int main(int argc, char**argv)
{
  if (!argv[1])
  {
      cout << "No file given" << endl;
      return 1;
  }
  return find_plots(argv[1]);

}
