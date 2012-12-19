#ifndef FUZZYCMEANS_H
#define FUZZYCMEANS_H

#include <vector>

using namespace std;

class FuzzyCMeans
{
public:
    FuzzyCMeans();

private:
    vector<double> training_set;
    vector<double> initial_conditions;
    int m;



};

#endif // FUZZYCMEANS_H
