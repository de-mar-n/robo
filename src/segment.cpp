
#include "segment.h"

#define CONTOUR_AREA_MIN  90000

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

void compute_angles(VideoType video_type)
{
  if (video_type == DRAW_RIGHT_ONLY)
  {
    cout << "ANGLE GENOU (DROIT)  : " << inner_angle(foot_right, knee_right, hip_right) << endl;
    cout << "ANGLE HANCHE (DROITE): " << inner_angle(knee_right, hip_right, shoulder_right) << endl;
  }
  else if (video_type == DRAW_LEFT_ONLY)
  {
    cout << "ANGLE GENOU (GAUCHE) : " << inner_angle(foot_left, knee_left, hip_left) << endl;
    cout << "ANGLE HANCHE (GAUCHE): " << inner_angle(knee_left, hip_left, shoulder_left) << endl;
  }
  else if (video_type == DRAW_LEFT_AND_RIGHT)
  {
    cout << "ANGLE GENOU GAUCHE   : " << inner_angle(foot_right, knee_right, hip_right) << endl;
    cout << "ANGLE HANCHE GAUCHE  : " << inner_angle(knee_right, hip_right, shoulder_right) << endl;
    cout << "ANGLE GENOU DROIT    : " << inner_angle(foot_left, knee_left, hip_left) << endl;
    cout << "ANGLE HANCHE DROITE  : " << inner_angle(knee_left, hip_left, shoulder_left) << endl;
  }
  else
    cout << "ERROR: can't compute angles, no data found" << endl;
}

void draw_line(Mat img, Point p1, Point p2)
{
  line(img, p1, p2, Scalar(255, 0, 0), 5);
}

void draw_lines(Mat img, VideoType video_type)
{
  if (video_type == DRAW_RIGHT_ONLY)
  {
    draw_line(img, foot_right, knee_right);
    draw_line(img, knee_right, hip_right);
    draw_line(img, hip_right, shoulder_right);
    draw_line(img, shoulder_right, head);
  }
  else if (video_type == DRAW_LEFT_ONLY)
  {
    draw_line(img, foot_left, knee_left);
    draw_line(img, knee_left, hip_left);
    draw_line(img, hip_left, shoulder_left);
    draw_line(img, shoulder_left, head);
  }
  else if (video_type == DRAW_LEFT_AND_RIGHT)
  {
    // Draw left leg
    draw_line(img, foot_left, knee_left);
    draw_line(img, knee_left, hip_left);
    // Draw right leg
    draw_line(img, foot_right, knee_right);
    draw_line(img, knee_left, hip_right);
    // Draw hips
    draw_line(img, hip_left, hip_right);
    // Draw shoulders
    draw_line(img, shoulder_left, shoulder_right);
    // Draw spine
    draw_line(img, head, middle_hips);
  }
  else
    cout << "PROBLEME : macro DRAW" << endl;
}

