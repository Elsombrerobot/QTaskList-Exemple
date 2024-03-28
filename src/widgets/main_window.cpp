#include <QMainWindow>
#include <QStatusBar>
#include <QJsonDocument>
#include <QString>

#include "main_window.h"
#include "../utils/qt_utils.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	// Widget setup
	QStatusBar* m_StatusBar = new QStatusBar(this);
	setStatusBar(m_StatusBar);

	// Show message
	m_StatusBar->showMessage("Connection as " + QtUtils::CurrentUser::FullName() + " successful !", 4000);

}