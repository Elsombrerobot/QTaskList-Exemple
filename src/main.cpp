// Externals
#include <QApplication>
#include <QObject>
#include <QDialog>

// External submodules
#include "qtasklist.h"

// Internals
#include "utils/qt_utils.h"
#include "widgets/connection_dialog.h"
#include "widgets/main_window.h"

int main(int argc, char** argv) {

	// Create app
	QApplication* app = QtUtils::CreateQApp(argc, argv);

	// Show connection dialog
	ConnectionDialog connection_dialog;

	if (connection_dialog.exec() != QDialog::Accepted)
	{
		// Connection dialog closed, exit application.
		return 0;
	}

	// Main window.
	MainWindow main_window;
	main_window.show();

	//Exit app
	int retCode = app->exec();
	delete app;
	return retCode;
}