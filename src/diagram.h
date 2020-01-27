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

#define DIAGRAM_HEIGHT  180
#define DIAGRAM_WIDTH   500

void create_diagram(vector<float> kneesAnglesLEFT, vector<float> kneesAnglesRIGHT,
                    vector<float> hipsAnglesLEFT, vector<float> hipsAnglesRIGHT);
