// Externals
#include <QApplication>

// External submodules
#include "qtasklist.h"

// Internals
#include "utils/network_utils.h"
#include "utils/qt_utils.h"
#include "utils/kitsu_utils.h"
#include "widgets/connection_dialog.h"


int main(int argc, char** argv) {

	QApplication* app = QtUtils::CreateQApp(argc, argv);

	// Create a network access manager
	//QNetworkAccessManager network_manager;

	// Show connection dialog
	ConnectionDialog connection_dialog;
	connection_dialog.show();


	//Exit app
	int retCode = app->exec();
	delete app;
	return retCode;
}