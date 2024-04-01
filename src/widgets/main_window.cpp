#include <QMainWindow>
#include <QStatusBar>
#include <QJsonDocument>
#include <QString>
#include <QLabel>
#include <QJsonArray>
#include <QSplitter>


#include "main_window.h"
#include "task_browser.h"
#include "../utils/qt_utils.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	// Create TaskBrowser instance with the taskStack argument
	m_TaskStack = QJsonArray();
	m_TaskBrowser = new TaskBrowser(m_TaskStack, this);

	// Widget setup
	m_StatusBar = new QStatusBar(this);
	setStatusBar(m_StatusBar);

	setCentralWidget(m_TaskBrowser);

	// Show message
	m_StatusBar->showMessage("Connection as " + QtUtils::CurrentUser::FullName() + " successful !", 4000);

	// Resize window
	resize(1200, 700);
}