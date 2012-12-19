#include <QtCore/QCoreApplication>
#include <QDebug>
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkSmartPointer.h"
#include "vtkArrayCalculator.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkFloatArray.h"
#include "vtkArray.h"
#include "vtkDataArray.h"
#include "vtkIdTypeArray.h"
#include "vtkSelectionNode.h"
#include "vtkSelection.h"
#include "vtkExtractSelection.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkKMeansClustering.h"

double Magnitude(double *point);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    //reader->SetFileName("/home/dinko/Desktop/scan005-pointcloud.vtp");
    reader->SetFileName("out-scalars.vtp");
    reader->Update();

    vtkSmartPointer<vtkArrayCalculator> calc = vtkSmartPointer<vtkArrayCalculator>::New();
    //calc->DebugOn();
    calc->SetInput(reader->GetOutput());
    calc->SetAttributeModeToUsePointData();
    calc->AddCoordinateVectorVariable("coords");
    calc->SetResultArrayName("magnitudes");
    //calc->AddVectorArrayName("coords");
    calc->SetFunction("mag(coords)");
    calc->Update();

    calc->GetOutput()->GetPointData()->RemoveArray("Scalar");

    qDebug() << "pass 1";

    vtkSmartPointer<vtkKMeansClustering> clusters = vtkSmartPointer<vtkKMeansClustering>::New();
    clusters->AddInput(calc->GetOutput());
    clusters->SetInitMethod(vtkKMeansClustering::RANDOM);
    clusters->SetK(3);

    clusters->Update();

    qDebug() << "pass 2";
    vtkSmartPointer<vtkPolyData> labels = vtkSmartPointer<vtkPolyData>::New();
    labels->ShallowCopy(clusters->GetOutput(0));

    vtkSmartPointer<vtkPolyData> centers = vtkSmartPointer<vtkPolyData>::New();
    centers->ShallowCopy(clusters->GetOutput(1));

    double smallest_mag = Magnitude(centers->GetPoint(0));
    int closest_cluster_id = 0;
    for (int i = 1; i < centers->GetNumberOfPoints(); i++)
    {
        double mag = Magnitude(centers->GetPoint(i));
        qDebug() << "magnitude: " << mag;
        if (mag < smallest_mag)
        {
            smallest_mag = mag;
            closest_cluster_id = i;
        }

    }

    qDebug() << "Closest cluster ID: " << closest_cluster_id;

    qDebug() << "pass 3";

    vtkSmartPointer<vtkPoints> inner_clusters = vtkSmartPointer<vtkPoints>::New();
    vtkDataArray* cluster_no = labels->GetPointData()->GetArray("ClusterId");

    qDebug() << labels->GetPointData()->GetArray("ClusterId")->GetNumberOfComponents();
    qDebug() << labels->GetPointData()->GetArray("ClusterId")->GetNumberOfTuples();


    vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
    ids->SetNumberOfComponents(1);

    for (unsigned int i = 0; i < labels->GetPointData()->GetArray("ClusterId")->GetNumberOfTuples(); i++)
    {
        if (labels->GetPointData()->GetArray("ClusterId")->GetTuple(i)[0] == closest_cluster_id)
            ids->InsertNextValue(i);
    }

    qDebug() << "pass 4";

    vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
    selectionNode->SetFieldType(vtkSelectionNode::POINT);
    selectionNode->SetContentType(vtkSelectionNode::INDICES);
    selectionNode->SetSelectionList(ids);

    vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(selectionNode);

    qDebug() << "pass 5";

    vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();

    extractSelection->SetInput(0, reader->GetOutput());
    extractSelection->SetInput(1, selection);
    extractSelection->Update();

    qDebug() << "pass 6";

//    vtkSmartPointer<vtkPolyData> extracted = vtkSmartPointer<vtkPolyData>::New();
//    extracted->SetPoints(inner_clusters);
//    extracted->GetPointData()->SetScalars(cluster_no);

    vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    surfaceFilter->SetInput(extractSelection->GetOutput());
    surfaceFilter->Update();

    vtkSmartPointer<vtkXMLPolyDataWriter> writer1 = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer1->SetInput(surfaceFilter->GetOutput());
    //writer1->SetInput(labels);
    writer1->SetFileName("output1.vtp");
    writer1->Write();

    vtkSmartPointer<vtkXMLPolyDataWriter> writer2 = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    //writer1->SetInput(surfaceFilter->GetOutput());
    writer2->SetInput(labels);
    writer2->SetFileName("output2.vtp");
    writer2->Write();


    return 0;
}

double Magnitude(double *point)
{
    double x = point[0];
    double y = point[1];
    double z = point[2];

    return sqrt(x*x+y*y+z*z);

}
