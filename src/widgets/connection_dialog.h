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
	void m_AttemptApiConnection();
	void m_HandleApiReponse();
	void m_AttemptAuth();
	void m_HandleAuthResponse();
	void m_SetInputsDisabled(bool state);

private:
	QLineEdit* m_kitsuApiUrlLineEdit;
	QLineEdit* m_mailLineEdit;
	QLineEdit* m_passwordLineEdit;
	QPushButton* m_connectButton;

private:
	void m_clearCredentialFields();
};

#endif // CONNECTION_DIALOG_H
