#include <vtkFuzzyCMeans.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include <ctime>
#include <QDebug>
#include <vector>
#include <cmath>

using namespace std;

int fcm( vtkSmartPointer<vtkPolyData> data, const int& clusters_n, const double& expo, const int& max_iter, const double& min_impro)
{
    int data_size = data->GetPoints()->GetNumberOfPoints();
    vector< vector<double> > U(clusters_n, vector<double>(data_size));

    double *range = data->GetPointData()->GetArray("magnitudes")->GetRange();

    std::cout << "range is " << range[0] << " to " << range[1] << endl;

    int temp = init_fcm(clusters_n,data_size,U);


    /*std::cout << "++++++++++++++++" << endl;

    for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < data_size; j++)
        {
            std::cout << U[i][j] << " ";
        }
        std::cout << endl;
    }
    std::cout << "++++++++++++++++" << endl;*/

    double obj_func = 0.0;
    double new_obj_func = obj_func;
    int k = 0;

    for (int i = 0; i < max_iter; i++)
    {
        k = i;
        new_obj_func = step_fcm (data, U, clusters_n, expo);
        std::cout <<  "Objective at " << k << "-th iteration is " << new_obj_func << endl;
        if (abs(new_obj_func - obj_func) < min_impro)
        {
            break;
        }
        obj_func = new_obj_func;

    }
    std::cout << endl << "------------------------------------" << endl;
    std::cout << "Stopped at " << k << "-th iteration\n";

    // ONLY FOR DISPLAYING U MATRIX

    /*for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < data_size; j++)
        {
            std::cout << U[i][j] << " ";
        }
        std::cout << endl;
    }*/



    vtkSmartPointer<vtkFloatArray> clusters_i = vtkSmartPointer<vtkFloatArray>::New();
    clusters_i->SetName("clusters");
    clusters_i->SetNumberOfTuples(data_size);

    for (long i = 0; i < data_size; i++)
    {
        //std::cout << U[0][i] << " " << U[1][i] << " " << U[2][i] << endl;
        vector<double> col_U(clusters_n);
        for (int k = 0; k < clusters_n; k++)
            col_U[k] = U[k][i];



        //std::cout  << temp1 << " " << temp2 << endl;
        int temp3 = max3(col_U);
        //std::cout << temp3 << endl;
        clusters_i->SetTuple1(i, temp3);
    }
    data->GetPointData()->AddArray(clusters_i);
    data->GetPointData()->SetActiveScalars("clusters");

    std::cout << endl;


    return 1;
}

int init_fcm ( const int& clusters_n, const int& N_points, vector< vector<double> > &U)
{

    srand(time(NULL));
    for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < N_points; j++)
        {
            U[i][j] = rand()/double(RAND_MAX);
            //std::cout << U[i][j] << " ";
        }
        //std::cout << endl;
    }

    //std::cout << "+++++++++++++" << endl;
    vector<double> col_sum(N_points);
    for (int i = 0; i < N_points; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < clusters_n; j++)
        {
            sum += U[j][i];
        }
        col_sum[i] = sum;
    }

    for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < N_points; j++)
        {
            U[i][j] /= col_sum[j];
        }
    }

    return 1;
}


double CalculateDistance(const double &x1,
                         const double &y1,
                         const double &z1,
                         const double &x2,
                         const double &y2,
                         const double &z2)
{
    double dx = x1-x2;
    double dy = y1-y2;
    double dz = z1-z2;
    return sqrt( dx*dx + dy*dy + dz*dz );
}

