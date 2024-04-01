#ifndef CONNECT_BUTTON_H
#define CONNECT_BUTTON_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QChar>
#include <QTimer>
#include <QPushButton>

class LoadingButton : public QPushButton {
    Q_OBJECT

public:
    explicit LoadingButton(const QString& text, QWidget* parent = nullptr);

    // Method to start the text animation
    void StartLoading();

    // Method to stop the text animation
    void StopLoading();

    // Overload setText
    void SetButtonText(QString text);

private:
    QString m_baseText;
    QTimer* m_Timer;

    const QString brailleChars[4] = {
        QChar(0x2834), QChar(0x2826), QChar(0x2816), QChar(0x2832),
    };
    quint8 currentFrameIndex = 0;


private slots:
    // Method to update the text animation
    void m_UpdateAnimation();

};

#endif // CONNECT_BUTTON_H
