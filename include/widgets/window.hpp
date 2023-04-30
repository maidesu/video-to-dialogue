#pragma once

#include "../fileinfo.hpp"

#include <QLabel>
#include <QProgressBar>
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

    void subDescriptionReceivedHandler(const SubInfo subInfo); // Receive selected subtitle description copy

    void audioDescriptionReceivedHandler(const AudioInfo audioInfo); // Receive selected audio description copy

private:
    Messenger m_windowMessenger;

    QVBoxLayout* m_layout;

    QGroupBox* m_infoGroupBox;
    QGroupBox* m_subSettingsGroupBox;
    QGroupBox* m_subGroupBox;
    QGroupBox* m_audioGroupBox;

    QComboBox* m_subComboBox;
    QComboBox* m_audioComboBox;
    QComboBox* m_languageComboBox;

    QLabel* m_subDescriptionLabel;
    QLabel* m_audioDescriptionLabel;

    QSpinBox* m_subLayerSpinBox;
    QSpinBox* m_subPaddingLeftSpinBox;
    QSpinBox* m_subPaddingRightSpinBox;
    QSpinBox* m_subOffsetSpinBox;
    QSpinBox* m_subMergeSpinBox;

    QPushButton* m_openFileButton;
    QPushButton* m_extractDialogueButton;
    QPushButton* m_applySettingsButton;
    QPushButton* m_exportSubtitleButton;
    QPushButton* m_exportPictureCollectionButton;

    QTextEdit* m_subTextEdit;

    QProgressBar* m_progressBar;

signals:
    void openFileSignal();

    void applySettingsSignal(int64_t usPaddingLeft,
                             int64_t usPaddingRight,
                             int64_t usOffset,
                             int64_t usMerge);

    void languageSettingsChangedSignal(const QString& language);

    void subDescriptionRequestedSignal(const QString& index); // Request sub description copy

    void subLayerRequestedSignal(const QString& index); // Requst layer change

    void audioDescriptionRequestedSignal(const QString& index); // Request audio description copy
};

} // namespace DialogueFromVideo
