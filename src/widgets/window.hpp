#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>

#include "../fileinfo.hpp"

namespace DialogueFromVideo {

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

public slots:
    void fileChangedHandler(const QList<DialogueFromVideo::SubInfo*>& subStreams,
                            const QList<DialogueFromVideo::AudioInfo*>& audioStreams);

private:
    Messenger m_windowMessenger;

    QVBoxLayout* m_layout;

    QGroupBox* m_infoGroupBox;
    QGroupBox* m_settingsGroupBox;
    QGroupBox* m_subGroupBox;
    QGroupBox* m_audioGroupBox;

    QComboBox* m_subComboBox;
    QComboBox* m_audioComboBox;

    QSpinBox* m_subLayerSpinBox;
    QSpinBox* m_subPaddingLeftSpinBox;
    QSpinBox* m_subPaddingRightSpinBox;
    QSpinBox* m_subMergeSpinBox;

    QPushButton* m_openFileButton;
    QPushButton* m_extractDialogueButton;
    QPushButton* m_applySettingsButton;

signals:
    void openFileSignal();
};

} // namespace DialogueFromVideo
