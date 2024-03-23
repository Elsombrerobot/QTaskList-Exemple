#ifndef QT_UTILS_H
#define QT_UTILS_H

#include <QMap>
#include <QApplication>
#include <QString>

namespace QtUtils
{
	typedef QMap<QString, QString> QStrMap;

	// Setup an application object.
	QApplication* CreateQApp(int& argc, char** argv);
}

#endif // QT_UTILS_H