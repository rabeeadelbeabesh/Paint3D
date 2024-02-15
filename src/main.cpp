#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
#include <QtWidgets/qapplication.h>
#include <Mainwindow.h>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	MainWindow MainWindow;
	MainWindow.show();

	return app.exec();
}