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
    QMenu* openInBrowserMenu = this->addMenu(tr("Open in browser..."));
    openInBrowserMenu->setIcon(QIcon(":/kitsu"));

    // Open actions
    QAction* openEpisodeInKitsuAction = openInBrowserMenu->addAction(tr("Episode"));

    // Hide shot or asset action based on entity type.
    QAction* openShotInKitsuAction = openInBrowserMenu->addAction(tr("Shot"));
    openShotInKitsuAction->setVisible(onTask.Field(TaskUtils::Fields::Entity) == "Shot");
    QAction* openAssetInKitsuAction = openInBrowserMenu->addAction("Asset");
    openAssetInKitsuAction->setVisible(onTask.Field(TaskUtils::Fields::Entity) == "Asset");

    QAction* openTaskInKitsuAction = openInBrowserMenu->addAction(tr("Task"));

    // Open task action.
    connect(openTaskInKitsuAction,
            &QAction::triggered,
            this,
            [this, onTask]() {KitsuUtils::Api::OpenTaskInBrowser(onTask); }
    );

    // Open episode action.
    connect(openEpisodeInKitsuAction,
        &QAction::triggered,
        this,
        [this, onTask]() {KitsuUtils::Api::OpenEpisodeInBrowser(onTask); }
    );

    // Open shot action.
    connect(openShotInKitsuAction,
        &QAction::triggered,
        this,
        [this, onTask]() {KitsuUtils::Api::OpenShotInBrowser(onTask); }
    );

    // Open asset action.
    connect(openAssetInKitsuAction,
        &QAction::triggered,
        this,
        [this, onTask]() {KitsuUtils::Api::OpenAssetInBrowser(onTask); }
    );
};
