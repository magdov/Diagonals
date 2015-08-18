#include<cstdio>
#include<iostream>
#include<algorithm>
#include<set>

#include "common.h"

struct CompareResults {
    bool operator()(const std::vector<int> v1, const std::vector<int> v2) const {
        std::vector<int> temp1, temp2;
        for (int i = 0; i < 6; ++i) {
            temp1.push_back(v1[i]);
            temp2.push_back(v2[i]);
        }

        std::sort(temp1.begin(), temp1.end());
        std::sort(temp2.begin(), temp2.end());

        return temp1 < temp2;
    }
};

bool calculateF(Pair ab, Pair de, Pair pq, int c, Vert &vertices, 
        Point &result) {
   Pair bc = std::make_pair(ab.second, c);
   Pair dc = std::make_pair(de.first, c);
   
   if (ifParallel(bc, pq, vertices)) {
       return false;
   }

   Point bcpq = calculateIntersection(bc, pq, vertices);
   
   if (ifParallel(dc, pq, vertices)) {
       return false;
   }

   Point dcpq = calculateIntersection(dc, pq, vertices);

   Point e = vertices[de.second];
   Point a = vertices[ab.first];

   if (ifParallel(bcpq, e, dcpq, a)) {
       return false;
   }

   result = calculateIntersection(bcpq, e, dcpq, a);
   return true;
}

int isVertex(Point f, Vert &vertices) {
    for (int i = 0; i < vertices.size(); ++i) {
        //printf("%f\n", std::abs(calculateDistance(vertices[i], f)));
        if (std::abs(calculateDistance(vertices[i], f)) < EPS) {
            return i;
        }
    }

    return NOT_FOUND;
}

bool checkIfPairConsist(Pair p, int c) {
    if ((p.first == c) || (p.second == c)) {
        return true;
    }

    return false;
}

bool checkIfInPairs(Pair ab, Pair de, Pair pq, int c) {
    if (checkIfPairConsist(ab, c)) {
        return true;
    }

    if (checkIfPairConsist(de, c)) {
        return true;
    }

    if (checkIfPairConsist(pq, c)) {
        return true;
    }

    return false;
}

void writePairs(Pair ab, Pair de, Pair cf) {
    printf("%d %d %d %d %d %d\n",
            ab.first, ab.second, de.first, de.second, cf.first, cf.second);
}

void addPairToResult(Pair p, std::vector<int> &v) {
    v.push_back(p.first);
    v.push_back(p.second);
}

void addNewResult(Pair ab, Pair de, Pair cf, Pair pq, 
        std::set<std::vector<int>,CompareResults> &results) {
    std::vector<int> result;
    addPairToResult(ab, result);
    addPairToResult(de, result);
    addPairToResult(cf, result);
    addPairToResult(pq, result);

    results.insert(result);
}

// Calculate points C and F from A,B,D,E,P,Q
void calculateCFFromABDEPQ(Pair ab, Pair de, Pair pq, Vert &vertices,
        std::set<std::vector<int>,CompareResults> &results) {
    for (int c = 0; c < vertices.size(); ++c) {
        if (!checkIfInPairs(ab, de, pq, c)) {
            Point f;
            if (calculateF(ab, de, pq, c, vertices, f)) {
                int vertex_nr = isVertex(f, vertices);
                if (vertex_nr != NOT_FOUND && 
                        !checkIfInPairs(ab, de, pq, vertex_nr)) {
                    //writePairs(ab, de, std::make_pair(c, vertex_nr));
                    addNewResult(ab, de, std::make_pair(c, vertex_nr), pq, results);
                }
            }
        }
    }
}

void checkAllPairs(std::vector<int> &toConsider, Vert &vertices,
        std::set<std::vector<int>,CompareResults> &results) {
    Pair ab, de, pq;
    for (int i = 0; i < POLYGON_SIZE; ++i) {
        ab = std::make_pair(toConsider[i], toConsider[(i+3) % POLYGON_SIZE]);
        
        de = std::make_pair(toConsider[(i+1)%6], toConsider[(i+4)%6]);
        pq = std::make_pair(toConsider[(i+2)%6], toConsider[(i+5)%6]);
        calculateCFFromABDEPQ(ab, de, pq, vertices, results);
       
        
        de = std::make_pair(toConsider[(i+4)%6], toConsider[(i+1)%6]);
        pq = std::make_pair(toConsider[(i+2)%6], toConsider[(i+5)%6]);
        calculateCFFromABDEPQ(ab, de, pq, vertices, results);

        de = std::make_pair(toConsider[(i+2)%6], toConsider[(i+5)%6]);
        pq = std::make_pair(toConsider[(i+1)%6], toConsider[(i+4)%6]);
        calculateCFFromABDEPQ(ab, de, pq, vertices, results);

        de = std::make_pair(toConsider[(i+5)%6], toConsider[(i+2)%6]);
        pq = std::make_pair(toConsider[(i+1)%6], toConsider[(i+4)%6]);
        calculateCFFromABDEPQ(ab, de, pq, vertices, results);
        
    }
}

void transformDistancesToVertex(std::vector<int> &distances, std::vector<int> &vertices) {
    
    int akt = 0;
    vertices.push_back(0);
    for (int i = 0; i < distances.size() - 1; ++i) {
        akt += distances[i];
        //printf("%d\n", akt);
        vertices.push_back(akt);
    }
}

void writeResults(std::set<std::vector<int>,CompareResults> results) {
    printf("%d\n", results.size());
    for (std::set<std::vector<int> >::iterator it = results.begin(); 
            it != results.end(); it++) {
        for (int i = 0; i < (*it).size(); ++i) {
            printf("%d ", (*it)[i]);
        }

        printf("\n");
    }
}

int main() {
   int n, a;

   Vert vertices;
   std::vector<int> distances;
   std::vector<int> toConsider;
   std::set<std::vector<int>,CompareResults> results;

   printf("Liczba wierzcholkow wielokata: \n");
   scanf("%d", &n);

   createPolygon(n, vertices);

   printf("Konfiguracja do zbadania:\n");
   
   for (int i = 0; i < 6; ++i) {
       scanf("%d", &a);
       distances.push_back(a);
   }

   transformDistancesToVertex(distances, toConsider);
   checkAllPairs(toConsider, vertices, results);
   writeResults(results);
}
