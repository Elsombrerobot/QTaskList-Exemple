#ifndef QT_UTILS_H
#define QT_UTILS_H

#include <QMap>
#include <QApplication>
#include <QString>
#include <QJsonObject>
#include <QVariant>

namespace QtUtils
{
    // Shorthand type for headers and url data
    typedef QMap<QString, QString> QStrMap;

    // Setup an application object.
    QApplication* CreateQApp(int& argc, char** argv);

    class CurrentUser
    {
    public:

        static CurrentUser& Get();
        static QJsonObject& Data();
        static void Set(QJsonObject& data);
        static QString FullName();
        static QString Id();
        static QString AccessToken();

    private:

        QJsonObject m_Data;
        CurrentUser();
        QJsonObject& IData();
    };
}

#endif // QT_UTILS_H
