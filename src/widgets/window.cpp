#include <widgets/window.hpp>

#include <common/console.hpp>
#include <common/time.hpp>
#include <common/progressbar.hpp>
#include <common/formatopts.hpp>

#include <QGuiApplication>
#include <QChartView>
#include <QTabWidget>
#include <QSplitter>
#include <QLabel>

namespace DialogueFromVideo {

Window::Window(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_infoGroupBox(new QGroupBox(tr("File details")))
    , m_subSettingsGroupBox(new QGroupBox(tr("Subtitle timing")))
    , m_subGroupBox(new QGroupBox(tr("Subtitle")))
    , m_audioGroupBox(new QGroupBox(tr("Audio")))
    , m_exportRemuxGroupBox(new QGroupBox(tr("Remux selection")))
    , m_exportDialogueGroupBox(new QGroupBox(tr("Dialogue")))
    , m_subComboBox(new QComboBox())
    , m_audioComboBox(new QComboBox())
    , m_languageComboBox(new QComboBox())
    , m_consoleLevelComboBox(new QComboBox())
    , m_exportContainerComboBox(new QComboBox())
    , m_exportOptionsComboBox(new QComboBox())
    , m_exportLossyRadioButton(new QRadioButton(tr("Lossy")))
    , m_exportLosslessRadioButton(new QRadioButton(tr("Lossless")))
    , m_lightUiRadioButton(new QRadioButton(tr("Light")))
    , m_darkUiRadioButton(new QRadioButton(tr("Dark")))
    , m_subDescriptionLabel(new QLabel())
    , m_audioDescriptionLabel(new QLabel())
    , m_exportContainerLabel(new QLabel(tr("codec")))
    , m_exportOptionsLabel(new QLabel())
    , m_subLayerSpinBox(new QSpinBox())
    , m_subPaddingLeftSpinBox(new QSpinBox())
    , m_subPaddingRightSpinBox(new QSpinBox())
    , m_subOffsetSpinBox(new QSpinBox())
    , m_subMergeSpinBox(new QSpinBox())
    , m_openFileButton(new QPushButton(tr("Open file")))
    , m_processFileButton(new QPushButton(tr("Process")))
    , m_applySettingsButton(new QPushButton(tr("Apply")))
    , m_consoleClearButton(new QPushButton(tr("Clear")))
    , m_exportSubtitleButton(new QPushButton(tr("Export subtitle")))
    , m_exportPictureCollectionButton(new QPushButton(tr("Create visual novel")))
    , m_exportVideoRemuxButton(new QPushButton(tr("Video")))
    , m_exportAudioRemuxButton(new QPushButton(tr("Audio")))
    , m_exportSubtitleRemuxButton(new QPushButton(tr("Subtitle")))
    , m_exportDialogueButton(new QPushButton(tr("Export Dialogue")))
    , m_subTextEdit(new QTextEdit())
    , m_waveformWidget(new WaveformWidget())
{
    this->setMinimumSize(900, 720);

    m_subComboBox->setDisabled(true);
    m_subLayerSpinBox->setDisabled(true);
    m_audioComboBox->setDisabled(true);

    m_processFileButton->setDisabled(true);

    m_exportVideoRemuxButton->setDisabled(true);
    m_exportAudioRemuxButton->setDisabled(true);
    m_exportSubtitleRemuxButton->setDisabled(true);

    m_exportPictureCollectionButton->setDisabled(true);

    m_exportDialogueButton->setDisabled(true);

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
    m_subPaddingLeftSpinBox->setMinimum(INT_MIN);
    m_subPaddingLeftSpinBox->setMaximum(INT_MAX);
    m_subPaddingLeftSpinBox->setMinimumWidth(60);
    settingsPaddingLayout->addWidget(m_subPaddingLeftSpinBox);

    settingsPaddingLayout->addWidget(new QLabel(tr("Padding right (ms)")));
    m_subPaddingRightSpinBox->setSingleStep(100);
    m_subPaddingRightSpinBox->setMinimum(INT_MIN);
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
    buttonLayout->addWidget(m_processFileButton);

    infoLayout->addWidget(buttonWidget, 1); // 1/6 space of infoLayout

    // Subtitle text edit
    m_subTextEdit->setReadOnly(true);
    m_subTextEdit->insertPlainText(tr("<no subtitle loaded>"));
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


    // Export Remux selection
    QHBoxLayout* exportRemuxLayout = new QHBoxLayout();
    m_exportRemuxGroupBox->setLayout(exportRemuxLayout);
    exportRemuxLayout->addWidget(m_exportVideoRemuxButton);
    exportRemuxLayout->addWidget(m_exportAudioRemuxButton);
    exportRemuxLayout->addWidget(m_exportSubtitleRemuxButton);

    // Export Dialogue
    QHBoxLayout* exportDialogueLayout = new QHBoxLayout();
    m_exportDialogueGroupBox->setLayout(exportDialogueLayout);

    QWidget* exportDialogueRadioButtonContainer = new QWidget();
    QVBoxLayout* exportDialogueRadioButtonLayout = new QVBoxLayout();
    exportDialogueRadioButtonContainer->setLayout(exportDialogueRadioButtonLayout);
    exportDialogueRadioButtonLayout->addWidget(m_exportLossyRadioButton);
    exportDialogueRadioButtonLayout->addWidget(m_exportLosslessRadioButton);

    QWidget* exportDialogueLabelContainer = new QWidget();
    QVBoxLayout* exportDialogueLabelLayout = new QVBoxLayout();
    exportDialogueLabelContainer->setLayout(exportDialogueLabelLayout);
    exportDialogueLabelLayout->addWidget(m_exportContainerLabel);
    exportDialogueLabelLayout->addWidget(m_exportOptionsLabel);

    QWidget* exportDialogueComboBoxContainer = new QWidget();
    QVBoxLayout* exportDialogueComboBoxLayout = new QVBoxLayout();
    exportDialogueComboBoxContainer->setLayout(exportDialogueComboBoxLayout);
    exportDialogueComboBoxLayout->addWidget(m_exportContainerComboBox);
    exportDialogueComboBoxLayout->addWidget(m_exportOptionsComboBox);

    exportDialogueLayout->addWidget(exportDialogueRadioButtonContainer);
    exportDialogueLayout->addWidget(exportDialogueLabelContainer);
    exportDialogueLayout->addWidget(exportDialogueComboBoxContainer);
    exportDialogueLayout->addWidget(m_exportDialogueButton);

    // Export methods container
    QWidget* exportMethodsContainer = new QWidget();
    QHBoxLayout* exportMethodsLayout = new QHBoxLayout();

    exportMethodsContainer->setLayout(exportMethodsLayout);

    exportMethodsLayout->addWidget(m_exportRemuxGroupBox);
    exportMethodsLayout->addWidget(m_exportDialogueGroupBox);


    // Settings - options
    QWidget* optionsContainer = new QWidget();
    QVBoxLayout* optionsLayout = new QVBoxLayout();

    optionsContainer->setLayout(optionsLayout);

    optionsLayout->setSpacing(20);
    optionsLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    optionsLayout->addWidget(new QLabel(tr("Language")));
    optionsLayout->addWidget(m_languageComboBox);
    optionsLayout->addWidget(new QLabel(tr("UI mode")));
    optionsLayout->addWidget(m_lightUiRadioButton);
    optionsLayout->addWidget(m_darkUiRadioButton);

    // Settings vertical line
    QFrame* vLine = new QFrame();
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFrameShadow(QFrame::Raised);

    // Settings - logo
    QWidget* logoContainer = new QWidget();
    QVBoxLayout* logoLayout = new QVBoxLayout();

    logoContainer->setLayout(logoLayout);

    QPixmap logoPixmap{":/logo.png"};

    QLabel* logoLabel = new QLabel();
    logoLabel->setPixmap(logoPixmap.scaled(QSize(280, 280), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QString logoString = QString{"<i>%1 ver. %2</i>"}.arg(QGuiApplication::applicationDisplayName(),
                                                          QGuiApplication::applicationVersion());

    logoLayout->setSpacing(0);
    logoLayout->setAlignment(Qt::AlignCenter);

    logoLayout->addWidget(logoLabel);
    logoLayout->addWidget(new QLabel(logoString), 0, Qt::AlignHCenter);


    // Tabs
    QTabWidget* tabWidget = new QTabWidget();

    QWidget* fileContainer = new QWidget();
    QWidget* subtitleContainer = new QWidget();
    QWidget* exportContainer = new QWidget();
    QWidget* settingsContainer = new QWidget();

    QVBoxLayout* fileContainerLayout = new QVBoxLayout();
    QVBoxLayout* subtitleContainerLayout = new QVBoxLayout();
    QVBoxLayout* exportContainerLayout = new QVBoxLayout();
    QHBoxLayout* settingsContainerLayout = new QHBoxLayout();

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
    QChartView* chartView = new QChartView(m_waveformWidget);
    chartView->setRenderHint(QPainter::Antialiasing);
    exportContainerLayout->addWidget(chartView);
    exportContainerLayout->addWidget(exportMethodsContainer);

    // Tabs: Settings tab
    //settingsContainerLayout->setAlignment(Qt::AlignCenter);

    settingsContainerLayout->addWidget(optionsContainer, 10);
    settingsContainerLayout->addWidget(vLine, 1);
    settingsContainerLayout->addWidget(logoContainer, 10);

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

    connect(m_processFileButton,
            &QPushButton::pressed,
            this,
            &Window::processFileSignal);

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
            [this](){
                emit Window::exportSubtitleSignal(static_cast<const QTextEdit*>(m_subTextEdit));
            });

    connect(m_exportPictureCollectionButton,
            &QPushButton::pressed,
            this,
            [this](){
                emit Window::exportPictureCollectionSignal(static_cast<const QTextEdit*>(m_subTextEdit));
            });

    connect(m_exportDialogueButton,
            &QPushButton::pressed,
            this,
            [this](){
                emit Window::exportDialogueSignal(m_exportOptionsComboBox->currentData().value<FormatOptions::Option>());
            });

    connect(m_exportVideoRemuxButton,
            &QPushButton::pressed,
            this,
            &Window::exportVideoRemuxSignal);

    connect(m_exportAudioRemuxButton,
            &QPushButton::pressed,
            this,
            &Window::exportAudioRemuxSignal);

    connect(m_exportSubtitleRemuxButton,
            &QPushButton::pressed,
            this,
            &Window::exportSubtitleRemuxSignal);

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
                m_waveformWidget->setTheme(QChart::ChartThemeLight);
            });

    connect(m_darkUiRadioButton,
            &QRadioButton::clicked,
            this,
            [this]()
            {
                emit Window::colorSchemeSettingsChangedSignal(true);
                m_waveformWidget->setTheme(QChart::ChartThemeDark);
            });

    connect(m_exportLossyRadioButton,
            &QRadioButton::clicked,
            this,
            [this]()
            {
                this->fillDialogueExportContainers();
            });

    connect(m_exportLosslessRadioButton,
            &QRadioButton::clicked,
            this,
            [this]()
            {
                this->fillDialogueExportContainers();
            });

    connect(m_exportContainerComboBox,
            &QComboBox::currentTextChanged,
            this,
            [this]()
            {
                this->fillDialogueExportOptions();
            });

    m_exportLossyRadioButton->click();
}

void Window::fileChangedHandler(int videoStream,
                                const QList<DialogueFromVideo::AudioInfo*>& audioStreams,
                                const QList<DialogueFromVideo::SubInfo*>& subStreams)
{
    emit m_windowMessenger.print("File changed, updating file info...",
                                 "MainWindow",
                                 MessageLevel::Debug);

    // Clear dialogue
    emit dialogueClearSignal();

    // Clear subtitles
    m_subTextEdit->clear();
    m_subTextEdit->insertPlainText(tr("<no subtitle loaded>"));
    emit subtitleClearSignal();

    // Clear audio widget
    m_waveformWidget->reset();

    // Clear audio
    emit audioClearSignal();

    // Clear stream information
    m_subDescriptionLabel->clear();
    m_audioDescriptionLabel->clear();

    // Clear stream selection
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

    m_processFileButton->setDisabled(false);

    m_exportVideoRemuxButton->setDisabled(videoStream == 0);
    m_exportAudioRemuxButton->setDisabled(m_audioComboBox->count() < 1);
    m_exportSubtitleRemuxButton->setDisabled(m_subComboBox->count() < 1);

    m_exportPictureCollectionButton->setDisabled(videoStream == 0);

    m_exportDialogueButton->setDisabled(true); // enabled only after processing done

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
    emit applySettingsSignal(static_cast<int64_t>(m_subPaddingLeftSpinBox->value()),
                             static_cast<int64_t>(m_subPaddingRightSpinBox->value()),
                             static_cast<int64_t>(m_subOffsetSpinBox->value()),
                             static_cast<int64_t>(m_subMergeSpinBox->value()));
}

void Window::initialSettingsHandler(int64_t usPaddingLeft,
                                    int64_t usPaddingRight,
                                    int64_t usOffset,
                                    int64_t usMerge)
{
    m_subPaddingLeftSpinBox->setValue(static_cast<int>(usPaddingLeft));
    m_subPaddingRightSpinBox->setValue(static_cast<int>(usPaddingRight));
    m_subOffsetSpinBox->setValue(static_cast<int>(usOffset));
    m_subMergeSpinBox->setValue(static_cast<int>(usMerge));
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
        m_waveformWidget->setTheme(QChart::ChartThemeLight);
    }
    else
    {
        m_darkUiRadioButton->click();
        m_waveformWidget->setTheme(QChart::ChartThemeDark);
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
                                        "Channels: %5\n"
                                        "Lossless: %6\n"
                                        "Language: %7\n"
                                        "Codec: %8")
                                         .arg(QString::number(audioInfo.index),
                                              QString::number(audioInfo.samplerate),
                                              audioInfo.bitdepth != 0
                                                ? QString::number(audioInfo.bitdepth)
                                                : QString("N/A"),
                                              audioInfo.bitrate != 0
                                                ? QString::number(audioInfo.bitrate)
                                                : QString("N/A"),
                                              QString::number(audioInfo.channels),
                                              audioInfo.lossless ? tr("Yes") : tr("No"),
                                              audioInfo.lang != "und"
                                                ? audioInfo.lang
                                                : QString("unknown"),
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
        m_subTextEdit->insertPlainText(tr("<no subtitle loaded>"));
        return;
    }

