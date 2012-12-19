#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include "vtkTextActor.h"
#include "vtkInteractorStyleSwitch.h"
#include "vtkCellLocator.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static void CallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData) );
    static void CallbackFunction1(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData) );
    QString GetFrameRateText() const
    {
        return frame_rate_text;
    }
    void SetFPS(QString text)
    {
        if (text != frame_rate_text)
        {
            frame_rate_text = text;
            emit FPSChanged(text);
        }

    }
    QString frame_rate_text;

private:
    Ui::MainWindow *ui;
    QVTKWidget* vtkWidget;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkInteractorStyleSwitch> flight_style;

signals:
    void FPSChanged(QString newtext);


private slots:
    void on_btnDOWN_clicked();
    void on_btnUP_clicked();
    void GenerateHSVLUT();
    void GenerateGRAYSCALELUT();
    void GenerateAUTUMNLUT();
    void GenerateJETLUT();
    void on_btn_RefreshColormap_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
