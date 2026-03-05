#include <vtkActor.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLight.h>
#include <vtkNew.h>

// --- MANDATORY DRIVERS ---
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

int main(int, char*[])
{
  // 1. Create the Shape
  vtkNew<vtkCylinderSource> cylinder;
  cylinder->SetResolution(12); 

  // 2. Create the Mapper
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(cylinder->GetOutputPort());

  // 3. Create the Actor
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  
  // Set color directly to Tomato Red (Red=1.0, Green=0.3, Blue=0.3)
  actor->GetProperty()->SetColor(1.0, 0.3, 0.3); 
  actor->GetProperty()->SetAmbient(0.3); 
  actor->GetProperty()->SetDiffuse(0.7); 

  // 4. Create the Renderer
  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(actor);
  renderer->SetBackground(0.1, 0.2, 0.4); // Dark blue background

  // 5. Create the Window
  vtkNew<vtkRenderWindow> renderWindow; // Fixed the "double vtkNew" error here
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(600, 600);
  renderWindow->SetWindowName("Exercise 1 - VTK Cylinder");

  // 6. Create the Interactor (Mouse control)
  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(renderWindow);
  
  vtkNew<vtkInteractorStyleTrackballCamera> style;
  interactor->SetInteractorStyle(style);

  // 7. Manual Lighting Fix
  vtkNew<vtkLight> light;
  light->SetLightTypeToHeadlight();
  renderer->AddLight(light);

  // 8. Launch
  renderer->ResetCamera();
  renderWindow->Render();
  interactor->Start();

  return 0;
}