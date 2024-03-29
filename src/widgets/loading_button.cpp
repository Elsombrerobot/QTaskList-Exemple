#include <QWidget>
#include <QString>
#include <QPushButton>

#include "loading_button.h"

LoadingButton::LoadingButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
    // Configure the timer to update the animation every 100 milliseconds
    m_Timer.setInterval(100);

    // Set memory text
    SetButtonText(text);

    // Connect timer to animation update
    connect(&m_Timer, &QTimer::timeout, this, &LoadingButton::m_UpdateAnimation);
    //connect(
    //    &m_Timer,
    //    SIGNAL(&QTimer::timeout()),
    //    this,
    //    SLOT(&LoadingLabel::m_UpdateAnimation()));
}

// Use this function to set button text, so text can changed while loading.
void LoadingButton::SetButtonText(const QString& text) {
    m_baseText = text;
}

// Method to update the text animation
void LoadingButton::m_UpdateAnimation() {
    // Update to the next Braille character
    setText(m_baseText + " " + brailleChars[currentFrameIndex]);
    currentFrameIndex = (currentFrameIndex + 1) % 4;
}

// Method to start the text animation
void LoadingButton::StartLoading()
{
    setText(m_baseText + " " + brailleChars[0]);
    if (!m_Timer.isActive()) {
        // Start the timer to update the animation
        m_Timer.start();
    }
}

// Method to stop the text animation
void LoadingButton::StopLoading()
{
    // Stop the timer
    m_Timer.stop();
    setText(m_baseText);

}


