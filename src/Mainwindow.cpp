#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
#include <Mainwindow.h>
#include "ui_MainWindow.h"
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkQtTableView.h>
#include <vtkVersion.h>
#include <vtkTransform.h>
#include <vtkRendererCollection.h>
#include <vtkActorCollection.h>
#include <QMessageBox>
#include <sstream>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkPyramid.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>
#include <QColor>
#include <QListWidget>
#include <QStyledItemDelegate>
#include <sstream>
#include <vtkNamedColors.h>
#include <QPainter>
#include <vtkTransformPolyDataFilter.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricTorus.h>
#include <algorithm>
MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// Set up the rendering
	mRenderWindow->AddRenderer(mRenderer);
	mRenderWindow->SetInteractor(mInteractor);
	ui->openGLWidget->setRenderWindow(mRenderWindow);
	mInteractor->SetInteractorStyle(mInteractorStyle);
	mInteractor->Initialize();

	// Set the UI connections
	QObject::connect(ui->drawButton, &QPushButton::clicked, this, &MainWindow::onDrawButtonClick);
	QObject::connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClick);

	//Fill the color list
	std::stringstream ss(namedColors->GetColorNames());
	std::string colorName;
	while (std::getline(ss, colorName, '\n'))
	{
		// Get the corresponding QColor object for the color name
		double rgb[3]; // Array to store RGB values
		namedColors->GetColor(colorName, rgb); // Obtain RGB values from vtkNamedColors

		// Convert RGB values to QColor
		QColor color = QColor::fromRgbF(rgb[0], rgb[1], rgb[2]);

		// Create a new item with the color name
		QListWidgetItem* item = new QListWidgetItem(colorName.c_str());

		// Set the background color of the item
		item->setData(Qt::BackgroundRole, color);


		// Add the item to the list widget
		ui->colorListWidget->addItem(item);
	}


	// Set opacityLineEdit to accept only numbers from 0 to 99
	ui->opacityLineEdit->setValidator(new QIntValidator(0, 100, this));

	
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onClearButtonClick()
{
	while (mRenderer->GetActors()->GetNumberOfItems())
	{
		auto lastActor = mRenderer->GetActors()->GetLastActor();
		mRenderer->RemoveActor(lastActor);
	}
	mRenderWindow->Render();
}

vtkSmartPointer<vtkActor> MainWindow::shapeController()
{
	vtkSmartPointer<vtkActor> shape;
	vtkNew<vtkDataSetMapper> shapeMapper;
	// Create shape mapper
	QString type = ui->typeListWidget->selectedItems().first()->text();
	if (type == "Cube") {
		shapeMapper->SetInputConnection(vtkNew<vtkCubeSource>()->GetOutputPort());
	}
	else if (type == "Sphere") {
		vtkNew<vtkSphereSource>sphere;
		sphere->SetThetaResolution(BESTRESOLUTION);
		sphere->SetPhiResolution(BESTRESOLUTION);
		shapeMapper->SetInputConnection(sphere->GetOutputPort());
	}
	else if (type == "Hemisphere") {
		vtkNew<vtkSphereSource>sphere;
		sphere->SetThetaResolution(BESTRESOLUTION);
		sphere->SetPhiResolution(BESTRESOLUTION);
		sphere->SetEndTheta(180);
		shapeMapper->SetInputConnection(sphere->GetOutputPort());
	}
	else if (type == "Cone") {
		vtkNew<vtkConeSource>cone;
		cone->SetResolution(BESTRESOLUTION);
		shapeMapper->SetInputConnection(cone->GetOutputPort());
	}
	else if (type == "Pyramid") {
		vtkNew<vtkConeSource>cone;
		cone->SetResolution(4);
		cone->SetRadius(0.7);
		shapeMapper->SetInputConnection(cone->GetOutputPort());
	}
	else if (type == "Cylinder") {
		vtkNew<vtkCylinderSource> cylinder;
		cylinder->SetResolution(BESTRESOLUTION);
		shapeMapper->SetInputConnection(cylinder->GetOutputPort());
	}
	else if (type == "Tube") {
		vtkNew<vtkCylinderSource> cylinder;
		cylinder->SetResolution(BESTRESOLUTION);
		cylinder->SetCapping(false);
		shapeMapper->SetInputConnection(cylinder->GetOutputPort());
	}
	else if (type == "CurvedCylinder") {
		vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
		cylinder->SetResolution(BESTRESOLUTION);
		cylinder->SetHeight(4.0); // Height of the cylinder
		cylinder->SetRadius(1.0); // Radius of the cylinder

		shapeMapper->SetInputConnection(cylinder->GetOutputPort());
	}
	else if (type == "Doughnut") {
		// Create a vtkParametricTorus to represent the doughnut shape
		vtkSmartPointer<vtkParametricTorus> doughnutSurface = vtkSmartPointer<vtkParametricTorus>::New();

		// Set the parameters of the torus to define the shape of the doughnut
		double majorRadius = 3.0; // Major radius of the torus
		double minorRadius = 1.0; // Minor radius of the torus
		doughnutSurface->SetRingRadius(majorRadius);
		doughnutSurface->SetCrossSectionRadius(minorRadius);

		// Create a vtkParametricFunctionSource to generate points based on the parametric surface
		vtkSmartPointer<vtkParametricFunctionSource> parametricSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
		parametricSource->SetParametricFunction(doughnutSurface);

		// Set the resolution of the generated points
		int resolutionU = 50; // Resolution in the U direction
		int resolutionV = 50; // Resolution in the V direction
		parametricSource->SetUResolution(resolutionU);
		parametricSource->SetVResolution(resolutionV);

		// Connect the output of the parametric source to the input of the shape mapper
		shapeMapper->SetInputConnection(parametricSource->GetOutputPort());
	}
	else {
		QMessageBox::warning(this, "Warning", "you should select type of shape");
		return vtkNew<vtkActor>();
	}

	//shapeMapper->SetInputData(shapeSource->GetOutput());
	shape = vtkNew<vtkActor>();
	shape->SetMapper(shapeMapper);

	// set shape color
	std::string selectedColor = ui->colorListWidget->selectedItems().first()->text().toStdString();
	shape->GetProperty()->SetColor(namedColors->GetColor4d(selectedColor.c_str()).GetData());

	// set shape opacity
	int opacity = std::min(ui->opacityLineEdit->text().toInt(),100);
	ui->opacityLineEdit->setText(std::to_string(opacity).c_str());

	shape->GetProperty()->SetOpacity(opacity / 100.0);


	return shape;
}

void MainWindow::onDrawButtonClick()
{
	//validate slected color, type, and opacity
	if (ui->typeListWidget->selectedItems().isEmpty() ||
		ui->colorListWidget->selectedItems().isEmpty() ||
		ui->opacityLineEdit->text().isEmpty()
		)
	{
		QMessageBox::warning(this, "Warning", "You should select the shape type, color, and opacity.");
		return;
	}

	// Add the shape actor to the OpenGL
	mRenderer->AddActor(shapeController());
	mRenderer->ResetCamera();
	mRenderWindow->Render();
}