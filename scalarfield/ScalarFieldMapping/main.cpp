#include <QtCore/QCoreApplication>
#include <QDebug>

#include "vtkParticleReader.h"
#include "vtkCellLocator.h"
#include "vtkPointLocator.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkCleanPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkIdList.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //*************************************************************************************
    // usage: ./ScalarFieldMapping input_mesh input_pointcloud output_mesh
    //*************************************************************************************

    if (argc < 3)
    {
        qDebug() << "********************************************************************";
        qDebug() << " USAGE: ./ScalarFieldMapping input_mesh input_pointcloud output_mesh";
        qDebug() << "********************************************************************";
        return 0;
    }

    vtkSmartPointer<vtkXMLPolyDataReader> reader_mesh = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    vtkSmartPointer<vtkParticleReader> reader_pointcloud = vtkSmartPointer<vtkParticleReader>::New();


    QString filename1 = argv[1];
    QString filename2 = argv[2];

    //filename1 = "scan000-mesh.vtp";   // mesh
    //filename2 = "scan000-pointcloud.vtp";  // point cloud
    //qDebug() << filename1;
    reader_mesh->SetFileName(filename1.toAscii());
    reader_mesh->Update();

    //qDebug() << "mesh:" << reader_mesh->GetNumberOfCells();

    reader_pointcloud->SetFileName(filename2.toAscii());
    reader_pointcloud->Update();


    vtkSmartPointer<vtkCleanPolyData> cleaned_polydata = vtkSmartPointer<vtkCleanPolyData>::New();
    cleaned_polydata->SetInputConnection(reader_pointcloud->GetOutputPort());
    cleaned_polydata->Update();

    vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
    cellLocator->SetDataSet(reader_mesh->GetOutput());
    cellLocator->BuildLocator();

    //double testPoint1[3] = {QString(argv[2]).toFloat(), QString(argv[3]).toFloat(), QString(argv[4]).toFloat()};

    //double testPoint[3] = {2.0, 0.0, 0.0};

    vtkSmartPointer<vtkFloatArray> weight = vtkFloatArray::SafeDownCast(reader_pointcloud->GetOutput()->GetPointData()->GetScalars());
    vtkSmartPointer<vtkFloatArray> weight_mesh = vtkFloatArray::SafeDownCast(reader_mesh->GetOutput()->GetPointData()->GetScalars());



    qDebug() << "number of points:" << weight->GetDataSize();
    //    qDebug() << "weight_mesh:" << weight_mesh->GetDataSize();
    //    QList<float> HighTemperatureScalars;

    double *range = weight->GetRange();
    double min = range[0];
    double max = range[1];

    double avg = 0.5*(max+min);

    for (vtkIdType i = 0; i < weight_mesh->GetNumberOfTuples(); i++)
    {
        weight_mesh->SetValue(i, min);
    }

    qDebug() << "Initialized!";
    qDebug() << "min temperature: " << min;
    qDebug() << "max temperature: " << max;
    qDebug() << "average temperature: " << avg;

    qDebug() << weight->GetNumberOfTuples();
    qDebug() << weight_mesh->GetNumberOfTuples();
    double mindist = 1000000;
    double maxdist = 0;

    qDebug() << "Mapping scalars onto mesh...";

    for (vtkIdType i = 0; i < weight->GetNumberOfTuples(); i++)
    {
        if (weight->GetValue(i)>-20)
        {
            double testPoint[3];
            reader_pointcloud->GetOutput()->GetPoint(i,testPoint);
            //qDebug() << testPoint;
            double closestPoint[3];
            double closestPointDist2;
            vtkIdType cellId;
            int subId;

            cellLocator->FindClosestPoint(testPoint, closestPoint, cellId, subId, closestPointDist2);
            if (closestPointDist2 < mindist)
                mindist = closestPointDist2;
            if (closestPointDist2 > maxdist)
                maxdist = closestPointDist2;
            vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
            reader_mesh->GetOutput()->GetCellPoints(cellId, cellPointIds);

            //qDebug() << "Ids: " << cellPointIds->GetNumberOfIds();
            if (weight->GetValue(i) > weight_mesh->GetValue(cellPointIds->GetId(0)))
                weight_mesh->SetValue(cellPointIds->GetId(0), weight->GetValue(i));
            if (weight->GetValue(i) > weight_mesh->GetValue(cellPointIds->GetId(1)))
                weight_mesh->SetValue(cellPointIds->GetId(1), weight->GetValue(i));
            if (weight->GetValue(i) > weight_mesh->GetValue(cellPointIds->GetId(2)))
                weight_mesh->SetValue(cellPointIds->GetId(2), weight->GetValue(i));

            if (weight->GetValue(i)<=min)
            {
                if (weight->GetValue(i) > weight_mesh->GetValue(cellPointIds->GetId(0)))
                    weight_mesh->SetValue(cellPointIds->GetId(0), avg);
                if (weight->GetValue(i) > weight_mesh->GetValue(cellPointIds->GetId(1)))
                    weight_mesh->SetValue(cellPointIds->GetId(1), avg);
                if (weight->GetValue(i) > weight_mesh->GetValue(cellPointIds->GetId(2)))
                    weight_mesh->SetValue(cellPointIds->GetId(2), avg);
            }

        }
    }
    //    qDebug() << HighTemperatureScalars.count();
    //    int k = 0;
    //    for (vtkIdType i = 0; i < weight_mesh->GetNumberOfTuples(); i++)
    //    {
    //        if (weight_mesh->GetValue(i)>50) { qDebug() << weight_mesh->GetValue(i); k++;}
    //    }
    reader_mesh->GetOutput()->GetPointData()->SetScalars(weight_mesh);
    qDebug() << mindist << maxdist;
    vtkSmartPointer<vtkXMLPolyDataWriter> writer_mesh = vtkSmartPointer<vtkXMLPolyDataWriter>::New();

    QString outfile = argv[3];
    writer_mesh->SetFileName(outfile.toAscii());
    writer_mesh->SetInput(reader_mesh->GetOutput());
    writer_mesh->Write();

    return 0;
}
