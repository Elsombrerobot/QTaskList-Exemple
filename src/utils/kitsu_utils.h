#ifndef KITSU_UTILS_H
#define KITSU_UTILS_H

#include <QString>
#include <QObject>
#include <QMap>
#include <QNetworkReply>

#include "network_utils.h"
#include "qt_utils.h"
#include "task_utils.h"

namespace KitsuUtils
{
    // Different routes of the api for GetRoute method are defined here.
    namespace Routes
    {
        inline QString Auth = "/api/auth/login";
        inline QString Api = "/api";
        inline QString UserTasks = "/api/data/persons/{person_id}/tasks";
        inline QString UserDoneTasks = "/api/data/persons/{person_id}/done-tasks";
        inline QString TaskUrl = "/productions/{project_id}/shots/tasks/{task_id}";
        inline QString ShotUrl = "/productions/{project_id}/shots/{shot_id}";
        inline QString AssetUrl = "/productions/{project_id}/assets/{asset_id}";
        inline QString EpisodeUrl = "/productions/{project_id}/episodes/{episode_id}/shots";
    }

    class Api : public QObject
    {
        Q_OBJECT

    public:
        static Api& Get();
        static QString BaseUrl();
        static void SetUrl(QString url);
        static void Validate();
        static void Auth(QString email, QString password);
        static void GetTasks(bool done);
        static QString GetRoute(QString route, const QtUtils::QStrMap& formatData = {});
        static void OpenTaskInBrowser(const TaskUtils::Task& task);
        static void OpenShotInBrowser(const TaskUtils::Task& task);
        static void OpenAssetInBrowser(const TaskUtils::Task& task);
        static void OpenEpisodeInBrowser(const TaskUtils::Task& task);

    private:
        explicit Api(QObject* parent = nullptr);
        QString m_BaseUrl;

    signals:
        void ValidateApiError(QString message);
        void ValidateApiSuccess();

        void AuthError(QString message);
        void AuthSuccess(QJsonObject userData);

        void GetTasksError(QString message);
        void GetTasksSuccess(QJsonArray tasksData);

    private slots:
        void m_HandleValidateResponse(); 
        void m_HandleAuthResponse();
        void m_HandleGetTasksResponse();
    };
}

#endif // KITSU_UTILS_H