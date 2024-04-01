#include <QLabel>
#include <QWidget>
#include <QString>
#include <QSplitter>
#include <QList>
#include <Qt>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QJsonDocument>
#include <QJsonArray>

#include "task_browser.h"
#include "task_loader.h"


TaskBrowser::TaskBrowser(QJsonArray& taskStack, QWidget* parent)
	: QWidget(parent), m_TaskStack(taskStack)
{
    // Create a splitter for the left part
    m_leftSplitter = new QSplitter(Qt::Vertical, this);
    m_leftSplitter->setHandleWidth(8);
    //m_leftSplitter->setSizes(QList<int>(100, 1));

    // Create a splitter for the main layout
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->setHandleWidth(8);
    m_mainSplitter->addWidget(m_leftSplitter);

    // Set the main layout for the widget
    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_mainSplitter);
    setLayout(m_layout);

    m_TaskLoader = new TaskLoader(m_leftSplitter);

    // TODO connect tasks loaded to task filter, and task list.
    /*connect(
        m_TaskLoader,
        SIGNAL(TasksAvailable(QJsonArray)),
        this,
        SLOT(PrintJson(QJsonArray)));*/

}