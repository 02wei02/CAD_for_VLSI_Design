/* Find the contour path of y-axis */
#ifndef TOUR_H
#define TOUR_H
#include <iostream>
#include "structure.h"
using namespace std;
class Tour
{
public:
    void insert(double x1, double x2, double y, Contour *&head);  // insert contour node
    void clear(Contour *&head);                                   // clear all the contour
    void remove(double x1, double x2, double y, Contour *&head);  // remove contour node
    void refresh(double x1, double x2, double y, Contour *&head); // refresh contour node
    void printContour(Contour *head);                             // print contour
    double findY(double x1, double x2, Contour *head);           // find the y value in x1 and x2
};
#endif