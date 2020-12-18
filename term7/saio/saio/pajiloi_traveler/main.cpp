//
//  main.cpp
//  pajiloi_traveler
//
//  Created by Kirill on 12/16/20.
//

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
 
using namespace std;

typedef long long LL;
typedef std::vector<LL> Vector;
typedef std::vector<std::vector<LL>> TwoDimensionalMatrix;
typedef std::vector<std::vector<std::vector<LL>>> ThreeDimensionalMatrix;

#define print_with_endl(x) std::cout << x << "\n"

void FindMinRoad(ThreeDimensionalMatrix& dynamic,
                 TwoDimensionalMatrix& roadLengths,
                 size_t n,
                 LL mask, LL visit, LL teleport)
{
    auto result = dynamic.at(mask).at(visit).at(teleport);
    for (size_t i = 0; i < n; ++i) {
        if ((mask >> (LL)i) & 1)
            continue;
        
        LL nextMask = mask ^ (LL)1 << i;

        if (teleport) {
            auto oldCost = dynamic.at(nextMask).at(i).at(teleport - 1);
            dynamic.at(nextMask).at(i).at(teleport - 1) = min(oldCost, result);
            if (dynamic.at(nextMask).at(i).at(teleport - 1) != oldCost)
                FindMinRoad(dynamic, roadLengths, n, nextMask, i, teleport - 1);
        }
        
        if (roadLengths.at(visit).at(i) != -1) {
            auto newCost = result + roadLengths.at(visit).at(i);
            auto oldCost = dynamic.at(nextMask).at(i).at(teleport);
            dynamic.at(nextMask).at(i).at(teleport) = min(oldCost, newCost);
            if (dynamic.at(nextMask).at(i).at(teleport) != oldCost)
                FindMinRoad(dynamic, roadLengths, n, nextMask, i, teleport);
        }
    }
}
 
int main(int argc, const char * argv[]) {
    size_t n, m, k;
    cin >> n >> m >> k;
 
    auto cities = map<string, LL>();
    for (size_t i = 0; i < n; ++i) {
        string city;
        cin >> city;
        cities[city] = i;
    }
 
    auto roadsLengths = TwoDimensionalMatrix(n, Vector(n, -1));
    for (size_t i = 0; i < m; ++i) {
        string city0, city1;
        LL length;
        cin >> city0 >> city1 >> length;
        roadsLengths.at(cities.at(city0)).at(cities.at(city1)) = length;
        roadsLengths.at(cities.at(city1)).at(cities.at(city0)) = length;
    }
    
    auto dynamic = ThreeDimensionalMatrix((LL)1 << n, vector<Vector>(n, Vector(k + 1, LONG_LONG_MAX)));
    auto startMasks = Vector();
    for (size_t i = 0; i < n; ++i) {
        startMasks.push_back((LL)1 << i);
        dynamic.at(startMasks.back()).at(i).at(k) = 0;
    }
 
    for (size_t i = 0; i < n; ++i)
        FindMinRoad(dynamic, roadsLengths, n, startMasks.at(i), i, k);
 
    auto answer = LONG_LONG_MAX;
    auto check = dynamic.back();
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j <= k; ++j)
            answer = min(answer, check.at(i).at(j));
 
    print_with_endl((answer == LONG_LONG_MAX ? -1 : answer));
    
    return 0;
}

