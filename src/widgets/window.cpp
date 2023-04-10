#include "../../include/widgets/window.hpp"

#include "../../include/common/console.hpp"

#include <QLabel>
#include <QSplitter>

namespace DialogueFromVideo {

Window::Window(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_infoGroupBox(new QGroupBox(tr("File details")))
    , m_subSettingsGroupBox(new QGroupBox(tr("Subtitle timing")))
    , m_subGroupBox(new QGroupBox(tr("Subtitle")))
    , m_audioGroupBox(new QGroupBox(tr("Audio")))
    , m_subComboBox(new QComboBox())
    , m_audioComboBox(new QComboBox())
    , m_subLayerSpinBox(new QSpinBox())
    , m_subPaddingLeftSpinBox(new QSpinBox())
    , m_subPaddingRightSpinBox(new QSpinBox())
    , m_subOffsetSpinBox(new QSpinBox())
    , m_subMergeSpinBox(new QSpinBox())
    , m_openFileButton(new QPushButton(tr("Open file")))
    , m_extractDialogueButton(new QPushButton(tr("Extract dialogue")))
    , m_applySettingsButton(new QPushButton(tr("Apply")))
    , m_progressBar(new QProgressBar())
{
    this->setMinimumSize(700, 500);

    m_subComboBox->setDisabled(true);
    m_audioComboBox->setDisabled(true);

    // Info group box contains
    QHBoxLayout* infoLayout = new QHBoxLayout();
    m_infoGroupBox->setLayout(infoLayout);
    infoLayout->addWidget(m_subGroupBox, 3);   // 3/6 space of infoLayout
    infoLayout->addWidget(m_audioGroupBox, 2); // 2/6 space of infoLayout

    // Sub timing group box contains
    QHBoxLayout* subSettingsLayout = new QHBoxLayout();
    m_subSettingsGroupBox->setLayout(subSettingsLayout);

    QWidget* settingsRowsWidget = new QWidget();
    QVBoxLayout* settingsRowsLayout = new QVBoxLayout();
    settingsRowsWidget->setLayout(settingsRowsLayout);
    subSettingsLayout->addWidget(settingsRowsWidget, 5); // 5/6 space of subSettingsLayout

    QWidget* settingsPaddingWidget = new QWidget();
    QWidget* settingsMiscWidget = new QWidget();
    QHBoxLayout* settingsPaddingLayout = new QHBoxLayout();
    QHBoxLayout* settingsMiscLayout = new QHBoxLayout();
    settingsPaddingWidget->setLayout(settingsPaddingLayout);
    settingsMiscWidget->setLayout(settingsMiscLayout);
    settingsRowsLayout->addWidget(settingsPaddingWidget);
    settingsRowsLayout->addWidget(settingsMiscWidget);

    settingsPaddingLayout->addWidget(new QLabel("Padding left (ms)"));
    m_subPaddingLeftSpinBox->setSingleStep(100);
    m_subPaddingLeftSpinBox->setMinimum(0);
    m_subPaddingLeftSpinBox->setMaximum(INT_MAX);
    m_subPaddingLeftSpinBox->setMinimumWidth(60);
    settingsPaddingLayout->addWidget(m_subPaddingLeftSpinBox);

    settingsPaddingLayout->addWidget(new QLabel("Padding right (ms)"));
    m_subPaddingRightSpinBox->setSingleStep(100);
    m_subPaddingRightSpinBox->setMinimum(0);
    m_subPaddingRightSpinBox->setMaximum(INT_MAX);
    m_subPaddingRightSpinBox->setMinimumWidth(60);
    settingsPaddingLayout->addWidget(m_subPaddingRightSpinBox);

    settingsMiscLayout->addWidget(new QLabel("Offset (ms)"));
    m_subOffsetSpinBox->setSingleStep(100);
    m_subOffsetSpinBox->setMinimum(INT_MIN);
    m_subOffsetSpinBox->setMaximum(INT_MAX);
    m_subOffsetSpinBox->setMinimumWidth(60);
    settingsMiscLayout->addWidget(m_subOffsetSpinBox);

    settingsMiscLayout->addWidget(new QLabel("Minimum gap (ms)"));
    m_subMergeSpinBox->setSingleStep(100);
    m_subMergeSpinBox->setMinimum(0);
    m_subMergeSpinBox->setMaximum(INT_MAX);
    m_subMergeSpinBox->setMinimumWidth(60);
    settingsMiscLayout->addWidget(m_subMergeSpinBox);

    subSettingsLayout->addWidget(m_applySettingsButton, 1); // 1/6 space of subSettingsLayout

    // Sub group box contains
    QHBoxLayout* subLayout = new QHBoxLayout();
    m_subGroupBox->setLayout(subLayout);

    subLayout->addWidget(new QLabel("Subtitle stream (id)"), 3); // 3/8 space of subLayout
    m_subComboBox->setMinimumWidth(40);
    subLayout->addWidget(m_subComboBox, 1); // 1/8 space of subLayout

    subLayout->addWidget(new QLabel("Subtitle layer (id)"), 3); // 3/8 space of subLayout
    m_subLayerSpinBox->setMinimumWidth(40);
    subLayout->addWidget(m_subLayerSpinBox, 1); // 1/8 space of subLayout

    // Audio group box contains
    QHBoxLayout* audioLayout = new QHBoxLayout();
    m_audioGroupBox->setLayout(audioLayout);

    audioLayout->addWidget(new QLabel("Audio stream (id)"), 3);
    m_audioComboBox->setMinimumWidth(40);
    audioLayout->addWidget(m_audioComboBox, 1);

    // Action buttons
    QWidget* buttonWidget = new QWidget();
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonWidget->setLayout(buttonLayout);
    buttonLayout->addWidget(m_openFileButton);
    buttonLayout->addWidget(m_extractDialogueButton);

    infoLayout->addWidget(buttonWidget, 1); // 1/6 space of infoLayout

    // Top content container
    QWidget* topContainer = new QWidget();
    QVBoxLayout* topLayout = new QVBoxLayout();
    topContainer->setLayout(topLayout);
    topLayout->addWidget(m_infoGroupBox);
    topLayout->addWidget(m_subSettingsGroupBox);
    topLayout->addWidget(m_progressBar);

    m_progressBar->setAlignment(Qt::Alignment::enum_type::AlignHCenter);

    // Dynamic splitter
    QSplitter* splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(topContainer);
    splitter->addWidget(Console::instance().textEdit());

    // Main layout
    m_layout->addWidget(splitter);

    // Connect
    connect(m_openFileButton, &QPushButton::pressed, this, &Window::openFileSignal);

    connect(m_applySettingsButton, &QPushButton::pressed, this, &Window::applySettingsButtonHandler);
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

void Window::applySettingsButtonHandler()
{
    emit applySettingsSignal(static_cast<int64_t>(m_subPaddingLeftSpinBox->value() * 1000),
                             static_cast<int64_t>(m_subPaddingRightSpinBox->value() * 1000),
                             static_cast<int64_t>(m_subOffsetSpinBox->value() * 1000),
                             static_cast<int64_t>(m_subMergeSpinBox->value() * 1000));
}

void Window::initialSettingsHandler(int64_t usPaddingLeft,
                                    int64_t usPaddingRight,
                                    int64_t usOffset,
                                    int64_t usMerge)
{
    m_subPaddingLeftSpinBox->setValue(static_cast<int>(usPaddingLeft / 1000));
    m_subPaddingRightSpinBox->setValue(static_cast<int>(usPaddingRight / 1000));
    m_subOffsetSpinBox->setValue(static_cast<int>(usOffset / 1000));
    m_subMergeSpinBox->setValue(static_cast<int>(usMerge / 1000));
}

} // namespace DialogueFromVideo
