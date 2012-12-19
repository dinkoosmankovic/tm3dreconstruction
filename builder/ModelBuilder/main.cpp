#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QString>
#include <QElapsedTimer>

#include "vtkSmartPointer.h"
#include "vtkSimplePointsReader.h"
#include "vtkPolyData.h"
#include "vtkGaussianSplatter.h"
#include "vtkMarchingCubes.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkDecimatePro.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkParticleReader.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //*******************************************************************************
    //  usage: ./ModelBuilder input_file SampleDimensions Reduction ExponentFactor output_file
    //
    // Defaults:  SampleDimensions = 50, Reduction = 0.0, ExponentFactor = 0.1
    //*******************************************************************************

    int SampleDimensions = 50;
    double Reduction = 0.0;
    double ExponentFactor = 0.1;

    qDebug() << argc;

    if (argc > 2)
    {
        SampleDimensions = QString(argv[2]).toInt();
    }
    if (argc > 3)
    {
        Reduction = QString(argv[3]).toDouble();
    }
    if (argc > 4)
    {
        ExponentFactor = QString(argv[4]).toDouble();
    }

    qDebug() << argc << SampleDimensions << Reduction << ExponentFactor;

    vtkSmartPointer <vtkParticleReader> reader = vtkSmartPointer<vtkParticleReader>::New();
    vtkSmartPointer<vtkPolyData> input;

    vtkSmartPointer<vtkXMLPolyDataReader> reader1 = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader1->SetFileName("scan005-pointcloud.vtp");
    reader1->Update();

    // File input as argument

    if (argc > 1)
    {
        printf("%s", argv[1]);
        reader->SetFileName(argv[1]);
        reader->Update();
        input = reader->GetOutput();

    }
    else
    {
        // Exit if no file was passed

        return 0;
    }
    QElapsedTimer myTimer;
    myTimer.start();
    // Creating PolyData Object from read points

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(input->GetPoints());
    //polydata->ShallowCopy(decimate->GetOutput());

    // Creating Gaussian splatter from polydata

    vtkSmartPointer<vtkGaussianSplatter> splatter = vtkSmartPointer<vtkGaussianSplatter>::New();
    //splatter->SetInput(polydata);
    splatter->SetInputConnection(reader1->GetOutputPort());
    splatter->SetSampleDimensions(SampleDimensions, SampleDimensions, SampleDimensions);
    splatter->SetExponentFactor(ExponentFactor);
    splatter->SetScaleFactor(10);
    splatter->SetRadius(0.01);
    splatter->ScalarWarpingOn();
    splatter->NormalWarpingOn();

    // Run Marching cubes algorithm across splattered surface to create mesh

    vtkSmartPointer<vtkMarchingCubes> surfaceMesh = vtkSmartPointer<vtkMarchingCubes>::New();
    surfaceMesh->SetNumberOfContours(1);
    surfaceMesh->SetInputConnection(splatter->GetOutputPort());
    surfaceMesh->ComputeNormalsOn();
    surfaceMesh->ComputeGradientsOn();
    surfaceMesh->SetValue(0, 0.1);

    // OPTIONAL **********************************************************************************************

    // Decimation of points

    vtkSmartPointer<vtkDecimatePro> decimator = vtkSmartPointer<vtkDecimatePro>::New();
    decimator->SetFeatureAngle(30);
    decimator->SplittingOff();
    decimator->PreserveTopologyOn();
    decimator->SetMaximumError(1);
    decimator->SetTargetReduction(Reduction);
    decimator->SetInput(surfaceMesh->GetOutput());
    decimator->Update();



    // SmoothPolyDataFilter

    vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    smoother->SetInputConnection(decimator->GetOutputPort());
    smoother->SetNumberOfIterations(500);

    // PolyDataNormals

    vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection(smoother->GetOutputPort());
    normals->FlipNormalsOn();

    // OPTIONAL **********************************************************************************************

    // File output

    vtkSmartPointer<vtkXMLPolyDataWriter> file_output = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    file_output->SetInputConnection(normals->GetOutputPort());

    if (argc > 5)
    {
        qDebug() << QString(argv[5]);
        file_output->SetFileName(QString(argv[5]).toUtf8().constData());
    }
    else
        file_output->SetFileName("output.vtp");

    file_output->DebugOff();
    file_output->Write();

    // Map points from surfaceMesh onto surfaceMeshMapper

    vtkSmartPointer<vtkPolyDataMapper> surfaceMeshMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    //surfaceMeshMapper->SetInputConnection(surfaceMesh->GetOutputPort());
    surfaceMeshMapper->SetInputConnection(normals->GetOutputPort());
    surfaceMeshMapper->ScalarVisibilityOff();

    // Create actor for surfaceMeshMapper

    vtkSmartPointer<vtkActor> surfaceActor = vtkSmartPointer<vtkActor>::New();
    surfaceActor->SetMapper(surfaceMeshMapper);
    surfaceActor->GetProperty()->SetDiffuseColor(1.0000, 0.3882, 0.2784);
    surfaceActor->GetProperty()->SetSpecularColor(1, 1, 1);
    surfaceActor->GetProperty()->SetSpecular(.4);
    surfaceActor->GetProperty()->SetSpecularPower(50);

    // Create renderer, render window, and interactor

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    windowInteractor->SetRenderWindow(renderWindow);


    renderer->AddActor(surfaceActor);
    renderer->SetBackground(1, 1, 1);
    renderWindow->SetSize(400, 400);
    renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    renderer->GetActiveCamera()->SetPosition(1, 0, 0);
    renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(20);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->GetActiveCamera()->Dolly(1.2);
    renderer->ResetCameraClippingRange();



    qDebug() << "Polygons:" << normals->GetOutput()->GetNumberOfPolys() << endl;



    windowInteractor->Initialize();
    renderWindow->SetSize(640,480);
    renderWindow->Render();
    windowInteractor->Start();


    //*******************************************************************************

    return 0;
}
