#include <QMap>
#include <QApplication>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include "../utils/qt_utils.h"

namespace QtUtils
{
    QApplication* CreateQApp(int& argc, char** argv)
    {
        QApplication* app = new QApplication(argc, argv);

        app->setStyle("fusion");
        app->setApplicationName("QTaskListExemple");
        return app;
    }

    CurrentUser::CurrentUser() {}

    CurrentUser& CurrentUser::Get() {
        static CurrentUser s_Instance;
        return s_Instance;
    }

    void CurrentUser::Set(QJsonObject& data) {
        Get().IData() = data;
    }

    QJsonObject& CurrentUser::Data() {
        return Get().IData();
    }

    QString CurrentUser::FullName()
    {
        return Data()["user"].toObject()["full_name"].toString();
    }

    QString CurrentUser::Id()
    { 
        return Data()["user"].toObject()["id"].toString();
    }

    QString CurrentUser::AccessToken()
    {
        return Data()["access_token"].toString();
    }

    QJsonObject& CurrentUser::IData() {
        return m_Data;
    }

}