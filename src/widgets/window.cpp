#include <widgets/window.hpp>

#include <common/console.hpp>
#include <common/time.hpp>
#include <common/progressbar.hpp>

#include <QLabel>
#include <QSplitter>
#include <QTabWidget>

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
    , m_languageComboBox(new QComboBox())
    , m_consoleLevelComboBox(new QComboBox())
    , m_lightUiRadioButton(new QRadioButton(tr("Light")))
    , m_darkUiRadioButton(new QRadioButton(tr("Dark")))
    , m_subDescriptionLabel(new QLabel())
    , m_audioDescriptionLabel(new QLabel())
    , m_subLayerSpinBox(new QSpinBox())
    , m_subPaddingLeftSpinBox(new QSpinBox())
    , m_subPaddingRightSpinBox(new QSpinBox())
    , m_subOffsetSpinBox(new QSpinBox())
    , m_subMergeSpinBox(new QSpinBox())
    , m_openFileButton(new QPushButton(tr("Open file")))
    , m_extractDialogueButton(new QPushButton(tr("Extract dialogue")))
    , m_applySettingsButton(new QPushButton(tr("Apply")))
    , m_consoleClearButton(new QPushButton(tr("Clear")))
    , m_exportSubtitleButton(new QPushButton(tr("Export subtitle")))
    , m_exportPictureCollectionButton(new QPushButton(tr("Create picture book")))
    , m_subTextEdit(new QTextEdit())
{
    this->setMinimumSize(900, 720);

    m_subComboBox->setDisabled(true);
    m_subLayerSpinBox->setDisabled(true);
    m_audioComboBox->setDisabled(true);

    // Info group box
    QHBoxLayout* infoLayout = new QHBoxLayout();
    m_infoGroupBox->setLayout(infoLayout);
    infoLayout->addWidget(m_subGroupBox, 3);   // 3/6 space of infoLayout
    infoLayout->addWidget(m_audioGroupBox, 2); // 2/6 space of infoLayout

    // Sub timing group box
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

    settingsPaddingLayout->addWidget(new QLabel(tr("Padding left (ms)")));
    m_subPaddingLeftSpinBox->setSingleStep(100);
    m_subPaddingLeftSpinBox->setMinimum(0);
    m_subPaddingLeftSpinBox->setMaximum(INT_MAX);
    m_subPaddingLeftSpinBox->setMinimumWidth(60);
    settingsPaddingLayout->addWidget(m_subPaddingLeftSpinBox);

    settingsPaddingLayout->addWidget(new QLabel(tr("Padding right (ms)")));
    m_subPaddingRightSpinBox->setSingleStep(100);
    m_subPaddingRightSpinBox->setMinimum(0);
    m_subPaddingRightSpinBox->setMaximum(INT_MAX);
    m_subPaddingRightSpinBox->setMinimumWidth(60);
    settingsPaddingLayout->addWidget(m_subPaddingRightSpinBox);

    settingsMiscLayout->addWidget(new QLabel(tr("Offset (ms)")));
    m_subOffsetSpinBox->setSingleStep(100);
    m_subOffsetSpinBox->setMinimum(INT_MIN);
    m_subOffsetSpinBox->setMaximum(INT_MAX);
    m_subOffsetSpinBox->setMinimumWidth(60);
    settingsMiscLayout->addWidget(m_subOffsetSpinBox);

    settingsMiscLayout->addWidget(new QLabel(tr("Minimum gap (ms)")));
    m_subMergeSpinBox->setSingleStep(100);
    m_subMergeSpinBox->setMinimum(0);
    m_subMergeSpinBox->setMaximum(INT_MAX);
    m_subMergeSpinBox->setMinimumWidth(60);
    settingsMiscLayout->addWidget(m_subMergeSpinBox);

    subSettingsLayout->addWidget(m_applySettingsButton, 1); // 1/6 space of subSettingsLayout

    // Sub group box
    QHBoxLayout* subLayout = new QHBoxLayout();
    m_subGroupBox->setLayout(subLayout);

    subLayout->addWidget(new QLabel(tr("Subtitle stream (id)")), 3); // 3/10 space of subLayout
    m_subComboBox->setMinimumWidth(40);
    subLayout->addWidget(m_subComboBox, 1); // 1/10 space of subLayout

    subLayout->addWidget(new QLabel(tr("Subtitle layer (id)")), 3); // 3/10 space of subLayout
    m_subLayerSpinBox->setMinimumWidth(40);
    subLayout->addWidget(m_subLayerSpinBox, 1); // 1/10 space of subLayout

    subLayout->addWidget(m_subDescriptionLabel, 2); // 2/10 space of subLayout

    // Audio group box
    QHBoxLayout* audioLayout = new QHBoxLayout();
    m_audioGroupBox->setLayout(audioLayout);

    audioLayout->addWidget(new QLabel(tr("Audio stream (id)")), 3); // 3/5 space of audioLayout
    m_audioComboBox->setMinimumWidth(40);
    audioLayout->addWidget(m_audioComboBox, 1); // 1/5 space of audioLayout

    audioLayout->addWidget(m_audioDescriptionLabel, 2); // 2/5 space of audioLayout

    // Action buttons
    QWidget* buttonWidget = new QWidget();
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonWidget->setLayout(buttonLayout);
    buttonLayout->addWidget(m_openFileButton);
    buttonLayout->addWidget(m_extractDialogueButton);

    infoLayout->addWidget(buttonWidget, 1); // 1/6 space of infoLayout

    // Subtitle text edit
    m_subTextEdit->setReadOnly(true);
    m_subTextEdit->insertPlainText("<no subtitle loaded>");
    m_subTextEdit->ensureCursorVisible();

    // Subtitle text edit buttons
    QWidget* textEditButtonsContainer = new QWidget();
    QHBoxLayout* textEditButtonsContainerLayout = new QHBoxLayout();
    textEditButtonsContainer->setLayout(textEditButtonsContainerLayout);
    textEditButtonsContainerLayout->addWidget(m_exportSubtitleButton);
    textEditButtonsContainerLayout->addWidget(m_exportPictureCollectionButton);

    // Languages
    m_languageComboBox->addItem("English", "en_US");
    m_languageComboBox->addItem("Magyar", "hu_HU");

    // Console levels
    qsizetype i = 0;
#ifndef QT_DEBUG
    i = 1;
#endif
    for (; i < Console::instance().messageLevelLabels.size(); ++i)
    {
        m_consoleLevelComboBox->addItem(Console::instance().messageLevelLabels.at(i),
                                        QVariant::fromValue(static_cast<MessageLevel>(i)));
#ifndef QT_DEBUG
        m_consoleLevelComboBox->setItemData(i-1,
#else
        m_consoleLevelComboBox->setItemData(i,
#endif
                                            QBrush(Console::instance().messageLevelColors.at(i)),
                                            Qt::ForegroundRole);
    }

    QPalette firstElementPalette = QPalette();
    firstElementPalette.setColor(QPalette::ButtonText,
                                  Console::instance().messageLevelColors.at(
                                      static_cast<qsizetype>( m_consoleLevelComboBox->itemData(0).value<MessageLevel>() )
                                      ));
    m_consoleLevelComboBox->setPalette(firstElementPalette);

    // Tabs
    QTabWidget* tabWidget = new QTabWidget();

    QWidget* fileContainer = new QWidget();
    QWidget* subtitleContainer = new QWidget();
    QWidget* exportContainer = new QWidget();
    QWidget* settingsContainer = new QWidget();

    QVBoxLayout* fileContainerLayout = new QVBoxLayout();
    QVBoxLayout* subtitleContainerLayout = new QVBoxLayout();
    QVBoxLayout* exportContainerLayout = new QVBoxLayout();
    QVBoxLayout* settingsContainerLayout = new QVBoxLayout();

    fileContainer->setLayout(fileContainerLayout);
    subtitleContainer->setLayout(subtitleContainerLayout);
    exportContainer->setLayout(exportContainerLayout);
    settingsContainer->setLayout(settingsContainerLayout);

    // Tabs: File tab
    fileContainerLayout->addWidget(m_infoGroupBox);
    fileContainerLayout->addWidget(m_subSettingsGroupBox);

    // Tabs: Subtitle tab
    subtitleContainerLayout->addWidget(m_subTextEdit);
    subtitleContainerLayout->addWidget(textEditButtonsContainer);

    // Tabs: Export tab

    // Tabs: Settings tab
    settingsContainerLayout->setSpacing(20);
    settingsContainerLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    settingsContainerLayout->addWidget(new QLabel(tr("Language")));
    settingsContainerLayout->addWidget(m_languageComboBox);
    settingsContainerLayout->addWidget(new QLabel(tr("UI mode")));
    settingsContainerLayout->addWidget(m_lightUiRadioButton);
    settingsContainerLayout->addWidget(m_darkUiRadioButton);

    // Tabs: Final
    tabWidget->addTab(fileContainer, tr("File"));
    tabWidget->addTab(subtitleContainer, tr("Subtitle"));
    tabWidget->addTab(exportContainer, tr("Export"));
    tabWidget->addTab(settingsContainer, tr("Settings"));

    // Top content container
    QWidget* topContainer = new QWidget();
    QVBoxLayout* topLayout = new QVBoxLayout();
    topContainer->setLayout(topLayout);
    topLayout->addWidget(tabWidget);
    topLayout->addWidget(ProgressBar::instance().progressBar());

    ProgressBar::instance().progressBar()->setAlignment(Qt::Alignment::enum_type::AlignHCenter);


    // Console options palette
    QPalette consoleOptsPalette = QPalette();
    consoleOptsPalette.setColor(QPalette::ButtonText, Qt::white);
    consoleOptsPalette.setColor(QPalette::Button, Qt::black);
    consoleOptsPalette.setColor(QPalette::Window, Qt::black);

    // Console options container
    QWidget* consoleOptsContainer = new QWidget();
    consoleOptsContainer->setAutoFillBackground(true);
    consoleOptsContainer->setPalette(consoleOptsPalette);
    QHBoxLayout* consoleOptsLayout = new QHBoxLayout();
    consoleOptsLayout->addStretch();
    consoleOptsLayout->setAlignment(Qt::AlignRight);
    consoleOptsContainer->setLayout(consoleOptsLayout);
    consoleOptsLayout->addWidget(m_consoleLevelComboBox);
    m_consoleClearButton->setMaximumWidth(60);
    consoleOptsLayout->addWidget(m_consoleClearButton);

    // Horizontal line for console
    QFrame* hLine = new QFrame();
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Raised);

    // Bottom container
    QWidget* bottomContainer = new QWidget();
    QVBoxLayout* bottomLayout = new QVBoxLayout();
    bottomLayout->setSpacing(0);
    bottomContainer->setLayout(bottomLayout);
    bottomLayout->addWidget(Console::instance().textEdit());
    bottomLayout->addWidget(hLine);
    bottomLayout->addWidget(consoleOptsContainer);

    // Dynamic splitter
    QSplitter* splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(topContainer);
    splitter->addWidget(bottomContainer);

    // Main layout
    m_layout->addWidget(splitter);

    // Connect
    connect(m_openFileButton,
            &QPushButton::pressed,
            this,
            &Window::openFileSignal);

    connect(m_applySettingsButton,
            &QPushButton::pressed,
            this,
            &Window::applySettingsButtonHandler);

    connect(m_consoleClearButton,
            &QPushButton::pressed,
            &Console::instance(),
            &Console::clearHandler);

    connect(m_exportSubtitleButton,
            &QPushButton::pressed,
            this,
            &Window::exportSubtitleSignal);

    connect(m_exportPictureCollectionButton,
            &QPushButton::pressed,
            this,
            &Window::exportPictureCollectionSignal);

    connect(m_subComboBox,
            &QComboBox::currentTextChanged,
            this,
            &Window::subDescriptionRequestedSignal);

    connect(m_subLayerSpinBox,
            &QSpinBox::textChanged,
            this,
            &Window::subLayerRequestedSignal);

    connect(m_audioComboBox,
            &QComboBox::currentTextChanged,
            this,
            &Window::audioDescriptionRequestedSignal);

    connect(m_languageComboBox,
            &QComboBox::currentIndexChanged,
            this,
            [this](int index)
            {
                emit Window::languageSettingsChangedSignal(m_languageComboBox
                                                                ->itemData(index)
                                                                .toString());
            });

    connect(this,
            &Window::consoleFilterSignal,
            &Console::instance(),
            &Console::filterHandler);

    connect(m_consoleLevelComboBox,
            &QComboBox::currentIndexChanged,
            this,
            [this]()
            {
                QPalette activeElementPalette = QPalette();
                activeElementPalette.setColor(QPalette::ButtonText,
                                              Console::instance().messageLevelColors.at(
                                                  static_cast<qsizetype>( m_consoleLevelComboBox->currentData().value<MessageLevel>() )
                                              ));
                m_consoleLevelComboBox->setPalette(activeElementPalette);

                emit consoleFilterSignal(m_consoleLevelComboBox->currentData().value<MessageLevel>());
            });

    connect(m_lightUiRadioButton,
            &QRadioButton::clicked,
            this,
            [this]()
            {
                emit Window::colorSchemeSettingsChangedSignal(false);
            });

    connect(m_darkUiRadioButton,
            &QRadioButton::clicked,
            this,
            [this]()
            {
                emit Window::colorSchemeSettingsChangedSignal(true);
            });
}

