#include <QtCore/QCoreApplication>

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPointSource.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPointData.h>
#include <vtkArrayCalculator.h>
#include <QDebug>
#include <vtkFuzzyCMeansE3.h>
#include <Eigen/Dense>

using namespace Eigen;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    vtkSmartPointer<vtkPointSource> test_cloud = vtkSmartPointer<vtkPointSource>::New();
//    test_cloud->SetCenter(0,0,0);
//    test_cloud->SetNumberOfPoints(1000);
//    test_cloud->SetRadius(5.0);
//    test_cloud->Update();

    vtkSmartPointer<vtkXMLPolyDataReader> test_cloud = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    test_cloud->SetFileName("out-scalars.vtp");
    test_cloud->Update();

    vtkSmartPointer<vtkArrayCalculator> calc = vtkSmartPointer<vtkArrayCalculator>::New();
    //calc->DebugOn();
    calc->SetInput(test_cloud->GetOutput());
    calc->SetAttributeModeToUsePointData();
    calc->AddCoordinateVectorVariable("coords");
    calc->SetResultArrayName("magnitudes");
    //calc->AddVectorArrayName("coords");
    calc->SetFunction("mag(coords)");
    calc->Update();

    //test_cloud->GetOutput()->GetPointData()->AddArray(calc->getp)

    qDebug() << test_cloud->GetOutput()->GetPoints()->GetNumberOfPoints();

    int t =  fcm(calc->GetPolyDataOutput(), 2,2,100,0.3);

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();

    writer->SetFileName("test_cloud.vtp");
    writer->SetInputConnection(calc->GetOutputPort());
    writer->Write();



    //qDebug() << t;


    return 0;
}
