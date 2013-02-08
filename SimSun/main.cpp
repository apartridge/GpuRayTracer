#include <QApplication>
#include "window.h"
#include "Quadrangle.h"
#include "global.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	//Loading Window Object which contains all the components
	//initGlobalColormap();
	Window window;
	window.show();

	return app.exec();
}
