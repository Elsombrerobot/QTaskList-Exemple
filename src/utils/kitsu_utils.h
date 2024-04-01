#ifndef KITSU_UTILS_H
#define KITSU_UTILS_H

#include <QString>
#include <QObject>
#include <QMap>
#include <QNetworkReply>

#include "network_utils.h"
#include "qt_utils.h"

namespace KitsuUtils
{
    namespace Routes {
        // Different routes of the api for GetRoute method are defined here.
        inline QString Auth = "/api/auth/login";
        inline QString Api = "/api";
        inline QString UserTasks = "/api/data/persons/{person_id}/tasks";
        inline QString UserDoneTasks = "/api/data/persons/{person_id}/done-tasks";
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