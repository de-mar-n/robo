#include "custom.h"

// Modify this function to detect the color of your liking for LEFT points!
Mat create_mask_left(InputArray hsv)
{
    Mat mask1, mask2;
    // Creating masks to detect the upper and lower red color
    // Red got 2 inRange because it's a major color while yellow is a minor
    inRange(hsv, Scalar(0, 120, 70), Scalar(10, 255, 255), mask1);
    inRange(hsv, Scalar(170, 120, 70), Scalar(180, 255, 255), mask2);
    mask1 = mask1 + mask2;
    return mask1;
}

// Modify this function to detect the color of your liking for RIGHT points!
Mat create_mask_right(InputArray hsv)
{
    // Creating masks to detect the upper and lower yellow color.
    Mat mask;
    inRange(hsv, Scalar(20, 150, 150), Scalar(50, 255, 255), mask);
    return mask;
}

#if 0
// Example: here, we use red for LEFT points and yellow for RIGHT points.s

Mat create_mask_left(InputArray hsv)
{
    Mat mask1, mask2;
    // Creating masks to detect the upper and lower red color
    // Red got 2 inRange because it's a major color while yellow is a minor
    inRange(hsv, Scalar(0, 120, 70), Scalar(10, 255, 255), mask1);
    inRange(hsv, Scalar(170, 120, 70), Scalar(180, 255, 255), mask2);
    mask1 = mask1 + mask2;
    return mask1;
}

Mat create_mask_right(InputArray hsv)
{
    // Creating masks to detect the upper and lower yellow color.
    Mat mask;
    inRange(hsv, Scalar(20, 150, 150), Scalar(50, 255, 255), mask);
    return mask;
}
#endif
