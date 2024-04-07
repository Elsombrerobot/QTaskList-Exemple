#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <QUrl>
#include <QObject>
#include <QUrlQuery>
#include <QString>
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QList>
#include <QNetworkReply>
#include <QJsonArray>

#include "qt_utils.h"

namespace NetworkUtils
{
	class AppNetworkManager : public QObject
	{
		Q_OBJECT

	public:
		static AppNetworkManager* Get();
		static QNetworkAccessManager* Manager();

	private:
		AppNetworkManager();
		AppNetworkManager(const AppNetworkManager&) = delete;

		QNetworkAccessManager* m_Manager;
	};

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
	QJsonObject ReadJsonReply(QNetworkReply* reply);
	QJsonArray ReadArrayReply(QNetworkReply* reply);
}

#endif // NETWORK_UTILS_H