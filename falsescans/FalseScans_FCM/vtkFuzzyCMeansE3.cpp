#include <vtkFuzzyCMeansE3.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include <vector>
#include <Eigen/Dense>

#include <QDebug>

using namespace Eigen;


typedef Matrix<double, Dynamic, Dynamic> MatrixXd;


int fcm ( vtkSmartPointer<vtkPolyData> data, const int& cluster_n, const double& expo, const int& max_iter, const double& min_impro)
{
    qDebug() << "pass1";
    int data_size = data->GetPoints()->GetNumberOfPoints();
    std::cout << cluster_n << " " << data_size << endl << endl << endl;
    MatrixXd U = init_fcm(cluster_n, data_size);
    double old_obj_func = 0.0;
    double obj_func = 0.0;
    int run_iterations = 1;

    for (int i = 0; i < max_iter; i++)
    {
        MatrixXd new_U = stepfcm(data, obj_func, U, cluster_n, expo);
        std::cout << "Objective at "<< run_iterations <<"-th iteration is:\t" << obj_func<< endl;
        if (abs(obj_func - old_obj_func) < min_impro && i > 2)
        {
            break;
        }
        old_obj_func = obj_func;
        U = new_U;
        run_iterations++;
    }

    std::cout << "-------------------------------------" << endl;
    std::cout << "Stopped at "<< run_iterations <<"-th iteration " << endl;
    std::cout << "-------------------------------------" << endl;



    vtkSmartPointer<vtkFloatArray> clusters_i = vtkSmartPointer<vtkFloatArray>::New();
    clusters_i->SetName("clusters");
    clusters_i->SetNumberOfTuples(data_size);

//    std::cout << " ----------------------------- " << endl;
//    std::cout << U << endl;
//    std::cout << " ----------------------------- " << endl;

    for (long j = 0; j < U.cols(); j++)
    {
        //std::cout << U[0][i] << " " << U[1][i] << " " << U[2][i] << endl;
        double max_U = -10;
        int max_index = 0;

        for (int k = 0; k < U.rows(); k++)
        {
            if (U(k,j) > max_U)
            {
                max_U = U(k,j);
                max_index = k;
            }
        }
        //std::cout << max_index << " ";
        clusters_i->SetTuple1(j, max_index + 1);
    }
    data->GetPointData()->AddArray(clusters_i);
    data->GetPointData()->SetActiveScalars("clusters");

    //std::cout << endl;

    return 1;

}

MatrixXd init_fcm (const int& cluster_n, const int& data_size)
{
    MatrixXd initial = MatrixXd::Random(cluster_n, data_size);
    MatrixXd col_sum = initial.colwise().sum();

    for (int i = 0; i < cluster_n; i++)
    {
        for (long j = 0; j < data_size; j++)
        {
            initial(i,j) /= col_sum(0,j);
        }
    }

    return initial;
}


MatrixXd stepfcm(vtkSmartPointer<vtkPolyData>data, double& obj_func, const MatrixXd& U, const int& cluster_n, const double& expo)
{
    MatrixXd mf = U.array().pow(expo);

    int data_size = data->GetPoints()->GetNumberOfPoints();
    int vector_size = 3;
    MatrixXd data_matrix(data_size, vector_size);

    double *range = data->GetPointData()->GetArray("magnitudes")->GetRange();


    for (long i = 0; i < data_size; i++)
    {
//        double *point = data->GetPoint(i);
//        data_matrix(i,0) = point[0];
//        data_matrix(i,1) = point[1];
//        data_matrix(i,2) = point[2];

        double value = data->GetPointData()->GetArray("magnitudes")->GetTuple1(i);
        data_matrix(i,0) = value;
    }



    MatrixXd temp = MatrixXd::Constant(vector_size,1,1.0);
    temp *= mf.transpose().colwise().sum();
    MatrixXd tempT = (temp.transpose()).array() + 1;

    //std::cout << data_matrix << endl;
    //std::cout << tempT << endl;

    //std::cout << " +++++++++++++centers+++++++++++++++ " << endl;

    MatrixXd centers = (mf*data_matrix).array() / tempT.array();

    //std::cout << range[0] << " " << range[1] << " " << centers.array().maxCoeff() << endl;
    centers = centers.array() * (range[1]-range[0])/(2*centers.array().maxCoeff());

    //std::cout << centers << endl;

    //std::cout << " +++++++distances+++++++++++++++++++++ " << endl;

    MatrixXd distances = CalculateDistances(centers,data_matrix);

    //std::cout << distances << endl;

    //std::cout << " ++++++++++++++++++++++++++++ " << endl;

    obj_func = CalculateObjective(distances, mf);

    //std::cout << obj_func << endl;
    //std::cout << " ++++++++++++++++++++++++++++ " << endl;

    MatrixXd tmp = distances.array().pow(-2.0/(expo-1));

    tmp = tmp.array() / (MatrixXd::Constant(cluster_n,1,1.0)*tmp.colwise().sum()).array();



    return tmp;
}

MatrixXd CalculateDistances(const MatrixXd& centers, const MatrixXd& data_matrix)
{
    MatrixXd distances(centers.rows(), data_matrix.rows());
    if (centers.cols() > 1)
    {
        for (int i = 0; i < centers.rows(); i++)
        {
            for (long j = 0; j < data_matrix.rows(); j++)
            {
                distances(i,j) = sqrt( pow(data_matrix(j,0) - centers(i,0),2) +
                                       pow(data_matrix(j,1) - centers(i,2),2) +
                                       pow(data_matrix(j,2) - centers(i,2),2)  );
            }
        }
    }
    else
    {
        //std::cout << "vector size is 1" << endl;
        for (int i = 0; i < centers.rows(); i++)
        {
            for (long j = 0; j < data_matrix.rows(); j++)
            {
                distances(i,j) =  fabs( centers(i,0) - data_matrix(j,0) );
            }
        }
    }

    return distances;

}

double CalculateObjective(const MatrixXd& distances, const MatrixXd& mf)
{
    MatrixXd temp = distances.array().pow(2)*mf.array();

    //std::cout << temp << endl;

    return temp.sum();
}
