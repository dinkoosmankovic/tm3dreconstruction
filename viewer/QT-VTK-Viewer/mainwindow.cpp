#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkPolyDataMapper.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkIdList.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkLODActor.h"
#include "vtkPolyData.h"
#include "vtkLookupTable.h"
#include "vtkKdTreePointLocator.h"
#include "vtkInteractorStyleSwitch.h"
#include "vtkCallbackCommand.h"
#include "vtkCommand.h"
#include "vtkTextActor.h"
#include "vtkActor2D.h"
#include "vtkTextProperty.h"
#include "vtkActor2DCollection.h"
#include "vtkCellPicker.h"
#include "vtkCell.h"
#include "vtkCellLocator.h"
#include "vtkPicker.h"
#include <QDebug>
#include <QVTKWidget.h>
#include <QFileDialog>

//static void CallbackFunction ( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData );

vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
vtkSmartPointer<vtkKdTreePointLocator> kdLocator = vtkSmartPointer<vtkKdTreePointLocator>::New();
vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
vtkSmartPointer<vtkFloatArray> weight;

vtkSmartPointer<vtkLookupTable> lut; // COLORMAP Lookup table
vtkSmartPointer<vtkPolyDataMapper> mapper;

double temp_value;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vtkWidget = new QVTKWidget(this,QFlag(0));
    ui->verticalLayout->addWidget(vtkWidget);
    ui->verticalLayout->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnUP_clicked()
{
    int *pos = vtkWidget->GetInteractor()->GetEventPosition();
    vtkWidget->GetInteractor()->SetNumberOfFlyFrames(1);
    vtkWidget->GetInteractor()->SetDesiredUpdateRate(25);
    vtkWidget->GetInteractor()->FlyTo(renderer, pos[0], pos[1], pos[2]+1);
    vtkWidget->setFocus();
    //delete pos;
}

void MainWindow::on_btnDOWN_clicked()
{
    int *pos = vtkWidget->GetInteractor()->GetEventPosition();
    vtkWidget->GetInteractor()->SetNumberOfFlyFrames(1);
    vtkWidget->GetInteractor()->SetDesiredUpdateRate(25);
    vtkWidget->GetInteractor()->FlyTo(renderer, pos[0], pos[1], pos[2]-1);

    vtkWidget->setFocus();
    //delete pos;

}


void MainWindow::CallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData) )
{
    vtkRenderer* renderer = static_cast<vtkRenderer*>(caller);
    double timeInSeconds = renderer->GetLastRenderTimeInSeconds();
    QString frame_rate = "FPS: " + QString::number(1.0/timeInSeconds,'f',2);

    vtkSmartPointer<vtkActor2DCollection> collection = vtkSmartPointer<vtkActor2DCollection>::New();
    collection = renderer->GetActors2D();

    vtkSmartPointer<vtkTextActor> text_actor = vtkSmartPointer<vtkTextActor>::New();
    text_actor->SetDisplayPosition(10,10);
    text_actor->GetTextProperty()->SetColor(1.0,0.0,0.0);
    text_actor->GetTextProperty()->SetFontSize(12);
    text_actor->SetInput(frame_rate.toUtf8().constData());

    vtkSmartPointer<vtkTextActor> text_actor1 = vtkSmartPointer<vtkTextActor>::New();
    text_actor1->SetDisplayPosition(350,10);
    text_actor1->GetTextProperty()->SetColor(1.0,0.0,0.0);
    text_actor1->GetTextProperty()->SetFontSize(12);
    QString temp = "Temperature: "+QString::number(temp_value,'f',1);
    text_actor1->SetInput(temp.toUtf8().constData());

    //qDebug() << "collection " << collection->GetNumberOfItems();


    while(collection->GetNumberOfItems() > 0)
    {
        renderer->RemoveActor2D(renderer->GetActors2D()->GetLastActor2D());
    }
    renderer->AddActor2D(text_actor);
    renderer->AddActor2D(text_actor1);
}

void MainWindow::CallbackFunction1(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData) )
{
    vtkRenderWindowInteractor* iren = static_cast<vtkRenderWindowInteractor*>(caller);
    double *point = iren->GetPicker()->GetPickPosition();

    vtkCellPicker* picker = static_cast<vtkCellPicker*>(iren->GetPicker());


    double closestPoint[3];
    double closestPointDist2;
    vtkIdType cellId;
    int subId;


    cellLocator->FindClosestPoint(point, closestPoint, cellId, subId, closestPointDist2);

    vtkIdType iD = kdLocator->FindClosestPoint(point);

    vtkSmartPointer<vtkFloatArray> scalars = vtkFloatArray::SafeDownCast(reader->GetOutput()->GetPointData()->GetScalars());

    qDebug() << "Temperature: " << scalars->GetValue(iD);
    temp_value = scalars->GetValue(iD);

}

void MainWindow::GenerateHSVLUT()
{
    //This function generates HSV LUT for the reconstructed mesh
    lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetHueRange( 0.667, 0.0);
    lut->SetSaturationRange(1.0, 1.0);
    lut->SetValueRange(1.0, 1.0);
    lut->SetTableRange(0, 280);
    lut->SetNumberOfColors(256);
    lut->Build();

}

