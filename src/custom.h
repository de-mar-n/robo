#pragma once

#include "math_fct.h"
//#define RED_YELLOW_MODE
#define GREEN_BLUE_MODE

using namespace cv;

// Create the mask for LEFT points (e.g., mask for red color).
Mat create_mask_left(InputArray hsv);

// Create the mask for RIGHT points (e.g., mask for yellow color).
Mat create_mask_right(InputArray hsv);
