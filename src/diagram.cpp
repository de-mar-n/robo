#include "diagram.h"

void create_diagram(vector<float> kneesAnglesLEFT, vector<float> kneesAnglesRIGHT,
                    vector<float> hipsAnglesLEFT, vector<float> hipsAnglesRIGHT)
{
  //Mat diagram = imread("../../NouvelElement328.png");
  Mat diagram(Size(DIAGRAM_WIDTH, DIAGRAM_HEIGHT), CV_8UC3, Scalar(255,255,255));
  //int kLEFTindex, kRIGHTindex, hLEFTindex, hRIGHTindex = 0;

  line(diagram, Point(0, DIAGRAM_HEIGHT), Point(DIAGRAM_WIDTH, DIAGRAM_HEIGHT), Scalar(0, 0, 0), 3);
  line(diagram, Point(0,0), Point(0, DIAGRAM_HEIGHT), Scalar(0, 0, 0), 3);

  for (int i = 0; i < kneesAnglesLEFT.size(); ++i)
  {
    line(diagram, Point(i, kneesAnglesLEFT.at(i) + DIAGRAM_HEIGHT - 10), Point(i, kneesAnglesLEFT.at(i) + DIAGRAM_HEIGHT - 10), Scalar(255, 0, 0), 2);
  }

  for (int j = 0; j < kneesAnglesRIGHT.size(); ++j)
  {
    line(diagram, Point(j, kneesAnglesRIGHT.at(j) + DIAGRAM_HEIGHT - 10), Point(j, kneesAnglesRIGHT.at(j) + DIAGRAM_HEIGHT - 10), Scalar(0, 255, 0), 2);
  }

  for (int i = 0; i < hipsAnglesLEFT.size(); ++i)
  {
    line(diagram, Point(i, hipsAnglesLEFT.at(i) + DIAGRAM_HEIGHT - 10), Point(i, hipsAnglesLEFT.at(i) + DIAGRAM_HEIGHT - 10), Scalar(0, 0, 255), 2);
  }

  for (int i = 0; i < hipsAnglesRIGHT.size(); ++i)
  {
    line(diagram, Point(i, hipsAnglesRIGHT.at(i) + DIAGRAM_HEIGHT - 10), Point(i, hipsAnglesRIGHT.at(i) + DIAGRAM_HEIGHT - 10), Scalar(0, 255, 255), 2);
  }

  imshow("Diagram (x: Time, y: Angle)", diagram);
}
