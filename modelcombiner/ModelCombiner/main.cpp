#include <QtCore/QCoreApplication>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkSmartPointer.h>
#include <vtkCleanPolyData.h>
#include <vtkAppendPolyData.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2)
    {
        qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        qDebug() << "USAGE: ./ModelCombiner output input1 input2 input3 ...";
        qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        return 0;
    }

    qDebug() << argv[0];
    qDebug() << argv[1];

    std::string outputFileName = argv[1];

    vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    for (int i = 2; i < argc; i++)
    {
        qDebug() << argv[i];

        std::string inputFileName = argv[i];
        vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
        reader->SetFileName(inputFileName.c_str());
        reader->Update();

        appendFilter->AddInput(reader->GetOutput());
        appendFilter->Update();


    }

    vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
    cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
    cleanFilter->Update();

    qDebug() << "-------------------------";
    qDebug() << "Model combining finished. Writing to the file...";

    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(outputFileName.c_str());
    writer->SetInputConnection(cleanFilter->GetOutputPort());
    writer->Write();

    
    return 0;
}
