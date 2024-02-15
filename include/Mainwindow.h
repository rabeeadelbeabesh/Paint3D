#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkNamedColors.h>
#include <vtkUnstructuredGrid.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
   /**
	 * @brief Constructor for the MainWindow class.
	 *
	 * Initializes the main window of the application.
	 *
	 * @param parent Pointer to the parent widget.
	 */
	explicit MainWindow(QWidget* parent = nullptr);
   /**
	 * @brief Destructor for the MainWindow class.
	 *
	 * Cleans up resources and deallocates memory used by the MainWindow object.
	 */
	~MainWindow();
   /**
	 * @brief Generates a VTK actor representing a shape based on user-selected parameters.
	 *
	 * This function creates a VTK actor representing a shape based on the type selected by the user
	 * and additional parameters such as color and opacity.
	 *
	 * @return A vtkSmartPointer to a vtkActor representing the generated shape.
	 */
	vtkSmartPointer<vtkActor> shapeController();
public slots:
   /**
	 * @brief Slot function called when the draw button is clicked.
	 *
	 * This function validates the selected shape type, color, and opacity. If any of these are not specified,
	 * it displays a warning message. Otherwise, it adds the shape actor to the renderer, resets the camera,
	 * and renders the scene.
	 */
	void onDrawButtonClick();
   /**
	 * @brief Slot function called when the clear button is clicked.
	 *
	 * This function removes all actors from the renderer and renders the scene.
	 */
	void onClearButtonClick();

private:


	const int BESTRESOLUTION = 100;
	vtkNew<vtkNamedColors> namedColors;
	Ui::MainWindow* ui;
	vtkNew<vtkGenericOpenGLRenderWindow> mRenderWindow;
	vtkNew<vtkRenderer> mRenderer;
	vtkNew<QVTKInteractor> mInteractor;
	vtkNew<vtkInteractorStyleTrackballActor> mInteractorStyle;
	
};

#endif // MAINWINDOW_H
