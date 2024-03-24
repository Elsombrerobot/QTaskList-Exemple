#ifndef QT_UTILS_H
#define QT_UTILS_H

#include <QMap>
#include <QApplication>
#include <QString>

namespace QtUtils
{
	// Shorthand type type for headers and url data
	typedef QMap<QString, QString> QStrMap;

	// Setup the application object.
	QApplication* CreateQApp(int& argc, char** argv)
	{
		QApplication* app = new QApplication(argc, argv);
		app->setStyle("fusion");
		app->setApplicationName("QTaskListExemple");
		return app;
	}
}

#endif // QT_UTILS_H