#include "common.h"

// Function to create a polygon with n vertices
void createPolygon(int n, Vert &vertices) {
    double pi = atan(1) * 4;
    double a = (2*pi) / (double)n;
    double x,y;

    for (int i = 0; i < n; ++i) {
        y = sin(i*a);
        x = cos(i*a);

        vertices.push_back(std::make_pair(x,y));
    }
}

double calculateDistance(Point p1, Point p2) {
    return (p1.first  - p2.first)*(p1.first - p2.first) + (p1.second - p2.second)*(p1.second - p2.second);
}

bool ifParallel(Point p1, Point p2, Point p3, Point p4) {
    
    double d = ((p1.second - p2.second)*(p3.first - p4.first)) - ((p3.second - p4.second)*(p1.first - p2.first));

	if (std::abs(d) < EPS) {
		return true;
	}

	return false;
}

bool ifParallel(Pair d1, Pair d2, Vert &vertices) {
    Point p1 = vertices[d1.first];
	Point p2 = vertices[d1.second];

	Point p3 = vertices[d2.first];
	Point p4 = vertices[d2.second];

    return ifParallel(p1, p2, p3, p4);
}

Point calculateIntersection(Point p1, Point p2, Point p3, Point p4) {
    
    double d = (p1.first - p2.first)*(p3.second - p4.second) - (p1.second - p2.second)*(p3.first - p4.first);

	double det1 = ((p1.first * p2.second) - (p1.second*p2.first));
	double det2 = ((p3.first * p4.second) - (p3.second*p4.first));

	Point p;
	p.first = (det1*(p3.first - p4.first) - det2*(p1.first - p2.first)) / d;
	p.second = (det1*(p3.second - p4.second) - det2*(p1.second - p2.second)) / d;

	// printf("%d %d %d %d\n", d1.first, d1.second, d2.first, d2.second);
	// printf("%.10f %.10f %.10f %.10f %.10f\n", d, det1, det2, p.first, p.second);

	return p;

}

Point calculateIntersection(Pair d1, Pair d2, Vert &vertices) {
    Point p1 = vertices[d1.first];
	Point p2 = vertices[d1.second];

	Point p3 = vertices[d2.first];
	Point p4 = vertices[d2.second];
	
    return calculateIntersection(p1, p2, p3, p4);	
}