    for (SubEntry* sub : subs)
    {
        m_subTextEdit->insertPlainText(QString("%1 -> %2\n%3\n")
                                           .arg(Time::millisecondsToStringTime(sub->start),
                                                Time::millisecondsToStringTime(sub->end),
                                                sub->text));
    }


    int64_t left = 0, right = 0, offset = 0, merge = 0;
    emit settingsRequestedSignal(left, right, offset, merge);

    emit processDialogueSignal(subs,
                               left,
                               right,
                               offset,
                               merge);
}

void Window::waveformReadyHandler(const QVector<double>& samples)
{
    int sampleRate = 0;
    emit sampleRateRequestedSignal(sampleRate);

    m_waveformWidget->initialize(samples.size(), sampleRate);

    m_waveformWidget->plotWaveform(samples);
}

void Window::readyDialogueHandler(const QList<Interval>& dialogue,
                                  const QList<Interval>& subtitle,
                                  const QList<Interval>& padding,
                                  const QList<Interval>& gap)
{
    m_waveformWidget->drawIntervals(dialogue,
                                    subtitle,
                                    padding,
                                    gap);
}

void Window::allowDialogueExportHandler()
{
    m_exportDialogueButton->setDisabled(false);
}

void Window::fillDialogueExportContainers()
{
    int lossy = m_exportLossyRadioButton->isChecked();

    m_exportContainerComboBox->clear();

    if (lossy)
    {
        m_exportContainerComboBox->addItems(FormatOptions::lossyOptions().keys());
    }
    else
    {
        m_exportContainerComboBox->addItems(FormatOptions::losslessOptions().keys());
    }
}

void Window::fillDialogueExportOptions()
{
    // Ignore currentTextChanged signals containing empty string
    if (!m_exportContainerComboBox || m_exportContainerComboBox->currentText().isEmpty())
    {
        return;
    }

    int lossy = m_exportLossyRadioButton->isChecked();

    QMap<QString, QList<FormatOptions::Option>> map;

    if (lossy)
    {
        map = FormatOptions::lossyOptions();
    }
    else
    {
        map = FormatOptions::losslessOptions();
    }

    QList<FormatOptions::Option> options = map[m_exportContainerComboBox->currentText()];

    m_exportOptionsLabel->setText(options[0].key);

    m_exportOptionsComboBox->clear();

    for (const FormatOptions::Option& option : options)
    {
        m_exportOptionsComboBox->addItem(option.value, QVariant::fromValue(option));
    }
}

} // namespace DialogueFromVideo
