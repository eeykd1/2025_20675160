#include "mainwindow.h"
#include "ui_mainwindow.h"

// VTK Includes
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 1. Link the VTK render window with the Qt widget [cite: 272, 273] */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    // Important: Ensure your widget in the UI is named 'vtkWidget' [cite: 266, 267]
    ui->vtkWidget->setRenderWindow(renderWindow);

    /* 2. Setup the renderer [cite: 273, 274] */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* 3. Create the Cylinder object [cite: 275-278] */
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8); // Makes it an octagon shape [cite: 278]

    /* 4. Setup Mapper and Actor [cite: 279-286] */
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.35); // Set color to red/pink [cite: 286]

    // Rotate the cylinder for a better view [cite: 287, 288]
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    /* 5. Add to the scene and setup the camera [cite: 289-293] */
    renderer->AddActor(cylinderActor);
    renderer->ResetCamera(); // Centers the object [cite: 290]
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

MainWindow::~MainWindow()
{
    delete ui;
}