double step_fcm (vtkSmartPointer<vtkPolyData> data, std::vector< std::vector<double> > &U, const int& clusters_n, const double& expo)
{

    std::setw(4);
    int data_size = data->GetPoints()->GetNumberOfPoints();
    vector< vector<double> > mf(clusters_n, vector<double>(data_size) );
    double *range = data->GetPointData()->GetArray("magnitudes")->GetRange();

    double obj_func = 0.0;

    int vector_size = 1;

    vector< vector<double> > mfxdata(clusters_n, vector<double>(vector_size));

    vector <double> sum_mf(clusters_n);

    for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < data_size; j++)
        {
            mf[i][j] = pow(U[i][j], expo);
          //  std::cout << mf[i][j] << " ";
        }
        //std::cout << endl;
    }

    /*for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < vector_size; j++)
        {
            double sum = 0;
            for (long k = 0; k < data_size ; k++)
            {
                double point;
                point = data->GetPointData()->GetArray("magnitudes")->GetTuple1(k);
                //std::cout << point << " ";
                sum += mf[i][k] * point;
            }
            std::cout << endl;
            mfxdata[i][j] = sum;
            std::cout << mfxdata[i][j] << " ";
        }
        std::cout << endl;
    }*/

    for (int i = 0; i < clusters_n; i++)
    {
        double sum = 0.0;
        for (long j = 0; j < data_size; j++)
        {
            double mag = data->GetPointData()->GetArray("magnitudes")->GetTuple1(j);
            mag /= range[1]-range[0];
            //if (i == 0) std::cout << " magnitude: " << mag << endl;
            sum += mf[i][j] * mag;
        }
        mfxdata[i][0] = sum;
        //std::cout << mfxdata[i][0] << endl;
    }
    //std::cout << endl;




    //std::cout << "+++++++++++++++++++" << endl;

    for (int i = 0; i < clusters_n; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < data_size; j++)
        {
            sum += mf[i][j];
        }
        sum_mf[i] = sum;
    }


    std::cout << "++++++++centers+++++++++" << endl;
    for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < vector_size; j++)
        {
            mfxdata[i][j] /= sum_mf[i];
            std::cout << mfxdata[i][j] << " ";
        }
        std::cout << endl;
    }
    //std::cout << "++++++++centers+++++++++" << endl;

    vector< vector<double> > distances(clusters_n, vector<double>(data_size));

    vector<double> magnitudes(data_size);

    for (long i = 0; i < data_size; i++)
    {
        magnitudes[i] = data->GetPointData()->GetArray("magnitudes")->GetTuple1(i)/(range[1]-range[0]);
        //std::cout << "mag(" << i << ") " << magnitudes[i] << endl;
    }

    //std::cout << "++++++++++++distances+++++++++++" << endl;
    for (int i = 0; i < clusters_n; i++)
    {
        for (long j = 0; j < data_size; j++)
        {
            //double mag = data->GetPointData()->GetArray("magnitudes")->GetTuple1(j);
            //distances[i][j] = CalculateDistance(p,p,p, mfxdata[i][0],mfxdata[i][1], mfxdata[i][2]);
            distances[i][j] =  abs(magnitudes[j] - mfxdata[i][0]);
            //std::cout << distances[i][j] << " ";
        }
        //std::cout << endl;
    }
    //std::cout << "++++++++++++distances+++++++++++" << endl;

    vector< vector<double> > dist2xmf(clusters_n, vector<double>(data_size));

    obj_func = 0.0;
    for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < data_size; j++)
        {
            dist2xmf[i][j] = distances[i][j]*mf[i][j];
            obj_func += dist2xmf[i][j];
        }
    }

    //std::cout << "objective: " << obj_func << endl;
    vector< vector<double> > tmp(clusters_n, vector<double>(data_size));
    for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < data_size; j++)
        {
            tmp[i][j] = pow(distances[i][j],-2/(expo-1));
        }
    }

    vector< double> sumtmp(data_size);

    for (int i = 0; i < data_size; i++)
    {
        double sum_tmp = 0.0;
        for (int j = 0; j < clusters_n; j++)
        {
            sum_tmp += tmp[j][i];
        }
        sumtmp[i] = sum_tmp;
    }
    //std::cout << "+++++++++++++" << endl;

    for (int i = 0; i < clusters_n; i++)
    {
        for (int j = 0; j < data_size; j++)
        {
            U[i][j] = tmp[i][j] / sumtmp[j];
            //std::cout << U[i][j] << " ";
        }
        //std::cout << endl;
    }
    //std::cout << "+++++++++++++" << endl;

    return obj_func;
}

double max(const double& x, const double& y)
{
    if (x > y)
        return x;
    else
        return y;
}

int max3(const vector<double>& x)
{
    double max = 0;
    int max_i = 0;
    for (int i = 0; i < x.size(); i++)
    {
        if (x[i] > max)
        {
            max = x[i];
            max_i = i;
        }
    }
    return max_i;
}
