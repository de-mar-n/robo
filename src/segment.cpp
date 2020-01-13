
#include "segment.h"

#define CONTOUR_AREA_MIN  90000

#define DRAW_YELLOW_ONLY    1
#define DRAW_RED_ONLY       2
#define DRAW_YELLOW_AND_RED  3

vector<vector<Point>> delete_noise(vector<vector<Point>> contours)
{
  vector<Moments> mu(contours.size());
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  for (int i = 1; i < contours.size(); ++i)
  {
    //cout << "=> AREA: " << contourArea(contours[i])<< endl;
    if (contourArea(contours[i]) < CONTOUR_AREA_MIN)
      contours.erase(contours.begin() + i);
  }
  return contours;
}

void print(vector<Point> input)
{
  for (int i = 0; i < input.size(); ++i)
      cout << input.at(i) << ' ';
  cout << '\n' << endl;
}

bool compare_point(const Point& a, const Point& b)
{
  return a.y > b.y;
}

// Get center of each contour
vector<Point> get_points(vector<vector<Point>> contours)
{
  vector<Point> result;
  for (int i = 0; i < contours.size(); ++i)
  {
    Point p;
    for (int x = 0; x < contours[i].size(); ++x)
    {
      p.x += contours[i][x].x;
      p.y += contours[i][x].y;
    }
    p.x /= contours[i].size();
    p.y /= contours[i].size();
    result.push_back(p);
  }
  return result;
}

void compute_angles(int video_type)
{
  if (video_type == DRAW_YELLOW_ONLY)
  {
    cout << "ANGLE GENOU: " << inner_angle(footYELLOW, kneeYELLOW, hipYELLOW) << endl;
    cout << "ANGLE HANCHE: " << inner_angle(kneeYELLOW, hipYELLOW, shoulderYELLOW) << endl;
  }
  else if (video_type == DRAW_RED_ONLY)
  {
    cout << "ANGLE GENOU: " << inner_angle(footRED, kneeRED, hipRED) << endl;
    cout << "ANGLE HANCHE: " << inner_angle(kneeRED, hipRED, shoulderRED) << endl;
  }
  else if (video_type == DRAW_YELLOW_AND_RED)
  {
    cout << "ANGLE GENOU GAUCHE: " << inner_angle(footYELLOW, kneeYELLOW, hipYELLOW) << endl;
    cout << "ANGLE HANCHE GAUCHE: " << inner_angle(kneeYELLOW, hipYELLOW, shoulderYELLOW) << endl;
    cout << "ANGLE GENOU DROIT: " << inner_angle(footRED, kneeRED, hipRED) << endl;
    cout << "ANGLE HANCHE DROITE: " << inner_angle(kneeRED, hipRED, shoulderRED) << endl;
  }
  else
    cout << "ERROR: can't compute angles, no data found" << endl;
}

void draw_line(Mat img, Point p1, Point p2)
{
  line(img, p1, p2, Scalar(255, 0, 0), 5);
}

void draw_lines(Mat img, int video_type)
{
  if (video_type == DRAW_YELLOW_ONLY)
  {
    draw_line(img, footYELLOW, kneeYELLOW);
    draw_line(img, kneeYELLOW, hipYELLOW);
    draw_line(img, hipYELLOW, shoulderYELLOW);
    draw_line(img, shoulderYELLOW, head);
  }
  else if (video_type == DRAW_RED_ONLY)
  {
    draw_line(img, footRED, kneeRED);
    draw_line(img, kneeRED, hipRED);
    draw_line(img, hipRED, shoulderRED);
    draw_line(img, shoulderRED, head);
  }
  else if (video_type == DRAW_YELLOW_AND_RED)
  {
    // Draw left leg
    draw_line(img, footRED, kneeRED);
    draw_line(img, kneeRED, hipRED);
    // Draw right leg
    draw_line(img, footYELLOW, kneeYELLOW);
    draw_line(img, kneeRED, hipYELLOW);
    // Draw hips
    draw_line(img, hipRED, hipYELLOW);
    // Draw shoulders
    draw_line(img, shoulderRED, shoulderYELLOW);
    // Draw spine
    draw_line(img, head, middle_hips);
  }
  else
    cout << "PROBLEME : macro DRAW" << endl;
}

