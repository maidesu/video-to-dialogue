#include "window.hpp"
#include "../common/console.hpp"

#include <QSplitter>

namespace DialogueFromVideo {

Window::Window(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_infoGroupBox(new QGroupBox(tr("File details")))
    , m_settingsGroupBox(new QGroupBox(tr("Subtitle timing")))
    , m_subGroupBox(new QGroupBox(tr("Subtitle")))
    , m_audioGroupBox(new QGroupBox(tr("Audio")))
    , m_subComboBox(new QComboBox())
    , m_audioComboBox(new QComboBox())
    , m_subLayerSpinBox(new QSpinBox())
    , m_subPaddingLeftSpinBox(new QSpinBox())
    , m_subPaddingRightSpinBox(new QSpinBox())
    , m_subMergeSpinBox(new QSpinBox())
    , m_openFileButton(new QPushButton(tr("Open file")))
    , m_extractDialogueButton(new QPushButton(tr("Extract dialogue")))
    , m_applySettingsButton(new QPushButton(tr("Apply")))
{
    this->setMinimumSize(700, 400);

    m_subComboBox->setDisabled(true);
    m_audioComboBox->setDisabled(true);

    // Info group box contains
    QHBoxLayout* infoLayout = new QHBoxLayout();
    m_infoGroupBox->setLayout(infoLayout);
    infoLayout->addWidget(m_subGroupBox);
    infoLayout->addWidget(m_audioGroupBox);

    // Sub timing group box contains
    QHBoxLayout* settingsLayout = new QHBoxLayout();
    m_settingsGroupBox->setLayout(settingsLayout);
    settingsLayout->addWidget(m_subPaddingLeftSpinBox);
    settingsLayout->addWidget(m_subPaddingRightSpinBox);
    settingsLayout->addWidget(m_subMergeSpinBox);
    settingsLayout->addWidget(m_applySettingsButton);

    // Sub group box contains
    QHBoxLayout* subLayout = new QHBoxLayout();
    m_subGroupBox->setLayout(subLayout);
    subLayout->addWidget(m_subComboBox);
    subLayout->addWidget(m_subLayerSpinBox);
    //subLayout->addWidget(); TODO: Add properties

    // Audio group box contains
    QHBoxLayout* audioLayout = new QHBoxLayout();
    m_audioGroupBox->setLayout(audioLayout);
    audioLayout->addWidget(m_audioComboBox);
    //audioLayout->addWidget(); TODO: Add properties

    // Action buttons
    QWidget* buttonWidget = new QWidget();
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonWidget->setLayout(buttonLayout);
    buttonLayout->addWidget(m_openFileButton);
    buttonLayout->addWidget(m_extractDialogueButton);
    //buttonLayout->addWidget(m_applySettingsButton);

    infoLayout->addWidget(buttonWidget);

    // Top content container
    QWidget* topContainer = new QWidget();
    QVBoxLayout* topLayout = new QVBoxLayout();
    topContainer->setLayout(topLayout);
    topLayout->addWidget(m_infoGroupBox);
    topLayout->addWidget(m_settingsGroupBox);

    // Dynamic splitter
    QSplitter* splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(topContainer);
    splitter->addWidget(Console::instance().textEdit());

    // Main layout
    m_layout->addWidget(splitter);

    // Connect
    connect(m_openFileButton, &QPushButton::pressed, this, &Window::openFileSignal);
}

void Window::fileChangedHandler(const QList<DialogueFromVideo::SubInfo*>& subStreams,
                                const QList<DialogueFromVideo::AudioInfo*>& audioStreams)
{
    emit m_windowMessenger.print("File changed, updating file info...", "MainWindow", MessageLevel::Debug);

    m_subComboBox->clear();
    m_audioComboBox->clear();

    for (const SubInfo* const si : subStreams)
        m_subComboBox->addItem(QString::number(si->index));

    for (const AudioInfo* const ai : audioStreams)
        m_audioComboBox->addItem(QString::number(ai->index));


    m_subComboBox->setDisabled(m_subComboBox->count() < 1); // Disable if combobox content is less than 1
    m_audioComboBox->setDisabled(m_audioComboBox->count() < 1);
}

} // namespace DialogueFromVideo
