#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <QUrl>
#include <QObject>
#include <QUrlQuery>
#include <QString>
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "qt_utils.h"

class AppNetworkManager
{
public:

	static AppNetworkManager& Get();
	static QNetworkAccessManager& Manager();

private:

	AppNetworkManager();
	AppNetworkManager(const AppNetworkManager&) = delete;

	QNetworkAccessManager m_Manager;
	QNetworkAccessManager& IManager();
};


namespace NetworkUtils
{
	enum Timeout : quint16
	{
		MsNormal = 5000,
		MsLong = 30000
	};

	QNetworkRequest CreateRequest(
		const QString& url,
		const QtUtils::QStrMap& headers,
		const QtUtils::QStrMap& urldata,
		quint16 msTimeout);

	QNetworkReply* Get(
		const QString& url,
		const QtUtils::QStrMap& headers = {},
		const QtUtils::QStrMap& urldata = {},
		quint16 msTimeout = Timeout::MsNormal);

	QNetworkReply* Post(
		const QString& url,
		const QtUtils::QStrMap& headers = {},
		const QtUtils::QStrMap& urldata = {},
		quint16 msTimeout = Timeout::MsNormal);

	QString EnsureTrailingSlash(QString url);
	QJsonObject ReadReplyData(QNetworkReply* reply);
}

#endif // NETWORK_UTILS_H