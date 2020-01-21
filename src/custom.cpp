#include "custom.h"

#ifdef RED_YELLOW_MODE
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
#else
    #ifdef GREEN_BLUE_MODE
        // Example: here, we use green for LEFT points and blue for RIGHT points.
        Mat create_mask_left(InputArray hsv)
        {
            // H: 97/360 → 48/180
            // S: 53/100 → 135/255
            // V: 51/100 → 130/255
            Mat mask;
            inRange(hsv, Scalar(30, 100, 100), Scalar(70, 255, 255), mask);
            return mask;
        }

        Mat create_mask_right(InputArray hsv)
        {
            // H: 201/360 → 100/180
            // S:  60/100 → 153/255
            // V:  68/100 → 173/255
            Mat mask;
            inRange(hsv, Scalar(80, 100, 100), Scalar(120, 255, 255), mask);
            return mask;
        }
    #else
        // Example: here, we use red for LEFT points and yellow for RIGHT points.
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
#endif
