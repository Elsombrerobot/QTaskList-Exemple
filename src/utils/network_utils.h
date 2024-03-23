#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <QUrl>
#include <QObject>
#include <QUrlQuery>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "qt_utils.h"

//Singleton for qapp network access manager.
class AppNetworkManager
{
public:

	static AppNetworkManager& Get();
	static QNetworkAccessManager& Manager();

private:

	QNetworkAccessManager m_Manager;
	QNetworkAccessManager& IManager(); // Internal manager access.
};

namespace NetworkUtils
{

	QNetworkRequest CreateRequest(
		const QString& url,
		const QtUtils::QStrMap& headers = {},
		const QtUtils::QStrMap& urldata = {});

	QNetworkReply* Get(
		const QString& url,
		const QtUtils::QStrMap& headers = {},
		const QtUtils::QStrMap& urldata = {});

	QNetworkReply* Post(
		const QString& url,
		const QtUtils::QStrMap& headers = {},
		const QtUtils::QStrMap& urldata = {});

	QUrl EnsureTrailingSlash(QUrl url);


}

#endif // NETWORK_UTILS_H