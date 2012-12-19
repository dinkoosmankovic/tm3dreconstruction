#include <QtCore/QCoreApplication>

#include <vtkXMLPolyDataReader.h>
#include <vtkX3DExporter.h>
#include <vtkVRMLExporter.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 3)
    {
        qDebug() << "++++++++++++++++++++++++++++++++++++++++++++++";
        qDebug() << "USAGE: ./ModelExporter input output type";
        qDebug() << "type = x3d or vrml";
        qDebug() << "++++++++++++++++++++++++++++++++++++++++++++++";
        return 0;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string modelType = argv[3];

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);

    vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName(inputFile.c_str());
    reader->Update();

    if (modelType == "vrml")
    {
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(reader->GetOutputPort());
        mapper->Update();

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        renderer->AddActor(actor);
        renWin->Render();

        vtkSmartPointer<vtkVRMLExporter> vrmlExporter = vtkSmartPointer<vtkVRMLExporter>::New();
        vrmlExporter->SetInput(renWin);
        vrmlExporter->SetFileName(outputFile.c_str());
        vrmlExporter->Write();

    }
    else if (modelType == "x3d")
    {
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(reader->GetOutputPort());
        mapper->Update();

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        renderer->AddActor(actor);
        renWin->Render();

        vtkSmartPointer<vtkX3DExporter> x3dExporter = vtkSmartPointer<vtkX3DExporter>::New();
        x3dExporter->SetInput(renWin);
        x3dExporter->SetFileName(outputFile.c_str());
        x3dExporter->Write();

    }
    else
    {
        qDebug() << "Wrong model type!";
        return 0;
    }
    
    return 0;
}
