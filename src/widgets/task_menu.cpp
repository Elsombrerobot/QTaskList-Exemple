#include <QMenu>
#include <QAction>
#include <QWidget>

#include "task_utils.h"
#include "task_menu.h"
#include "../utils/kitsu_utils.h"

// --- TaskContextMenu ---
// Context menu to request an opening of the task on the production trackers, function to open the task is to implement in application using this widget.
TaskContextMenu::TaskContextMenu(const TaskUtils::Task& onTask,
                                 TaskUtils::TaskConstRefList selectedTasks,
                                 QWidget* parent) : QMenu(parent)
{
    // Empty action with the name of the task.
    QAction* labelAction = addAction(onTask.descriptiveName);
    labelAction->setEnabled(false);

    // Sub menu open in browser
    QMenu* openInBrowserMenu = this->addMenu("Open in browser...");
    openInBrowserMenu->setIcon(QIcon(":/kitsu"));

    // Open action
    QAction* openInKitsuAction = openInBrowserMenu->addAction("Task");

    // Emit the signal informing that open task has been requested.
    connect(openInKitsuAction,
            &QAction::triggered,
            this,
            [this, onTask]() {KitsuUtils::Api::OpenTaskInBrowser(onTask); }
    );
};
