#include "math_fct.h"


float inner_angle_coor(float px1, float py1, float px2, float py2, float cx1, float cy1)
{

  float dist1 = sqrt(  (px1-cx1)*(px1-cx1) + (py1-cy1)*(py1-cy1) );
  float dist2 = sqrt(  (px2-cx1)*(px2-cx1) + (py2-cy1)*(py2-cy1) );

  float Ax, Ay;
  float Bx, By;
  float Cx, Cy;

  //find closest point to C
  //printf("dist = %lf %lf\n", dist1, dist2);

  Cx = cx1;
  Cy = cy1;
  if(dist1 < dist2)
  {
    Bx = px1;
    By = py1;
    Ax = px2;
    Ay = py2;
  }
  else
  {
    Bx = px2;
    By = py2;
    Ax = px1;
    Ay = py1;
  }

  float Q1 = Cx - Ax;
  float Q2 = Cy - Ay;
  float P1 = Bx - Ax;
  float P2 = By - Ay;

  float A = acos( (P1*Q1 + P2*Q2) / ( sqrt(P1*P1+P2*P2) * sqrt(Q1*Q1+Q2*Q2) ) );

  A = A*180/M_PI;

  return A;
}

float inner_angle(cv::Point p1, cv::Point p2, cv::Point c1)
{
  return inner_angle_coor(p1.x, p1.y, p2.x, p2.y, c1.x, c1.y);
}
