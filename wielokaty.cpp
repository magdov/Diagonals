#include<iostream>
#include<cstdio>
#include<cmath>
#include<vector>
#include<algorithm>

using namespace std;

typedef pair<double, double> Point;
typedef pair<int, int> Pair;
typedef vector< Point > Vert;
typedef vector< Pair > Diag;

const double EPS = 0.000000001;


// Tworzenie wielokata
void createPolygon(int n, Vert &vertices) {
	
	double pi = atan(1) * 4;
	double a = (2*pi) / (double)n;
	double x,y;
	
	for (int i = 0; i < n; ++i) {
		y = sin(i*a);
		x = cos(i*a);

		// printf("%.10f, %.10f\n", x, y);
		vertices.push_back(make_pair(x,y));
	}
}

bool ifParallel(Pair d1, Pair d2, Vert &vertices) {
	Point p1 = vertices[d1.first];
	Point p2 = vertices[d1.second];

	Point p3 = vertices[d2.first];
	Point p4 = vertices[d2.second];

	double d = ((p1.second - p2.second)*(p3.first - p4.first)) - ((p3.second - p4.second)*(p1.first - p2.first));

	if (abs(d) < EPS) {
		return true;
	}

	return false;
}

Point calculateIntersection(Pair d1, Pair d2, Vert &vertices) {
	Point p1 = vertices[d1.first];
	Point p2 = vertices[d1.second];

	Point p3 = vertices[d2.first];
	Point p4 = vertices[d2.second];
	
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

bool ifIntersect(Point p, Pair d3, Vert &vertices) {
	Point p1 = vertices[d3.first];
	Point p2 = vertices[d3.second];

	double a = (p1.second - p2.second) / (p1.first - p2.first);
	double b = p1.second - a*p1.first;

	double d = (p1.second - p2.second)*(p.first - p1.first) - (p.second - p1.second)*(p1.first - p2.first);

	// printf("%d %d %.10f\n", d3.first, d3.second, p);

	if (abs(d) < EPS) {
		return true;
	}

	return false;
}

void addDiagonal(Pair d1, Pair d2, Pair d3, vector<Diag> &diagonals) {
	Diag v;
	v.push_back(d1);
	v.push_back(d2);
	v.push_back(d3);

	// printf("diagonals %d %d %d %d %d %d\n", d1.first, d1.second, d2.first, d2.second, d3.first, d3.second);

	diagonals.push_back(v);
}

void addIfIntersectOrParallel(Vert &vertices, Pair d1, Pair d2, Pair d3, vector<Diag> &diagonals) {
	if (ifParallel(d1, d2, vertices) && ifParallel(d1, d3, vertices)) {
		// addDiagonal(d1, d2, d3, diagonals);
	}

	if (ifParallel(d1, d2, vertices) || ifParallel(d1, d3, vertices)) {
		return;
	}

	Point p = calculateIntersection(d1, d2, vertices);
	Point p2 = calculateIntersection(d1, d3, vertices);

	// printf("%.10f %.10f\n", p.second, p2.second);

	if (ifIntersect(p,d3, vertices)) {
		addDiagonal(d1, d2, d3, diagonals);
	} 

}

void findThirdDiagonal(int n, Vert &vertices, vector<Diag> &diagonals, Pair d1, Pair d2) {
	Pair d;
	for (int j = d1.second+1; j < n-1; ++j) {
		for (int k = j+2; k < n; ++k) {
			d = make_pair(j,k);
			addIfIntersectOrParallel(vertices, d1, d2, d, diagonals);
		}
	}
}

void findSecondDiagonal(int n, Vert &vertices, vector<Diag> &diagonals, Pair d1)
{
	Pair p;
	for (int j = 1; j < d1.second-1; ++j) 
	{
		for (int k = j+2; k < d1.second; ++k) 
		{
			p = make_pair(j,k);
			findThirdDiagonal(n, vertices, diagonals, d1, p);
		}
	}
}

void findDiagonals(int n, Vert &vertices, vector<Diag> &diagonals) {
	
	Pair p;
	
	// Przekatne od 0 do wierzcholka w polowie
	for (int i = 3; i <= n/2; ++i)
	{
		p = make_pair(0, i);
		findSecondDiagonal(n, vertices, diagonals, p);
	}
}

void diagonalDistances(int n, vector<Diag> &diagonals, vector<vector<int> > &diagDistances) 
{
	vector<int> v, w;
	for (int i = 0; i < diagonals.size(); ++i)
	{
		v.clear();
		w.clear();
		for (int j = 0; j < 3; ++j)
		{
			v.push_back(diagonals[i][j].first);
			v.push_back(diagonals[i][j].second);
		}

		sort(v.begin(), v.end());

		for (int i = 1; i < 6; ++i)
		{
			w.push_back(v[i] - v[i-1]);
		}
	
		w.push_back(n - v[5]);

		diagDistances.push_back(w);
	}
}

bool ifSymmetric(vector<int> diag)
{
	if ((diag[0] == diag[5]) && (diag[1] == diag[4]) && (diag[2] == diag[3]))
	{
		return true;
	}

	return false;
}

void removeSymmetricDiagonals(vector<vector<int> > &diagDistances, vector<vector<int> > &result)
{
	for (int i = 0; i < diagDistances.size(); ++i)
	{
		if (!(ifSymmetric(diagDistances[i])))
		{
			result.push_back(diagDistances[i]);
		}
	}
}

void printDiagonals(vector<Diag> &diagonals)
{
	for (int i = 0; i < diagonals.size(); ++i)
	{
		printf("%d %d %d %d %d %d\n",
			diagonals[i][0].first,
			diagonals[i][0].second,
			diagonals[i][1].first,
			diagonals[i][1].second,
			diagonals[i][2].first,
			diagonals[i][2].second
		);
	}
}

void printDiagonalDistances(vector<vector<int> > &diagDistances)
{
	for (int i = 0; i < diagDistances.size(); ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			printf("%d ", diagDistances[i][j]);
		}

		printf("\r\n");
	}
}

int main() {
	
	int n;

	Vert vertices;
	vector<Diag> diagonals;
	vector<vector<int> > diagDistances;
	vector<vector<int> > diagDistancesWithoutSym;

	printf("Liczba wierzcholkow wielokata: \n");
	scanf("%d", &n);

	createPolygon(n, vertices);
	findDiagonals(n, vertices, diagonals);
	diagonalDistances(n, diagonals, diagDistances);
	removeSymmetricDiagonals(diagDistances, diagDistancesWithoutSym);

	printf("%d\r\n", diagDistancesWithoutSym.size());
	printDiagonalDistances(diagDistancesWithoutSym);

	return 0;
}
