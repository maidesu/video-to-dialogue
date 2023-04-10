#pragma once

#include "../fileinfo.hpp"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QWidget>

namespace DialogueFromVideo {

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

public slots:
    void fileChangedHandler(const QList<DialogueFromVideo::SubInfo*>& subStreams,
                            const QList<DialogueFromVideo::AudioInfo*>& audioStreams);

    void applySettingsButtonHandler();

    void initialSettingsHandler(int64_t usPaddingLeft,
                                int64_t usPaddingRight,
                                int64_t usOffset,
                                int64_t usMerge);

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
    QSpinBox* m_subOffsetSpinBox;
    QSpinBox* m_subMergeSpinBox;

    QPushButton* m_openFileButton;
    QPushButton* m_extractDialogueButton;
    QPushButton* m_applySettingsButton;

signals:
    void openFileSignal();

    void applySettingsSignal(int64_t usPaddingLeft,
                             int64_t usPaddingRight,
                             int64_t usOffset,
                             int64_t usMerge);
};

} // namespace DialogueFromVideo
