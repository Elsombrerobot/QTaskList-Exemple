#include <QLabel>
#include <QWidget>
#include <QString>
#include <QSplitter>
#include <QList>
#include <Qt>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QContextMenuEvent>
#include <QItemSelection>

#include "../utils/kitsu_utils.h"
#include "task_browser.h"
#include "task_loader.h"
#include "task_utils.h"
#include "task_filter.h"
#include "task_list.h"
#include "task_menu.h"

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

    // Show infos about currents task list.
    m_taskResumeLabel = new QLabel(this);
    m_taskResumeLabel->setDisabled(true); // Just for dimmer color.

    // TaskList, display the tasks in a form of a list.
    m_taskTable = new TaskTable(&m_taskList, &m_taskFilter->currentFilter, this);
    m_taskTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Layouts
    m_tableWidget = new QWidget(this);
    m_tableLayout = new QVBoxLayout(m_tableWidget);
    m_tableLayout->setContentsMargins(0,0,0,0);
    m_tableLayout->addWidget(m_taskResumeLabel);
    m_tableLayout->addWidget(m_taskTable);
    m_mainSplitter->addWidget(m_tableWidget);

    // Set the stretch factors for the splitters
    m_mainSplitter->setStretchFactor(1, 1);

    // Set disabled on tasks loading started.
    connect(m_TaskLoader,
        &TaskLoader::ReloadStarted,
        this,
        [this]() {setDisabled(true);});

    // Set enabled on tasks loading finished.
    connect(m_TaskLoader,
        &TaskLoader::ReloadFinished,
        this,
        [this]() {setEnabled(true);});

    // On tasks available create the task list to pass to the filter and the view.
    connect(m_TaskLoader, 
        SIGNAL(TasksAvailable(QJsonArray)),
        this,
        SLOT(m_HandleAvailableTasks(QJsonArray)));

    // On task list ready, send pointer to filter.
    connect(this,
        SIGNAL(TaskListReady(TaskUtils::TaskList*)),
        m_taskFilter,
        SLOT(m_HandleTaskListReady(TaskUtils::TaskList*)));

    // On task list ready, refresh model view.
    connect(this,
        SIGNAL(TaskListReady(TaskUtils::TaskList*)),
        m_taskTable->taskModel,
        SLOT(RefreshModel()));

    // On filter updated by user, refresh proxy filter.
    connect(m_taskFilter,
        SIGNAL(SelectedFiltersChanged(TaskUtils::FilterMap*)),
        m_taskTable->filterProxy,
        SLOT(RefreshFilter()));

    // On filter updated by user, refresh task resume label, aswell as on model reset and selection changed.
    connect(m_taskTable->taskModel,
        SIGNAL(modelReset()),
        this,
        SLOT(m_UpdateResumeLabel()));

    connect(m_taskFilter,
        SIGNAL(SelectedFiltersChanged(TaskUtils::FilterMap*)),
        this,
        SLOT(m_UpdateResumeLabel()));

    connect(m_taskTable->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection& , const QItemSelection&)),
        this,
        SLOT(m_UpdateResumeLabel()));

    // Connect task menu request to TaskMenu implementation.
    connect(m_taskTable,
        SIGNAL(TaskContextMenuRequested(const TaskUtils::Task&, TaskUtils::TaskConstRefList, QContextMenuEvent*)),
        this,
        SLOT(m_HandlTaskMenuRequest(const TaskUtils::Task&, TaskUtils::TaskConstRefList, QContextMenuEvent*)));

    m_UpdateResumeLabel();
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

// Update resume label with current info.
void TaskBrowser::m_UpdateResumeLabel()
{
    QString resume = QString(tr("Total : %1 | Shown : %2 | Selected : %3"))
        .arg(QString::number(m_taskList.size()))
        .arg(QString::number(m_taskTable->filterProxy->rowCount()))
        .arg(QString::number(m_taskTable->selectionModel()->selectedRows().size()));
    m_taskResumeLabel->setText(resume);
};

// Update resume label with current info.
void TaskBrowser::m_HandlTaskMenuRequest(const TaskUtils::Task& onTask, TaskUtils::TaskConstRefList selectedTasks, QContextMenuEvent* event)
{
    // Create task menu.
    TaskContextMenu* menu = new TaskContextMenu(onTask, selectedTasks, this);
    menu->exec(event->globalPos());
    delete menu;
};
