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
#include <vtkVertexGlyphFilter.h>
#include "vtkSelectionNode.h"
#include "vtkSelection.h"
#include "vtkExtractSelection.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkKMeansClustering.h"

double Magnitude(double *point);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 5)
    {
        qDebug() << "--------------------------------";
        qDebug() << "USAGE: ./LeakDetection input output number_of_clusters det";
        qDebug() << "det = 1 for leaks of det = 2 for heat sources";
        qDebug() << "--------------------------------";
        return 0;
    }

    std::string inputFileName = argv[1];
    std::string outpuFileName = argv[2];
    std::string strK = argv[3];
    int numK = atoi(strK.c_str());

    std::string strDet = argv[4];
    int det = atoi(strDet.c_str());



    vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    //reader->SetFileName("/home/dinko/Desktop/scan005-pointcloud.vtp");
    reader->SetFileName(inputFileName.c_str());
    reader->Update();


    vtkSmartPointer<vtkPoints> trans_points = vtkSmartPointer<vtkPoints>::New();

    for (vtkIdType i = 0; i < reader->GetOutput()->GetPointData()->GetArray("Scalars_")->GetSize(); i++)
    {
        trans_points->InsertNextPoint(reader->GetOutput()->GetPointData()->GetArray("Scalars_")->GetTuple1(i),0.0,0.0);
    }

    vtkSmartPointer<vtkPolyData> trans_polydata = vtkSmartPointer<vtkPolyData>::New();
    trans_polydata->SetPoints(trans_points);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexGlyphFilter->AddInput(trans_polydata);
    vertexGlyphFilter->Update();

//    vtkSmartPointer<vtkArrayCalculator> calc = vtkSmartPointer<vtkArrayCalculator>::New();
//    //calc->DebugOn();
//    calc->SetInput(vertexGlyphFilter->GetOutput());
//    calc->SetAttributeModeToUsePointData();
//    calc->AddCoordinateVectorVariable("coords");
//    calc->SetResultArrayName("magnitudes");
//    //calc->AddVectorArrayName("coords");
//    //calc->SetFunction("Scalars_");
//    calc->Update();

//    calc->GetOutput()->GetPointData()->RemoveArray("Scalar");

    vtkSmartPointer<vtkKMeansClustering> clusters = vtkSmartPointer<vtkKMeansClustering>::New();
    clusters->AddInput(vertexGlyphFilter->GetOutput());
    clusters->SetInitMethod(vtkKMeansClustering::RANDOM);
    clusters->SetK(numK);

    clusters->Update();

    vtkSmartPointer<vtkPolyData> labels = vtkSmartPointer<vtkPolyData>::New();
    labels->ShallowCopy(clusters->GetOutput(0));


    vtkSmartPointer<vtkPolyData> centers = vtkSmartPointer<vtkPolyData>::New();
    centers->ShallowCopy(clusters->GetOutput(1));

    qDebug() << "Centers: " <<centers->GetNumberOfPoints();


    double *smallest; double smallest_mag;
    //centers->GetPoints()->GetPoint(0,smallest);
    if (det == 2)
        smallest = centers->GetPoint(0);
    else if (det == 1)
        smallest = centers->GetPoint(numK);
    else
    {
        qDebug() << "Wrong type of detection!";
        return 0;
    }

    smallest_mag = Magnitude(smallest);
    int closest_cluster_id = 0;

    for (int i = 1; i < centers->GetNumberOfPoints(); i++)
    {
        double mag = Magnitude(centers->GetPoint(i));
        if (mag > smallest_mag)
        {
            smallest_mag = mag;
            closest_cluster_id = i;
        }

    }

    qDebug() << "Closest cluster ID: " << closest_cluster_id;


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


    vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
    selectionNode->SetFieldType(vtkSelectionNode::POINT);
    selectionNode->SetContentType(vtkSelectionNode::INDICES);
    selectionNode->SetSelectionList(ids);

    vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(selectionNode);


    reader->GetOutput()->GetPointData()->AddArray(clusters->GetOutput()->GetPointData()->GetArray(1));

    vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();

    extractSelection->SetInput(0, reader->GetOutput());
    extractSelection->SetInput(1, selection);
    extractSelection->Update();

    vtkSmartPointer<vtkPolyData> extracted = vtkSmartPointer<vtkPolyData>::New();
    extracted->SetPoints(inner_clusters);
    extracted->GetPointData()->SetScalars(cluster_no);



    vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    surfaceFilter->SetInput(extractSelection->GetOutput());
    surfaceFilter->Update();

//    vtkSmartPointer<vtkXMLPolyDataWriter> writer1 = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
//    writer1->SetInput(clusters->GetOutput());
//    //writer1->SetInput(labels);
//    writer1->SetFileName("output1.vtp");
//    writer1->Write();


    vtkSmartPointer<vtkXMLPolyDataWriter> writer2 = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    //writer1->SetInput(surfaceFilter->GetOutput());
    writer2->SetInputConnection(surfaceFilter->GetOutputPort());
    writer2->SetFileName(outpuFileName.c_str());
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