int display_skeleton(vector<Point> pointsRED, vector<Point> pointsYELLOW, Mat img)
{
  //cout << "===> nb point : " << pointsRED.size() << endl;
    //  RED - YELLOW
    if (pointsRED.size() >= 4 && pointsYELLOW.size() >= 4)
    {
      // Find feet
      footRED = pointsRED.at(0);
      footYELLOW = pointsYELLOW.at(0);
      // Find knees
      kneeRED = pointsRED.at(1);
      kneeYELLOW = pointsYELLOW.at(1);
      // Find hips
      hipRED = pointsRED.at(2);
      hipYELLOW = pointsYELLOW.at(2);
      // Find middle of the hips
      middle_hips;
      middle_hips.x = (hipRED.x + hipYELLOW.x) / 2;
      // Find shoulders
      shoulderRED = pointsRED.at(3);
      shoulderYELLOW = pointsYELLOW.at(3);

      // Find head
      head = (shoulderRED + shoulderYELLOW) / 2;
      if (pointsRED.size() == 5)
        head = pointsRED.at(4);

      draw_lines(img, DRAW_YELLOW_AND_RED);
      compute_angles(DRAW_YELLOW_AND_RED);
      //auto frame_middle = img.rows / 2;

    }
    // Only RED - Profil gauche
    else if (pointsRED.size() >= 4 && pointsYELLOW.empty())
    {
      // Find feet
      footRED = pointsRED.at(0);
      // Find knees
      kneeRED = pointsRED.at(1);
      // Find hips
      hipRED = pointsRED.at(2);
      // Find shoulders
      shoulderRED = pointsRED.at(3);
      // Find "head"
      head = shoulderRED;
      // Relies all point

      draw_lines(img, DRAW_RED_ONLY);
      compute_angles(DRAW_RED_ONLY);
    }
    // Only YELLOW - Profil droit
    else if (pointsRED.empty() && pointsYELLOW.size() >= 4)
    {
      // Find feet
      footYELLOW = pointsYELLOW.at(0);
      // Find knees
      kneeYELLOW = pointsYELLOW.at(1);
      // Find hips
      hipYELLOW = pointsYELLOW.at(2);
      // Find shoulders
      shoulderYELLOW = pointsYELLOW.at(3);
      // Find "head"
      head = shoulderYELLOW;
      // Relies all points
      draw_lines(img, DRAW_YELLOW_ONLY);
      compute_angles(DRAW_YELLOW_ONLY);
      //arrowedLine(img, nearestRED[0], nearestYELLOW[0], Scalar(255, 0, 0),5);
    }
    // 0 RED - 0 YELLOW
    else
      cout << "No valid data" << endl;

    return 1;

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
  findContours(canny_output, contoursRED, hierarchy,  cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));

  // Detect YELLOW plot using canny
  Canny( res3, canny_output, thresh, thresh*2, 3 );
  findContours(canny_output, contoursYELLOW, hierarchy,  cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));

  // Enleve le bruit
  contoursRED = delete_noise(contoursRED);
  contoursYELLOW = delete_noise(contoursYELLOW);
  //cout << "NB 2 : " << contoursRED.size() << endl;
  // Sort
  vector<Point> pointsRED = get_points(contoursRED);
  vector<Point> pointsYELLOW = get_points(contoursYELLOW);
  sort(pointsRED.begin(), pointsRED.end(), compare_point);
  sort(pointsYELLOW.begin(), pointsYELLOW.end(), compare_point);
  if (pointsRED.size() > 4)
    pointsRED = vector<Point>(pointsRED.begin(), pointsRED.begin() + 3);
  if (pointsYELLOW.size() > 4)
    pointsYELLOW = vector<Point>(pointsYELLOW.begin(), pointsYELLOW.begin() + 3);

  // Merge res2 (for red plot) and res3 (for yellow plot)
  addWeighted(res2,1,res3,1,0,final_output);

  // Compute distance between 2 nearest plots from 2 diffrent colors
  result = display_skeleton(pointsRED, pointsYELLOW, final_output);
  //print_directive(result);
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
