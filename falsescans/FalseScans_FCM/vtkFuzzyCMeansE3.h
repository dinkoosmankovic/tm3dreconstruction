#ifndef VTKFUZZYCMEANSE3_H
#define VTKFUZZYCMEANSE3_H

#endif // VTKFUZZYCMEANSE3_H

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;

//typedef Matrix<double, Dynamic, Dynamic> MatrixXd;

int fcm( vtkSmartPointer<vtkPolyData> data, const int& clusters_n = 2, const double& expo = 2.0, const int& max_iter = 10, const double& min_impro = 1e-5 );

MatrixXd init_fcm (const int& cluster_n, const int& data_size);

MatrixXd stepfcm(vtkSmartPointer<vtkPolyData>data, double& obj_func, const MatrixXd& U, const int& cluster_n, const double& expo);

MatrixXd CalculateDistances(const MatrixXd& centers, const MatrixXd& data_matrix);

double CalculateObjective(const MatrixXd& distances, const MatrixXd& mf);
