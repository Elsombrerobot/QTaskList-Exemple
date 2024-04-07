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
#include <QJsonValueRef>

#include "task_browser.h"
#include "task_loader.h"
#include "task_utils.h"
#include "task_filter.h"
#include "task_list.h"

// The TaskBrowser contain 3 main Widget, TaskLoader, TaskFilter, and TaskList, it is the main widget of the application
TaskBrowser::TaskBrowser(QWidget* parent)
	: QWidget(parent)
{
    // Create a splitter for the left part
    m_leftSplitter = new QSplitter(Qt::Vertical, this);
    m_leftSplitter->setHandleWidth(8);

    // Create a splitter for the main layout
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->setHandleWidth(8);
    m_mainSplitter->addWidget(m_leftSplitter);


    // Set the main layout for the widget
    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_mainSplitter);
    setLayout(m_layout);

    // TaskLoader, load task from the api.
    m_TaskLoader = new TaskLoader(m_leftSplitter);
    m_TaskLoader->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // TaskFilter, use graphical filters to filter the tasks display.
    m_taskFilter = new TaskFilter(m_leftSplitter);
    m_taskFilter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    // TaskList, display the tasks in a form of a list.
    m_taskModel = new TaskTableModel(&m_taskList, this);
    m_taskTable = new TaskTable(m_mainSplitter);
    m_filterProxy = new TaskTableFilterProxy(&m_taskFilter->currentFilter, this);
    m_taskTable->setModel(m_taskModel);
    m_taskTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_filterProxy->setSourceModel(m_taskModel);
    m_taskTable->setModel(m_filterProxy);

    // Set the stretch factors for the splitters
    m_mainSplitter->setStretchFactor(1, 1);

    // Set disabled on tasks loading started.
    connect(m_TaskLoader, &TaskLoader::ReloadStarted, this, [this]() {setDisabled(true);});

    // Set enabled on tasks loading finished.
    connect(m_TaskLoader, &TaskLoader::ReloadFinished, this, [this]() {setEnabled(true);});

    // On tasks available create the task list to pass to the filter and the view.
    connect(m_TaskLoader, SIGNAL(TasksAvailable(QJsonArray)), this, SLOT(m_HandleAvailableTasks(QJsonArray)));

    // On task list ready, send pointer to filter.
    connect(this,
        SIGNAL(TaskListReady(TaskUtils::TaskList*)),
        m_taskFilter,
        SLOT(m_HandleTaskListReady(TaskUtils::TaskList*)));

    // On task list ready, refresh model view.
    connect(this,
        SIGNAL(TaskListReady(TaskUtils::TaskList*)),
        m_taskModel,
        SLOT(RefreshModel()));

    // On filter updated by user, refresh proxy filter.
    connect(m_taskFilter,
        SIGNAL(SelectedFiltersChanged(TaskUtils::FilterMap*)),
        m_filterProxy,
        SLOT(RefreshFilter()));
}

// Transform data into object, and emit pointer to task list when done for filter and the view to use the objects.
void TaskBrowser::m_HandleAvailableTasks(QJsonArray tasksData)
{
    m_taskList.clear();
    for (const QJsonValueRef& item : tasksData)
    {
        m_taskList.emplaceBack(item.toObject());
    }
    emit TaskListReady(&m_taskList);
};