void Window::fileChangedHandler(const QList<DialogueFromVideo::SubInfo*>& subStreams,
                                const QList<DialogueFromVideo::AudioInfo*>& audioStreams)
{
    emit m_windowMessenger.print("File changed, updating file info...",
                                 "MainWindow",
                                 MessageLevel::Debug);

    m_subTextEdit->clear();
    m_subTextEdit->insertPlainText("<no subtitle loaded>");
    emit subtitleClearSignal();

    m_subDescriptionLabel->clear();
    m_audioDescriptionLabel->clear();

    m_subComboBox->clear();
    m_audioComboBox->clear();

    for (const SubInfo* const si : subStreams)
        m_subComboBox->addItem(QString::number(si->index));

    for (const AudioInfo* const ai : audioStreams)
        m_audioComboBox->addItem(QString::number(ai->index));

    // Disable layers if no subs are present
    m_subLayerSpinBox->setDisabled(m_subComboBox->count() < 1);
    // Disable if combobox content is less than 1
    m_subComboBox->setDisabled(m_subComboBox->count() < 1);
    m_audioComboBox->setDisabled(m_audioComboBox->count() < 1);

    // Force update
    if (!m_subLayerSpinBox->isEnabled())
    {
        emit m_subLayerSpinBox->textChanged(QString::number(-1));
    }
    else
    {
        if (m_subLayerSpinBox->value() == 0)
        {
            emit m_subLayerSpinBox->textChanged(QString::number(0));
        }
        else
        {
            // setValue only emits textChanged if the value is actually changed
            m_subLayerSpinBox->setValue(0);
        }
    }
    if (!m_subComboBox->isEnabled())
    {
        emit m_subComboBox->currentTextChanged("-1");
    }
    if (!m_audioComboBox->isEnabled())
    {
        emit m_audioComboBox->currentTextChanged("-1");
    }
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

void Window::initialLanguageHandler(const QString& language)
{
    m_languageComboBox->blockSignals(true);

    m_languageComboBox->setCurrentIndex(m_languageComboBox->findData(language));

    m_languageComboBox->blockSignals(false);
}

void Window::initialColorSchemeHandler(bool darkModeEnabled)
{
    m_lightUiRadioButton->blockSignals(true);
    m_darkUiRadioButton->blockSignals(true);

    if (!darkModeEnabled)
    {
        m_lightUiRadioButton->click();
    }
    else
    {
        m_darkUiRadioButton->click();
    }

    m_lightUiRadioButton->blockSignals(false);
    m_darkUiRadioButton->blockSignals(false);
}

void Window::subDescriptionReceivedHandler(const SubInfo subInfo)
{
    emit m_windowMessenger.print(QString("subDescriptionReceivedHandler()"),
                                 "Window",
                                 MessageLevel::Debug);

    m_subDescriptionLabel->setText(tr("Index: %1\n"
                                      "Language: %2\n"
                                      "Codec: %3")
                                       .arg(QString::number(subInfo.index),
                                            subInfo.lang != "und"
                                                ? subInfo.lang
                                                : QString("N/A"),
                                            subInfo.codec_name));
}

void Window::audioDescriptionReceivedHandler(const AudioInfo audioInfo)
{
    emit m_windowMessenger.print(QString("audioDescriptionReceivedHandler()"),
                                 "Window",
                                 MessageLevel::Debug);

    m_audioDescriptionLabel->setText(tr("Index: %1\n"
                                        "Sample rate: %2 Hz\n"
                                        "Bits per sample: %3\n"
                                        "Bitrate: %4 kbps\n"
                                        "Lossless: %5\n"
                                        "Language: %6\n"
                                        "Codec: %7")
                                         .arg(QString::number(audioInfo.index),
                                              QString::number(audioInfo.samplerate),
                                              audioInfo.bitdepth != 0
                                                ? QString::number(audioInfo.bitdepth)
                                                : QString("N/A"),
                                              audioInfo.bitrate != 0
                                                ? QString::number(audioInfo.bitrate)
                                                : QString("N/A"),
                                              audioInfo.lossless ? tr("Yes") : tr("No"),
                                              audioInfo.lang != "und"
                                                ? audioInfo.lang
                                                : QString("N/A"),
                                              audioInfo.codec_name));
}

void Window::subtitleExtractedHandler(const QList<SubEntry*>& subs)
{
    emit m_windowMessenger.print(QString("Subtitle content received"),
                                 "Window",
                                 MessageLevel::Debug);

    m_subTextEdit->clear();

    if (subs.isEmpty())
    {
        m_subTextEdit->insertPlainText("<no subtitle loaded>");
        return;
    }

    for (SubEntry* sub : subs)
    {
        m_subTextEdit->insertPlainText(QString("%1 -> %2\n%3\n")
                                           .arg(Time::millisecondsToStringTime(sub->start),
                                                Time::millisecondsToStringTime(sub->end),
                                                sub->text));
    }
}

} // namespace DialogueFromVideo
