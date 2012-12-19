#ifndef VTKFUZZYCMEANS_H
#define VTKFUZZYCMEANS_H

#endif // VTKFUZZYCMEANS_H

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vector>

int fcm( vtkSmartPointer<vtkPolyData> data, const int& clusters_n = 2, const double& expo = 2.0, const int& max_iter = 10, const double& min_impro = 0.00001 );

int init_fcm ( const int& clusters_n, const int& N_points, std::vector< std::vector<double> > &U);

double CalculateDistance(const double &x1,
                         const double &y1,
                         const double &z1,
                         const double &x2,
                         const double &y2,
                         const double &z2);

double step_fcm (vtkSmartPointer<vtkPolyData> data, std::vector< std::vector<double> > &U, const int& clusters_n, const double& expo);
double max(const double& x, const double& y);
int max3(const std::vector <double>& x);
