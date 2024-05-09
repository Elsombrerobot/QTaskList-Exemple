#ifndef QT_UTILS_H
#define QT_UTILS_H

#include <QMap>
#include <QApplication>
#include <QString>
#include <QJsonObject>
#include <QVariant>

namespace QtUtils
{
    // Shorthand type for a lot of uses cases, including url data, header...
    typedef QMap<QString, QString> QStrMap;

    QApplication* CreateQApp(int& argc, char** argv);
    QString GetWindowName();
    void SetLanguage(const QString& langId);

    namespace Settings
    {
        // Standard app settings, to use with write/read settings.
        namespace Keys 
        {
            inline QString LastKitsuUrlUsed = "Connection/LastKitsuUrlUsed";
            inline QString LastMailUsed = "Connection/LastMailUsed";
            inline QString AppLangage = "App/Langage";
        };

        void Write(const QString& group, QVariant value);
        QVariant Read(const QString& group, QVariant defaultValue = QVariant());
    };

    class CurrentUser
    {
    public:
        static CurrentUser& Get();
        static QJsonObject& Data();
        static void Set(QJsonObject& data);
        static QString FullName();
        static QString Id();
        static QString Mail();
        static QString AccessToken();
        bool connected;

    private:
        QJsonObject m_Data;
        CurrentUser();
        QJsonObject& IData();
    };
}

#endif // QT_UTILS_H