void MainWindow::GenerateGRAYSCALELUT()
{
    //This function generates GRAYSCALE LUT for the reconstructed mesh
    lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetHueRange( 0.667, 0.667);
    lut->SetSaturationRange(0.0, 0.0);
    lut->SetValueRange(0.1, 1.0);
    lut->SetTableRange(0, 280);
    lut->SetNumberOfColors(256);
    lut->Build();

}

void MainWindow::GenerateAUTUMNLUT()
{
    //This function generates GRAYSCALE LUT for the reconstructed mesh
    lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetHueRange( 0.167, 0.0);
    lut->SetSaturationRange(1, 1);
    lut->SetValueRange(1.0, 1.0);
    lut->SetTableRange(0, 280);
    lut->SetNumberOfColors(256);
    lut->Build();

}

void MainWindow::GenerateJETLUT()
{
    //This function generates GRAYSCALE LUT for the reconstructed mesh
    lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetHueRange( 0.667, 0.0);
    lut->SetSaturationRange(0.5, 0.5);
    lut->SetValueRange(1.0, 1.0);
    lut->SetTableRange(0, 280);
    lut->SetNumberOfColors(256);
    lut->Build();

}

void MainWindow::on_btn_RefreshColormap_clicked()
{
    int SelectedColormap = 0;
    // HSV = 1
    // JET = 2
    // GRAYSCALE = 3
    // AUTUMN = 4
    if (ui->radioHSV->isChecked())
        SelectedColormap = 1;
    if (ui->radioJET->isChecked())
        SelectedColormap = 2;
    if (ui->radioGRAYSCALE->isChecked())
        SelectedColormap = 3;
    if (ui->radioAUTUMN->isChecked())
        SelectedColormap = 4;

    switch(SelectedColormap)
    {
    case 1: GenerateHSVLUT();
        mapper->SetLookupTable(lut);
        mapper->Update();
        break;
    case 2: GenerateJETLUT();
        mapper->SetLookupTable(lut);
        mapper->Update();
        break;
    case 3: GenerateGRAYSCALELUT();
        mapper->SetLookupTable(lut);
        mapper->Update();
        break;
    case 4: GenerateAUTUMNLUT();
        mapper->SetLookupTable(lut);
        mapper->Update();
        break;
    }

}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open VTP Model"), "./", tr("VTP File (*.vtp)"));

    reader->SetFileName(fileName.toStdString().c_str());
    reader->Update();

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->ShallowCopy(reader->GetOutput());
    polydata->Update();

    weight = vtkFloatArray::SafeDownCast(polydata->GetPointData()->GetScalars());

    cellLocator->SetDataSet(polydata);
    cellLocator->BuildLocator();

    kdLocator->SetDataSet(reader->GetOutput());
    kdLocator->BuildLocator();

    connect(ui->radioHSV, SIGNAL(clicked()), this,SLOT(GenerateHSVLUT()));
    connect(ui->radioGRAYSCALE, SIGNAL(clicked()), this,SLOT(GenerateGRAYSCALELUT()));

    // HSV colormap lookup table
    lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetHueRange( 0.667, 0.0);
    lut->SetSaturationRange(1.0, 1.0);
    lut->SetValueRange(1.0, 1.0);
    lut->SetTableRange(0, 280);
    lut->SetNumberOfColors(256);
    lut->Build();

    //Implement other colormaps !!!!

    double *range;
    range = polydata->GetPointData()->GetScalars()->GetRange();

    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInput(polydata);
    mapper->SetScalarRange(range[0],range[1]);
    mapper->SetLookupTable(lut);
    mapper->Update();

    vtkSmartPointer<vtkLODActor> actor = vtkSmartPointer<vtkLODActor>::New();
    actor->SetMapper(mapper);
    actor->VisibilityOn();
    actor->SetNumberOfCloudPoints(50000);
    actor->SetPickable(1);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddViewProp(actor);
    renderer->SetBackground(0,0,0);
    //renderer->AddActor2D(text_actor);

    vtkWidget->GetInteractor()->SetDesiredUpdateRate(25);

    vtkSmartPointer<vtkCallbackCommand> callback = vtkSmartPointer<vtkCallbackCommand>::New();
    callback->SetCallback(MainWindow::CallbackFunction);
    renderer->AddObserver(vtkCommand::EndEvent, callback);

    QString temp = "FPS: " + QString::number(0.0,'f',2);
    vtkSmartPointer<vtkTextActor> text_actor = vtkSmartPointer<vtkTextActor>::New();

    text_actor->SetDisplayPosition(10,10);
    text_actor->GetTextProperty()->SetColor(1.0,0.0,0.0);
    text_actor->GetTextProperty()->SetFontSize(12);
    text_actor->SetInput(temp.toUtf8().constData());
    renderer->AddActor2D(text_actor);

    vtkWidget->GetRenderWindow()->AddRenderer(renderer);

    vtkSmartPointer<vtkCallbackCommand> callback1 = vtkSmartPointer<vtkCallbackCommand>::New();
    callback1->SetCallback(MainWindow::CallbackFunction1);
    vtkWidget->GetInteractor()->AddObserver(vtkCommand::EndPickEvent, callback1);

    vtkSmartPointer<vtkInteractorStyleSwitch> flight_style = vtkSmartPointer<vtkInteractorStyleSwitch>::New();
    vtkWidget->GetInteractor()->SetInteractorStyle(flight_style);

}