int display_skeleton(vector<Point> points_left, vector<Point> points_right, Mat img)
{
  //cout << "===> nb point : " << points_left.size() << endl;
    //  LEFT - RIGHT
    if (points_left.size() >= 4 && points_right.size() >= 4)
    {
      // Find feet
      foot_left = points_left.at(0);
      foot_right = points_right.at(0);
      // Find knees
      knee_left = points_left.at(1);
      knee_right = points_right.at(1);
      // Find hips
      hip_left = points_left.at(2);
      hip_right = points_right.at(2);
      // Find middle of the hips
      middle_hips;
      middle_hips.x = (hip_left.x + hip_right.x) / 2;
      // Find shoulders
      shoulder_left = points_left.at(3);
      shoulder_right = points_right.at(3);

      // Find head
      head = (shoulder_left + shoulder_right) / 2;
      if (points_left.size() == 5)
        head = points_left.at(4);

      draw_lines(img, DRAW_LEFT_AND_RIGHT);
      compute_angles(DRAW_LEFT_AND_RIGHT);
      //auto frame_middle = img.rows / 2;

    }
    // Only LEFT - Profil gauche
    else if (points_left.size() >= 4)
    {
      // Find feet
      foot_left = points_left.at(0);
      // Find knees
      knee_left = points_left.at(1);
      // Find hips
      hip_left = points_left.at(2);
      // Find shoulders
      shoulder_left = points_left.at(3);
      // Find "head"
      head = shoulder_left;
      // Relies all point

      draw_lines(img, DRAW_LEFT_ONLY);
      compute_angles(DRAW_LEFT_ONLY);
    }
    // Only RIGHT - Profil droit
    else if (points_right.size() >= 4)
    {
      // Find feet
      foot_right = points_right.at(0);
      // Find knees
      knee_right = points_right.at(1);
      // Find hips
      hip_right = points_right.at(2);
      // Find shoulders
      shoulder_right = points_right.at(3);
      // Find "head"
      head = shoulder_right;
      // Relies all points
      draw_lines(img, DRAW_RIGHT_ONLY);
      compute_angles(DRAW_RIGHT_ONLY);
      //arrowedLine(img, nearest_left[0], nearest_right[0], Scalar(255, 0, 0),5);
    }
    // 0 left - 0 right
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

  Mat mask_left = create_mask_left(hsv);
  Mat mask_right = create_mask_right(hsv);

  // Reconize the figure of the LEFT point
  Mat kernel = Mat::ones(3,3, CV_32F);
  morphologyEx(mask_left, mask_left, cv::MORPH_OPEN, kernel);
  morphologyEx(mask_left, mask_left, cv::MORPH_DILATE, kernel);

  // Reconize the figure of the RIGHT point
  morphologyEx(mask_right, mask_right, cv::MORPH_OPEN, kernel);
  morphologyEx(mask_right, mask_right, cv::MORPH_DILATE, kernel);

  Mat res1, res2, res3, final_output;
  /*// creating an inverted mask to segment out the cloth from the frame
  bitwise_not(mask1, mask2); // for the res1 only
  // creating image: LEFT plots go in black
  bitwise_and(frame,frame,res1,mask2);*/

  // creating image: black background and LEFT plot
  bitwise_and(frame, frame, res2, mask_left);

  // creating image: black background and RIGHT plots
  bitwise_and(frame, frame, res3, mask_right);


// -----------------------
  Mat canny_output;
  int thresh = 100;
  int max_thresh = 255;
  RNG rng(12345);
  vector<vector<Point>> contours_left;
  vector<vector<Point>> contours_right;
  vector<Vec4i> hierarchy;

  /// Detect LEFT plot using canny
  Canny( res2, canny_output, thresh, thresh*2, 3 );
  findContours(canny_output, contours_left, hierarchy,  cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));

  // Detect RIGHT plot using canny
  Canny( res3, canny_output, thresh, thresh*2, 3 );
  findContours(canny_output, contours_right, hierarchy,  cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));

  // Enleve le bruit
  contours_left = delete_noise(contours_left);
  contours_right = delete_noise(contours_right);
  //cout << "NB 2 : " << contours_left.size() << endl;
  // Sort
  vector<Point> points_left = get_points(contours_left);
  vector<Point> points_right = get_points(contours_right);
  sort(points_left.begin(), points_left.end(), compare_point);
  sort(points_right.begin(), points_right.end(), compare_point);
  if (points_left.size() > 4)
    points_left = vector<Point>(points_left.begin(), points_left.begin() + 3);
  if (points_right.size() > 4)
    points_right = vector<Point>(points_right.begin(), points_right.begin() + 3);

  // Merge res2 (for red plot) and res3 (for yellow plot)
  addWeighted(res2,1,res3,1,0,final_output);

  // Compute distance between 2 nearest plots from 2 diffrent colors
  result = display_skeleton(points_left, points_right, final_output);
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
