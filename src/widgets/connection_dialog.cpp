
#include <QVBoxLayout>
#include <QMessageBox>
#include <QObject>
#include <QApplication>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "connection_dialog.h"
#include "../utils/qt_utils.h"
#include "../utils/network_utils.h"

ConnectionDialog::ConnectionDialog(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Connect to kitsu instance.");

	QVBoxLayout* layout = new QVBoxLayout(this);

	kitsuApiUrlLineEdit = new QLineEdit(this);
	kitsuApiUrlLineEdit->setPlaceholderText("https://kitsu.my_studio/api/");
	layout->addWidget(kitsuApiUrlLineEdit);

	mailLineEdit = new QLineEdit(this);
	mailLineEdit->setPlaceholderText("name@company.com");
	layout->addWidget(mailLineEdit);

	passwordLineEdit = new QLineEdit(this);
	passwordLineEdit->setPlaceholderText("Password");
	passwordLineEdit->setEchoMode(QLineEdit::Password);
	layout->addWidget(passwordLineEdit);

	connectButton = new QPushButton("Connect", this);
	layout->addWidget(connectButton);

	connect(connectButton, &QPushButton::clicked, this, &ConnectionDialog::AttemptApiConnection);
	connect(this, &ConnectionDialog::accepted, qApp, &QApplication::quit); // Close application on dialog accept

	setMinimumWidth(400);
}

void ConnectionDialog::AttemptAuth()
{
	SetInputsDisabled(true);
	QUrl baseUrl = NetworkUtils::EnsureTrailingSlash(QUrl(kitsuApiUrlLineEdit->text()));
	QUrl authRoute = QUrl(baseUrl).resolved(QUrl("auth/login"));
	QtUtils::QStrMap urlData;
	urlData["email"] = mailLineEdit->text();
	urlData["password"] = passwordLineEdit->text();
	QNetworkReply* reply = NetworkUtils::Post(authRoute.toString(), {}, urlData);
	connect(reply, SIGNAL(finished()), this, SLOT(HandleAuthResponse()));
}

void ConnectionDialog::HandleAuthResponse()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	QUrl baseUrl = reply->url();
	baseUrl.setQuery("");


	// Make sure the reply isn't in error state
	if (reply->error() == QNetworkReply::NoError)
	{
		// Read the response data
		QByteArray responseData = reply->readAll();
		QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

		// Try to read the reply data as a json
		if (!jsonResponse.isNull() && jsonResponse.isObject())
		{
			// Success if {"api" : "zou"} exists in reponse
			QString keyToCheck = "login";
			QJsonObject jsonObject = jsonResponse.object();
			QJsonValue apiValue = jsonObject.value(keyToCheck);

			if (apiValue.isBool() && apiValue.toBool() == true)
			{
				QMessageBox::information(this, "Success", "Connection successful.");
				accept();
			}
			else
			{
				QMessageBox::warning(this,
					QString("Error reading server response."),
					QString("Error reading response from ") + reply->url().toString());
			}
		}
		else
		{
			QMessageBox::warning(this,
				QString("Error reading server response."),
				QString("Error reading response from ") + baseUrl.toString());
		}
	}
	else
	{
		QMessageBox::warning(this, "Error " + baseUrl.toString(), "Connection failed, verify login and password.");
	}
	mailLineEdit->setText("");
	passwordLineEdit->setText("");
	SetInputsDisabled(false);
}

void ConnectionDialog::AttemptApiConnection()
{
	SetInputsDisabled(true);
	QString kitsuApiUrl = kitsuApiUrlLineEdit->text();
	QNetworkReply* reply = NetworkUtils::Get(kitsuApiUrl);
	connect(reply, SIGNAL(finished()), this, SLOT(HandleApiReponse()));
}

void ConnectionDialog::HandleApiReponse()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	QString apiUrl = kitsuApiUrlLineEdit->text();

	// Make sure the reply isn't in error state
	if (reply->error() == QNetworkReply::NoError)
	{
		// Read the response data
		QByteArray responseData = reply->readAll();
		QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

		// Try to read the reply data as a json
		if (!jsonResponse.isNull() && jsonResponse.isObject())
		{
			// Success if {"login" : true} exists in reponse
			QString keyToCheck = "api";
			QJsonObject jsonObject = jsonResponse.object();
			QJsonValue apiValue = jsonObject.value(keyToCheck);

			if (!apiValue.isUndefined() && apiValue.isString() && apiValue.toString() == "Zou")
			{
				AttemptAuth();
				return;
			}
			else
			{
				QMessageBox::warning(this, QString("Invalid url : ") + apiUrl, "Url is not a kitsu api instance.");
				kitsuApiUrlLineEdit->setText("");
			}
		}
		else
		{
			QMessageBox::warning(this, QString("Invalid url : ") + apiUrl, "Url is not a kitsu api instance.");
			kitsuApiUrlLineEdit->setText("");
		}
	}
	else
	{
		QMessageBox::warning(this, reply->errorString(), "Can't connect to " + apiUrl);
		kitsuApiUrlLineEdit->setText("");
	}

	SetInputsDisabled(false);
}

void ConnectionDialog::SetInputsDisabled(bool state)
{
	kitsuApiUrlLineEdit->setDisabled(state);
	mailLineEdit->setDisabled(state);
	passwordLineEdit->setDisabled(state);
	connectButton->setDisabled(state);
}

