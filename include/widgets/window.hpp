#pragma once

#include <subtitle/subtitle.hpp>
#include <fileinfo.hpp>

#include <QLabel>
#include <QRadioButton>
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
    void fileChangedHandler(const QList<SubInfo*>& subStreams,
                            const QList<AudioInfo*>& audioStreams);

    void applySettingsButtonHandler();

    void initialSettingsHandler(int64_t usPaddingLeft,
                                int64_t usPaddingRight,
                                int64_t usOffset,
                                int64_t usMerge);

    void initialLanguageHandler(const QString& language);

    void initialColorSchemeHandler(bool darkModeEnabled);

    void subDescriptionReceivedHandler(const SubInfo subInfo); // Receive selected subtitle description copy

    void audioDescriptionReceivedHandler(const AudioInfo audioInfo); // Receive selected audio description copy

    void subtitleExtractedHandler(const QList<SubEntry*>& subs);

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

    QRadioButton* m_lightUiRadioButton;
    QRadioButton* m_darkUiRadioButton;

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

signals:
    void openFileSignal();

    void applySettingsSignal(int64_t usPaddingLeft,
                             int64_t usPaddingRight,
                             int64_t usOffset,
                             int64_t usMerge);

    void languageSettingsChangedSignal(const QString& language);

    void colorSchemeSettingsChangedSignal(bool darkModeEnabled);

    void subDescriptionRequestedSignal(const QString& index); // Request sub description copy

    void subLayerRequestedSignal(const QString& index); // Requst layer change

    void audioDescriptionRequestedSignal(const QString& index); // Request audio description copy
};

} // namespace DialogueFromVideo
