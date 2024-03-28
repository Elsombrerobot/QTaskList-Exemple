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
        // Makes it defined in header, since we onlt need it in one file.
        inline QString Auth = "/api/auth/login";
        inline QString Api = "/api";
    }

    class Api : public QObject
    {
        Q_OBJECT

    public:

        // Static function to access the singleton instance
        static Api& Get();

        // Static function to access the singleton instance
        static QString BaseUrl();

        // Static function to set the URL
        static void SetUrl(QString url);

        // Static function for validation
        static void Validate();

        // Static function for authentication
        static void Auth(QString email, QString password);

        // Construct any route for the api
        static QString GetRoute(QString route);

    private:
        // Private constructor to prevent external instantiation
        explicit Api(QObject* parent = nullptr);

        // Private member for storing the URL
        QString m_BaseUrl;

    signals:
        void ValidateApiError(QString message);
        void ValidateApiSuccess();
        void AuthError(QString message);
        void AuthSuccess(QJsonObject userData);


    private slots:
        void m_HandleValidateResponse(); 
        void m_HandleAuthResponse();
    };
}

#endif // KITSU_UTILS_H