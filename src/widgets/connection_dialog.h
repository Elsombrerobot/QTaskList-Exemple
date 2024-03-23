// connectiondialog.h
#ifndef CONNECTION_DIALOG_H
#define CONNECTION_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkReply>
#include <QUrl>

class ConnectionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConnectionDialog(QWidget* parent = nullptr);

private slots:
	void AttemptApiConnection();
	void HandleApiReponse();
	void AttemptAuth();
	void HandleAuthResponse();
	void SetInputsDisabled(bool state);

private:
	QLineEdit* kitsuApiUrlLineEdit;
	QLineEdit* mailLineEdit;
	QLineEdit* passwordLineEdit;
	QPushButton* connectButton;
};

#endif // CONNECTION_DIALOG_H
