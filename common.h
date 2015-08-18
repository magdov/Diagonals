#ifndef COMMON_H
#define COMMON_H

#include<iostream>
#include<vector>
#include<cmath>

typedef std::pair<double, double> Point;
typedef std::pair<int, int> Pair;
typedef std::vector<Point> Vert;
typedef std::vector<Pair> Diag;

const double EPS = 0.00000001;
const int NOT_FOUND = -1;
const int POLYGON_SIZE = 6;

// Function to create a polygon with n vertices
void createPolygon(int n, Vert &vertices);

// Calculate distance between points
double calculateDistance(Point p1, Point p2);

// Check if two lines are parallel
bool ifParallel(Point p1, Point p2, Point p3, Point p4);

// Check if two lines are parallel
bool ifParallel(Pair d1, Pair d2, Vert &vertices);

// Calculate intersection between points
Point calculateIntersection(Point p1, Point p2, Point p3, Point p4);

// Calculate intersection between two lines given as pairs of vertices
Point calculateIntersection(Pair d1, Pair d2, Vert &vertices);

#endif
