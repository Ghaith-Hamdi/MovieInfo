#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TableColumnManager/TableColumnManager.h"
#include "MovieDataRefresher/MovieDataRefresher.h"

#include <QAction>
#include <QComboBox>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QIntValidator>
#include <QRegularExpression>
#include <QSet>
#include <QSettings>
#include <QTextStream>
#include <QUrl>
#include <QInputDialog>
#include <QLabel>
#include <QCheckBox>
#include <windows.h> // For GetLastError()
#include <QThread>
#include <QToolBar>
#include <QDragEnterEvent>
#include <QIcon>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProgressDialog>
#include <QTimer>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

class NumericTableWidgetItem : public QTableWidgetItem
{
public:
    NumericTableWidgetItem(const QString &text) : QTableWidgetItem(text) {}

    bool operator<(const QTableWidgetItem &other) const override
    {
        // Remove commas and convert to double for comparison
        QString thisText = text().remove(',');
        QString otherText = other.text().remove(',');

        bool ok1, ok2;
        double thisNum = thisText.toDouble(&ok1);
        double otherNum = otherText.toDouble(&ok2);

        if (ok1 && ok2)
            return thisNum < otherNum;

        return QTableWidgetItem::operator<(other);
    }
};

// ========================================================================
// CONSTRUCTOR / DESTRUCTOR
// ========================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), progressDialog(nullptr),
      totalMoviesToFetch(0), moviesFetched(0)
{
    ui->setupUi(this);

    // Set window icon and improve window appearance
    setWindowIcon(QIcon("icons/app_icon.png"));
    setWindowTitle("MovieInfo - Video Library Manager");

    // Add a toolbar if it doesn't exist
    if (!ui->toolBar)
    {
        ui->toolBar = new QToolBar(this);
        this->addToolBar(ui->toolBar);
    }

    // Initialize database
    movieDb = new MovieDB(this);
    if (!movieDb->init())
    {
        QMessageBox::warning(this, "Database Error", "Failed to initialize movie database");
    }

    // Initialize OMDb client
    omdbClient = new OmdbClient("10f95a16", movieDb, this);

    // Initialize helper classes
    columnManager = new TableColumnManager(ui->tableWidget, ui->toolBar, this);
    dataRefresher = new MovieDataRefresher(movieDb, omdbClient, ui->statusbar, this);

    // Setup table with improved styling
    ui->tableWidget->setColumnCount(22);
    ui->tableWidget->setHorizontalHeaderLabels({"Title", "Year", "Decade", "Resolution", "Aspect Ratio", "Quality", "Size", "Duration", "Language",
                                                "Actions", "Rated", "Rating", "Votes", "Director", "Actors", "Writers", "Awards", "Language", "Country", "Box Office", "Plot", "Genre"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Enhanced table styling
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setGridStyle(Qt::SolidLine);

    // Enhanced header styling
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Setup column management
    columnManager->setupColumnVisibilityMenu();
    columnManager->setupColumnReorderingMenu();
    columnManager->loadSettings();

    // Add drive & year selection controls to toolbar
    ui->toolBar->addSeparator();

    QLabel *driveLabel = new QLabel(" Drive: ", this);
    driveLabel->setStyleSheet("QLabel { color: #c9d1d9; font-weight: 600; }");
    ui->toolBar->addWidget(driveLabel);

    driveComboBox = new QComboBox(this);
    driveComboBox->addItems({"D:\\", "E:\\"});
    driveComboBox->setMinimumWidth(80);
    driveComboBox->setToolTip("Select drive where movies are stored");
    ui->toolBar->addWidget(driveComboBox);

    QLabel *yearLabel = new QLabel(" Year: ", this);
    yearLabel->setStyleSheet("QLabel { color: #c9d1d9; font-weight: 600; }");
    ui->toolBar->addWidget(yearLabel);

    yearLineEdit = new QLineEdit(this);
    yearLineEdit->setPlaceholderText("e.g., 2023");
    yearLineEdit->setMaxLength(4);
    yearLineEdit->setMinimumWidth(100);
    yearLineEdit->setToolTip("Enter year (e.g., 2023, 1994)");
    ui->toolBar->addWidget(yearLineEdit);

    fetchByDriveYearButton = new QPushButton("📁 Fetch", this);
    fetchByDriveYearButton->setToolTip("Fetch movies from selected drive and year");
    fetchByDriveYearButton->setMinimumWidth(100);
    ui->toolBar->addWidget(fetchByDriveYearButton);

    ui->toolBar->addSeparator();

    showMoviesToMoveButton = new QPushButton("🔄 Show Movies to Move", this);
    showMoviesToMoveButton->setToolTip("Show movies that need to be moved to the correct drive");
    showMoviesToMoveButton->setMinimumWidth(150);
    showMoviesToMoveButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #a371f7, stop:1 #8957e5);"
        "    color: white;"
        "    border: 1px solid #b392f0;"
        "    border-radius: 6px;"
        "    padding: 6px 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #b392f0, stop:1 #a371f7);"
        "}"
        "QPushButton:pressed {"
        "    background: #8957e5;"
        "}");
    ui->toolBar->addWidget(showMoviesToMoveButton);

    ui->toolBar->addSeparator();

    QPushButton *settingsButton = new QPushButton("⚙️ Settings", this);
    settingsButton->setToolTip("Open application settings");
    settingsButton->setMinimumWidth(100);
    settingsButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #30363d;"
        "    color: #c9d1d9;"
        "    border: 1px solid #484f58;"
        "    border-radius: 6px;"
        "    padding: 6px 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #484f58;"
        "    border-color: #6e7681;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #21262d;"
        "}");
    ui->toolBar->addWidget(settingsButton);

    ui->toolBar->addSeparator();

    QPushButton *clearTableButton = new QPushButton("🗑️ Clear Table", this);
    clearTableButton->setToolTip("Clear all movies from the table");
    clearTableButton->setMinimumWidth(110);
    clearTableButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #8b0000;"
        "    color: white;"
        "    border: 1px solid #b30000;"
        "    border-radius: 6px;"
        "    padding: 6px 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #a50000;"
        "    border-color: #cc0000;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #6b0000;"
        "}");
    ui->toolBar->addWidget(clearTableButton);

    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(ui->selectFolderButton, &QPushButton::clicked, this, &MainWindow::onSelectFolderClicked);

    // Initialize combo boxes with "All"
    addComboBoxItemIfNotExist(ui->comboBoxDecade, "All");
    addComboBoxItemIfNotExist(ui->comboBoxAspectRatio, "All");
    addComboBoxItemIfNotExist(ui->comboBoxQuality, "All");

    setAcceptDrops(true);

    // Connect combo boxes to filterTable
    connect(ui->comboBoxDecade, &QComboBox::currentTextChanged, this, &MainWindow::filterTable);
    connect(ui->comboBoxAspectRatio, &QComboBox::currentTextChanged, this, &MainWindow::filterTable);
    connect(ui->comboBoxQuality, &QComboBox::currentTextChanged, this, &MainWindow::filterTable);

    // Other signal connections
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportToExcel);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterTableRows);
    connect(omdbClient, &OmdbClient::movieFetched, this, &MainWindow::onMovieFetched);
    connect(omdbClient, &OmdbClient::movieFetchedFromDatabase, this, &MainWindow::onMovieFetchedFromDatabase);
    connect(omdbClient, &OmdbClient::movieExistsInDatabase, this, &MainWindow::onMovieExistsInDatabase);
    connect(ui->fetchButton, &QPushButton::clicked, this, &MainWindow::onFetchClicked);
    connect(fetchByDriveYearButton, &QPushButton::clicked, this, &MainWindow::onFetchByDriveYearClicked);
    connect(showMoviesToMoveButton, &QPushButton::clicked, this, &MainWindow::onShowMoviesToMoveClicked);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);
    connect(clearTableButton, &QPushButton::clicked, this, &MainWindow::onClearTableClicked);

    // Add tooltips for better user experience
    ui->searchLineEdit->setToolTip("Search through all movie titles, directors, actors, and other fields");
    ui->comboBoxQuality->setToolTip("Filter movies by video quality (4K, 1080p, 720p, etc.)");
    ui->comboBoxDecade->setToolTip("Filter movies by release decade");
    ui->comboBoxAspectRatio->setToolTip("Filter movies by aspect ratio");
    ui->selectFolderButton->setToolTip("Select a folder containing video files to add to your library");
    ui->fetchButton->setToolTip("Fetch additional movie information from IMDb for selected movies");
    ui->exportButton->setToolTip("Export the current movie library to an Excel file");

    // Add status bar message
    ui->statusbar->showMessage("Ready to process videos. Drag and drop files or use 'Select Folder' button.");

    // Load external stylesheet for additional styling
    loadExternalStylesheet();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSelectFolderClicked()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    QString lastFolder = settings.value("lastFolder").toString();
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Video Folder", lastFolder);
    if (!folderPath.isEmpty())
    {
        settings.setValue("lastFolder", folderPath);
        ui->statusbar->showMessage("Processing videos from: " + folderPath);
        processVideos(folderPath, false); // Process as folder
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
        ui->statusbar->showMessage("Drop files or folders to add them to the library");
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    int processedFiles = 0;
    int processedFolders = 0;

    for (const QUrl &url : urls)
    {
        QString localPath = url.toLocalFile();
        QFileInfo info(localPath);

        if (info.isFile())
        {
            QString ext = info.suffix().toLower();
            if (ext == "mp4" || ext == "mkv" || ext == "avi" || ext == "mov" || ext == "flv" || ext == "wmv")
            {
                processVideos(localPath, true); // Process single video file
                processedFiles++;
            }
        }
        else if (info.isDir())
        {
            processVideos(localPath, false); // Process entire folder recursively
            processedFolders++;
        }
    }

    if (processedFiles > 0 || processedFolders > 0)
    {
        ui->statusbar->showMessage(QString("Added %1 files and %2 folders to library")
                                       .arg(processedFiles)
                                       .arg(processedFolders),
                                   3000);
    }
}

// ========================================================================
// VIDEO PROCESSING
// ========================================================================

void MainWindow::processVideos(const QStringList &filePaths)
{
    for (const QString &filePath : filePaths)
    {
        processVideos(filePath, true);
    }
}

QPushButton *MainWindow::createActionButton(const QString &text, const QString &iconPath, const QString &styleSheet, const QString &objectName)
{
    QPushButton *button = new QPushButton(text);

    // Try to load custom icon, fallback to Qt standard icon
    QIcon icon(iconPath);
    if (icon.isNull())
    {
        if (text == "Open")
            icon = style()->standardIcon(QStyle::SP_MediaPlay);
        else
            icon = style()->standardIcon(QStyle::SP_ComputerIcon);
    }
    button->setIcon(icon);
    button->setIconSize(QSize(16, 16));
    button->setStyleSheet(styleSheet);

    if (!objectName.isEmpty())
        button->setObjectName(objectName);

    return button;
}

QWidget *MainWindow::createActionButtonsWidget(const QString &filePath, const QString &title, const QString &year)
{
    const QString openButtonStyle =
        "QPushButton {"
        "    background-color: #2e7d32;"
        "    color: white;"
        "    border: none;"
        "    padding: 6px 12px;"
        "    border-radius: 4px;"
        "    font-size: 11px;"
        "    font-weight: bold;"
        "    min-width: 60px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #388e3c;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1b5e20;"
        "}";

    const QString imdbButtonStyle =
        "QPushButton {"
        "    background-color: #f57c00;"
        "    color: white;"
        "    border: none;"
        "    padding: 6px 12px;"
        "    border-radius: 4px;"
        "    font-size: 11px;"
        "    font-weight: bold;"
        "    min-width: 60px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #ff8f00;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #e65100;"
        "}";

    const QString paheButtonStyle =
        "QPushButton {"
        "    background-color: #1976d2;"
        "    color: white;"
        "    border: none;"
        "    padding: 6px 12px;"
        "    border-radius: 4px;"
        "    font-size: 11px;"
        "    font-weight: bold;"
        "    min-width: 60px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1565c0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0d47a1;"
        "}";

    QPushButton *openButton = createActionButton("Open", "icons/open.png", openButtonStyle);
    openButton->setProperty("filePath", filePath);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::onOpenFileClicked);

    QPushButton *imdbButton = createActionButton("IMDb", "icons/imdb.png", imdbButtonStyle, "imdbButton");
    imdbButton->setToolTip("Search on IMDb");
    imdbButton->setProperty("title", title);
    imdbButton->setProperty("year", year);
    connect(imdbButton, &QPushButton::clicked, this, &MainWindow::onImdbButtonClicked);

    QPushButton *paheButton = createActionButton("Pahe", "icons/pahe.png", paheButtonStyle);
    paheButton->setProperty("title", title);
    paheButton->setProperty("year", year);
    connect(paheButton, &QPushButton::clicked, this, &MainWindow::onPaheButtonClicked);

    QWidget *buttonsWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(buttonsWidget);
    layout->addWidget(openButton);
    layout->addWidget(imdbButton);
    layout->addWidget(paheButton);
    layout->setContentsMargins(4, 2, 4, 2);
    layout->setSpacing(4);

    return buttonsWidget;
}

void MainWindow::processVideos(const QString &path, bool isSingleFile)
{
    QStringList videoExtensions = {".mp4", ".mkv", ".avi", ".mov", ".flv", ".wmv"};
    QSet<QString> decades, aspectRatios, qualities, raitings;
    QStringList filesToProcess;

    if (isSingleFile)
    {
        QFileInfo fileInfo(path);
        if (videoExtensions.contains("." + fileInfo.suffix().toLower()))
            filesToProcess.append(path);
    }
    else
    {
        QDirIterator it(path, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString filePath = it.next();
            QFileInfo fileInfo(filePath);
            if (videoExtensions.contains("." + fileInfo.suffix().toLower()))
                filesToProcess.append(filePath);
        }
    }

    // Show progress dialog for processing
    if (!filesToProcess.isEmpty())
    {
        progressDialog = new QProgressDialog("Processing video files...", "Cancel", 0, filesToProcess.size(), this);
        progressDialog->setWindowModality(Qt::WindowModal);
        progressDialog->setMinimumDuration(500);
        progressDialog->setValue(0);
    }

    int processedCount = 0;
    for (const QString &filePath : filesToProcess)
    {
        // Check if user cancelled
        if (progressDialog && progressDialog->wasCanceled())
        {
            ui->statusbar->showMessage("Processing cancelled by user.", 3000);
            break;
        }

        // Use batch metadata extraction for better performance
        VideoMetadata metadata = getVideoMetadataBatch(filePath);

        QString folderName = QFileInfo(filePath).dir().dirName();
        QPair<QString, QString> parsed = parseFolderName(folderName);
        QString title = parsed.first;
        QString year = parsed.second;
        QString decade = getDecade(year);

        decades.insert(decade);
        aspectRatios.insert(metadata.aspectRatio);
        qualities.insert(metadata.quality);

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setData(FilePathRole, filePath);
        ui->tableWidget->setItem(row, 0, titleItem);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(year));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(decade));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(metadata.resolution));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(metadata.aspectRatio));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(metadata.quality));
        ui->tableWidget->setItem(row, 6, new NumericTableWidgetItem(metadata.fileSize));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(metadata.duration));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(metadata.audioLanguage));

        // Create action buttons widget
        QWidget *buttonsWidget = createActionButtonsWidget(filePath, title, year);
        ui->tableWidget->setCellWidget(row, 9, buttonsWidget);

        // Update progress
        processedCount++;
        if (progressDialog)
        {
            progressDialog->setValue(processedCount);
            progressDialog->setLabelText(QString("Processing video %1 of %2...").arg(processedCount).arg(filesToProcess.size()));
        }
    }

    // Clean up progress dialog
    if (progressDialog)
    {
        progressDialog->close();
        delete progressDialog;
        progressDialog = nullptr;
    }

    addComboBoxItemsSorted(ui->comboBoxDecade, decades);
    addComboBoxItemsSorted(ui->comboBoxAspectRatio, aspectRatios, "UltraWide");
    addComboBoxItemsSorted(ui->comboBoxQuality, qualities);

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    // Update status bar with processing results
    int totalFiles = filesToProcess.size();
    ui->statusbar->showMessage(QString("Processed %1 video files. Total movies in library: %2")
                                   .arg(totalFiles)
                                   .arg(ui->tableWidget->rowCount()),
                               5000);
}

// ========================================================================
// VIDEO METADATA METHODS
// ========================================================================

QString MainWindow::runFfprobe(const QStringList &args)
{
    QProcess ffprobe;
    ffprobe.start("ffprobe", args);
    ffprobe.waitForFinished();
    return ffprobe.readAllStandardOutput().trimmed();
}

MainWindow::VideoMetadata MainWindow::getVideoMetadataBatch(const QString &filePath)
{
    VideoMetadata metadata;

    // Run ffprobe once to get all metadata in JSON format
    QProcess ffprobe;
    QStringList args;
    args << "-v" << "quiet"
         << "-print_format" << "json"
         << "-show_format"
         << "-show_streams"
         << filePath;

    ffprobe.start("ffprobe", args);
    ffprobe.waitForFinished();

    QByteArray output = ffprobe.readAllStandardOutput();
    QJsonDocument doc = QJsonDocument::fromJson(output);

    if (doc.isNull() || !doc.isObject())
    {
        // Fallback to unknown values
        metadata.resolution = "Unknown";
        metadata.aspectRatio = "Unknown";
        metadata.quality = "Unknown";
        metadata.duration = "Unknown";
        metadata.audioLanguage = "Unknown";
        metadata.fileSize = "0.00 GB";
        return metadata;
    }

    QJsonObject root = doc.object();

    // Extract resolution from video stream
    if (root.contains("streams"))
    {
        QJsonArray streams = root["streams"].toArray();

        // Find video stream
        for (const QJsonValue &streamVal : streams)
        {
            QJsonObject stream = streamVal.toObject();
            if (stream["codec_type"].toString() == "video")
            {
                int width = stream["width"].toInt(0);
                int height = stream["height"].toInt(0);

                if (width > 0 && height > 0)
                {
                    metadata.resolution = QString("%1x%2").arg(width).arg(height);

                    // Calculate aspect ratio
                    double ratio = static_cast<double>(width) / height;
                    metadata.aspectRatio = QString::number(ratio, 'f', 2);
                }
                break;
            }
        }

        // Find audio stream for language
        for (const QJsonValue &streamVal : streams)
        {
            QJsonObject stream = streamVal.toObject();
            if (stream["codec_type"].toString() == "audio")
            {
                if (stream.contains("tags"))
                {
                    QJsonObject tags = stream["tags"].toObject();
                    if (tags.contains("language"))
                    {
                        metadata.audioLanguage = tags["language"].toString();
                    }
                }
                break;
            }
        }
    }

    // Extract duration from format
    if (root.contains("format"))
    {
        QJsonObject format = root["format"].toObject();
        if (format.contains("duration"))
        {
            bool ok;
            int seconds = format["duration"].toString().toDouble(&ok);
            if (ok)
            {
                int h = seconds / 3600;
                int m = (seconds % 3600) / 60;
                int s = seconds % 60;
                metadata.duration = QString("%1:%2:%3")
                                        .arg(h, 2, 10, QChar('0'))
                                        .arg(m, 2, 10, QChar('0'))
                                        .arg(s, 2, 10, QChar('0'));
            }
        }
    }

    // Extract quality from filename (same logic as before)
    QString fileName = QFileInfo(filePath).fileName().toLower();
    if (fileName.contains("2160p") || fileName.contains("4k"))
        metadata.quality = "4K";
    else if (fileName.contains("1080p"))
        metadata.quality = "1080p";
    else if (fileName.contains("720p"))
        metadata.quality = "720p";
    else
        metadata.quality = "Unknown";

    // Calculate file size
    QFileInfo info(filePath);
    double sizeInGB = info.size() / (1024.0 * 1024.0 * 1024.0);
    metadata.fileSize = QString::number(sizeInGB, 'f', 2) + " GB";

    // Set defaults for any missing values
    if (metadata.resolution.isEmpty())
        metadata.resolution = "Unknown";
    if (metadata.aspectRatio.isEmpty())
        metadata.aspectRatio = "Unknown";
    if (metadata.duration.isEmpty())
        metadata.duration = "Unknown";
    if (metadata.audioLanguage.isEmpty())
        metadata.audioLanguage = "Unknown";

    return metadata;
}

QString MainWindow::getVideoResolution(const QString &filePath)
{
    QString output = runFfprobe({"-v", "error", "-select_streams", "v:0", "-show_entries", "stream=width,height", "-of", "csv=p=0", filePath});
    return output.isEmpty() ? "Unknown" : output.replace(",", "x");
}

QString MainWindow::getAspectRatio(const QString &resolution)
{
    if (resolution == "Unknown")
        return "Unknown";

    QStringList parts = resolution.split("x");
    if (parts.size() != 2)
        return "Unknown";

    bool ok1, ok2;
    int width = parts[0].toInt(&ok1);
    int height = parts[1].toInt(&ok2);
    if (!ok1 || !ok2 || height == 0)
        return "Unknown";

    double ratio = static_cast<double>(width) / height;
    return QString::number(ratio, 'f', 2);
}

QString MainWindow::getVideoQuality(const QString &filePath)
{
    QString fileName = QFileInfo(filePath).fileName().toLower();
    if (fileName.contains("2160p") || fileName.contains("4k"))
        return "4K";
    if (fileName.contains("1080p"))
        return "1080p";
    if (fileName.contains("720p"))
        return "720p";
    return "Unknown";
}

QString MainWindow::getFileSize(const QString &filePath)
{
    QFileInfo info(filePath);
    double sizeInGB = info.size() / (1024.0 * 1024.0 * 1024.0);
    return QString::number(sizeInGB, 'f', 2) + " GB";
}

QString MainWindow::getVideoDuration(const QString &filePath)
{
    bool ok;
    int seconds = runFfprobe({"-v", "error", "-select_streams", "v:0", "-show_entries", "format=duration", "-of", "default=noprint_wrappers=1:nokey=1", filePath}).toDouble(&ok);
    if (!ok)
        return "Unknown";
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    return QString("%1:%2:%3").arg(h, 2, 10, QChar('0')).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
}

QString MainWindow::getAudioLanguage(const QString &filePath)
{
    QString lang = runFfprobe({"-v", "error", "-select_streams", "a:0", "-show_entries", "stream_tags=language", "-of", "default=noprint_wrappers=1:nokey=1", filePath});
    return lang.isEmpty() ? "Unknown" : lang;
}

QPair<QString, QString> MainWindow::parseFolderName(const QString &folderName)
{
    QRegularExpression re("(.+?) \\((\\d{4})\\)");
    QRegularExpressionMatch match = re.match(folderName);
    if (match.hasMatch())
        return {match.captured(1).trimmed(), match.captured(2)};
    return {folderName, "Unknown"};
}

QString MainWindow::getDecade(const QString &year)
{
    bool ok;
    int y = year.toInt(&ok);
    return ok ? QString::number(y - (y % 10)) + "s" : "Unknown";
}

// ========================================================================
// TABLE FILTERING & SEARCH
// ========================================================================

void MainWindow::filterTable()
{
    QString selectedDecade = ui->comboBoxDecade->currentText();
    QString selectedAspectRatio = ui->comboBoxAspectRatio->currentText();
    QString selectedQuality = ui->comboBoxQuality->currentText();

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        bool matchDecade = (selectedDecade == "All" || ui->tableWidget->item(row, 2)->text() == selectedDecade);
        bool matchAspectRatio = (selectedAspectRatio == "All" ||
                                 (selectedAspectRatio == "UltraWide" &&
                                  ui->tableWidget->item(row, 4)->text().toDouble() >= 2.2 &&
                                  ui->tableWidget->item(row, 4)->text().toDouble() <= 2.5) ||
                                 ui->tableWidget->item(row, 4)->text() == selectedAspectRatio);
        bool matchQuality = (selectedQuality == "All" || ui->tableWidget->item(row, 5)->text() == selectedQuality);

        ui->tableWidget->setRowHidden(row, !(matchDecade && matchAspectRatio && matchQuality));
    }
}

void MainWindow::filterTableRows(const QString &text)
{
    QString searchText = text.trimmed().toLower();

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        bool match = false;

        for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
        {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item && item->text().toLower().contains(searchText))
            {
                match = true;
                break;
            }
        }

        ui->tableWidget->setRowHidden(row, !match);
    }
}

// ========================================================================
// CONTEXT MENU ACTIONS
// ========================================================================

std::optional<QFileInfo> MainWindow::getFileInfoForRow(int row)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, 0);
    if (!item)
        return std::nullopt;

    QString filePath = item->data(FilePathRole).toString();
    if (filePath.isEmpty())
        return std::nullopt;

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists())
        return std::nullopt;

    return fileInfo;
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->tableWidget->indexAt(pos);
    if (!index.isValid())
        return;

    contextMenuRow = index.row();

    QMenu menu(this);
    QAction *refreshMovieAction = menu.addAction("Refresh Movie Data from IMDb");
    menu.addSeparator();
    QAction *openFolderAction = menu.addAction("Open Containing Folder");
    QAction *renameFolderAction = menu.addAction("Rename Folder");
    QAction *moveFolderAction = menu.addAction("Move to Archive Folder");

    connect(refreshMovieAction, &QAction::triggered, this, &MainWindow::onRefreshMovieClicked);
    connect(openFolderAction, &QAction::triggered, this, &MainWindow::onOpenFolderClicked);
    connect(renameFolderAction, &QAction::triggered, this, &MainWindow::onRenameFolderClicked);
    connect(moveFolderAction, &QAction::triggered, this, &MainWindow::onMoveFolderToArchiveClicked);

    menu.exec(ui->tableWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::onOpenFolderClicked()
{
    std::optional<QFileInfo> fileInfoOpt = getFileInfoForRow(contextMenuRow);
    if (!fileInfoOpt)
    {
        QMessageBox::warning(this, "Error", "Could not determine file path.");
        return;
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfoOpt->absolutePath()));
}

void MainWindow::onRenameFolderClicked()
{
    std::optional<QFileInfo> fileInfoOpt = getFileInfoForRow(contextMenuRow);
    if (!fileInfoOpt)
        return;

    const QFileInfo &fileInfo = *fileInfoOpt;
    QDir parentDir = fileInfo.dir();
    QString currentPath = parentDir.absolutePath();

    // Read existing movie title/year from table, fall back to parsing the folder name
    QString title = "";
    QString year = "";
    QTableWidgetItem *titleItem = ui->tableWidget->item(contextMenuRow, 0);
    QTableWidgetItem *yearItem = ui->tableWidget->item(contextMenuRow, 1);
    if (titleItem)
        title = titleItem->text().trimmed();
    if (yearItem)
        year = yearItem->text().trimmed();

    if (title.isEmpty() || year.isEmpty() || year == "Unknown")
    {
        // try parsing the parent folder's name (e.g., "Splitsville (2025)")
        QPair<QString, QString> parsed = parseFolderName(parentDir.dirName());
        if (title.isEmpty())
            title = parsed.first;
        if (year.isEmpty() || year == "Unknown")
            year = parsed.second;
    }

    // Build a small dialog with two fields: Movie name and Year
    QDialog dlg(this);
    dlg.setWindowTitle("Rename Folder");
    dlg.setMinimumWidth(520);
    QFormLayout form(&dlg);

    QLineEdit *nameEdit = new QLineEdit(title, &dlg);
    QLineEdit *yearEdit = new QLineEdit(year, &dlg);
    yearEdit->setValidator(new QIntValidator(1800, 3000, yearEdit));
    yearEdit->setMaxLength(4);

    form.addRow("Movie name:", nameEdit);
    form.addRow("Year:", yearEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                     Qt::Horizontal, &dlg);
    form.addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return;

    QString newTitle = nameEdit->text().trimmed();
    QString newYear = yearEdit->text().trimmed();
    if (newTitle.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Movie name cannot be empty.");
        return;
    }

    QString newName;
    if (!newYear.isEmpty())
        newName = QString("%1 (%2)").arg(newTitle, newYear);
    else
        newName = newTitle;
    newName = sanitizeForWindowsFolder(newName).replace('\n', ' ').trimmed();
    if (newName.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Invalid folder name after sanitization.");
        return;
    }

    QDir baseDir = parentDir;
    baseDir.cdUp();
    QString newPath = QDir::cleanPath(baseDir.absoluteFilePath(newName));

    if (QDir(newPath).exists())
    {
        QMessageBox::warning(this, "Error", "Target folder already exists.");
        return;
    }

    QString newPathNative = QDir::toNativeSeparators(newPath);
    QString currentPathNative = QDir::toNativeSeparators(currentPath);

    bool renamed = QDir().rename(currentPathNative, newPathNative);
    if (!renamed)
    {
        QProcess elevate;
        QStringList args = {"/c", "move",
                            QString("\"%1\"").arg(currentPathNative),
                            QString("\"%1\"").arg(newPathNative)};
        elevate.start("cmd.exe", args);
        elevate.waitForFinished();

        renamed = QDir(newPathNative).exists() && !QDir(currentPathNative).exists();
    }

    if (renamed)
    {
        QString newFilePath = newPathNative + QDir::separator() + fileInfo.fileName();
        // Update stored file path
        if (ui->tableWidget->item(contextMenuRow, 0))
            ui->tableWidget->item(contextMenuRow, 0)->setData(FilePathRole, newFilePath);

        // Update visible UI: title, year and decade
        if (!newTitle.isEmpty())
        {
            if (ui->tableWidget->item(contextMenuRow, 0))
                ui->tableWidget->item(contextMenuRow, 0)->setText(newTitle);
        }
        if (ui->tableWidget->item(contextMenuRow, 1))
            ui->tableWidget->item(contextMenuRow, 1)->setText(newYear);
        // Update decade (col 2) if year is numeric
        QString newDecade = getDecade(newYear);
        if (ui->tableWidget->item(contextMenuRow, 2))
            ui->tableWidget->item(contextMenuRow, 2)->setText(newDecade);

        // If any combo boxes depend on decades list, refresh them
        addComboBoxItemIfNotExist(ui->comboBoxDecade, newDecade);

        // After renaming, refresh movie data for this movie and update UI when OMDb returns
        if (!newTitle.isEmpty())
        {
            int fetchYear = 0;
            bool ok;
            int y = newYear.toInt(&ok);
            if (ok)
                fetchYear = y;
            ui->statusbar->showMessage(QString("Refreshing movie data for %1...").arg(newTitle), 3000);
            if (omdbClient)
                omdbClient->fetchMovie(newTitle, fetchYear);
        }

        QMessageBox::information(this, "Success", "Folder renamed successfully.");
    }
    else
    {
        DWORD error = GetLastError();
        LPWSTR messageBuffer = nullptr;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, nullptr);
        QString errorMessage = messageBuffer ? QString::fromWCharArray(messageBuffer) : QString("Unknown error");
        if (messageBuffer)
            LocalFree(messageBuffer);

        QString errorMsg = QString("Failed to rename folder.\nError code: %1 (0x%2)\n%3\n\nFrom: %4\nTo: %5")
                               .arg(error)
                               .arg(error, 8, 16, QChar('0'))
                               .arg(errorMessage.trimmed())
                               .arg(currentPathNative)
                               .arg(newPathNative);
        QMessageBox::warning(this, "Error", errorMsg);
    }
}

void MainWindow::onMoveFolderToArchiveClicked()
{
    const QString kArchiveFolderPath = "D:/New folder";

    QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
    if (ranges.isEmpty())
    {
        QMessageBox::warning(this, "No Selection", "Please select at least one movie.");
        return;
    }

    QDir archiveDir(kArchiveFolderPath);
    if (!archiveDir.exists() && !archiveDir.mkpath("."))
    {
        QMessageBox::warning(this, "Error", "Failed to create archive folder.");
        return;
    }

    int movedCount = 0;

    for (const QTableWidgetSelectionRange &range : ranges)
    {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row)
        {
            std::optional<QFileInfo> fileInfoOpt = getFileInfoForRow(row);
            if (!fileInfoOpt)
                continue;

            const QFileInfo &fileInfo = *fileInfoOpt;
            QDir currentDir = fileInfo.dir();
            QString currentPath = currentDir.absolutePath();
            QString folderName = currentDir.dirName();
            QString newPath = QDir::cleanPath(archiveDir.filePath(folderName));

            if (QDir(newPath).exists())
                continue;

            QString currentPathNative = QDir::toNativeSeparators(currentPath);
            QString newPathNative = QDir::toNativeSeparators(newPath);

            bool moved = QDir().rename(currentPathNative, newPathNative);
            if (moved)
            {
                QString newFilePath = newPathNative + QDir::separator() + fileInfo.fileName();
                ui->tableWidget->item(row, 0)->setData(FilePathRole, newFilePath);
                ++movedCount;
            }
        }
    }

    if (movedCount > 0)
    {
        QMessageBox::information(this, "Success", QString("Moved %1 folder(s) to archive.").arg(movedCount));
    }
    else
    {
        QMessageBox::warning(this, "No Folders Moved", "No folders were moved. They may already exist in the archive.");
    }
}

// ========================================================================
// ACTION BUTTONS
// ========================================================================

void MainWindow::onOpenFileClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;

    QString filePath = btn->property("filePath").toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void MainWindow::onImdbButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;

    QString title = btn->property("title").toString();
    QString year = btn->property("year").toString();

    QPoint center(btn->width() / 2, btn->height() / 2);
    QPoint viewportPos = btn->mapTo(ui->tableWidget->viewport(), center);
    QModelIndex idx = ui->tableWidget->indexAt(viewportPos);
    int clickedRow = idx.isValid() ? idx.row() : -1;

    if (clickedRow >= 0)
    {
        QTableWidgetItem *titleItemLocal = ui->tableWidget->item(clickedRow, 0);
        if (titleItemLocal)
        {
            QString imdbId = titleItemLocal->data(ImdbIdRole).toString();
            if (!imdbId.isEmpty())
            {
                QString url = QString("https://www.imdb.com/title/%1/").arg(imdbId);
                QDesktopServices::openUrl(QUrl(url));
                return;
            }
        }
    }

    openImdbPage(title, year);
}

void MainWindow::onPaheButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;

    QString title = btn->property("title").toString();
    QString year = btn->property("year").toString();
    openPahePage(title, year);
}

void MainWindow::cleanupProgressDialog()
{
    if (progressDialog)
    {
        progressDialog->close();
        delete progressDialog;
        progressDialog = nullptr;
    }

    // Show fetch summary if enabled in settings
    showFetchSummary();
}

// ========================================================================
// UTILITY METHODS
// ========================================================================

void MainWindow::addComboBoxItemIfNotExist(QComboBox *comboBox, const QString &item)
{
    if (comboBox->findText(item) == -1)
        comboBox->addItem(item);
}

void MainWindow::addComboBoxItemsSorted(QComboBox *comboBox, const QSet<QString> &items, const QString &additionalItem)
{
    QList<double> numericRatios;
    QStringList otherItems;

    for (const QString &item : items)
    {
        if (item == "Unknown")
            continue;

        bool ok;
        double value = item.toDouble(&ok);
        if (ok)
        {
            if (value >= 2.2 && value <= 2.5)
                continue; // Skip UltraWide
            numericRatios.append(value);
        }
        else
        {
            otherItems.append(item);
        }
    }

    std::sort(numericRatios.begin(), numericRatios.end());
    std::sort(otherItems.begin(), otherItems.end());

    comboBox->clear();
    addComboBoxItemIfNotExist(comboBox, "All");

    for (double val : numericRatios)
        comboBox->addItem(QString::number(val, 'f', 2));

    comboBox->addItems(otherItems);

    if (!additionalItem.isEmpty() && comboBox->findText(additionalItem) == -1)
        comboBox->addItem(additionalItem);
}

// ========================================================================
// EXTERNAL NAVIGATION
// ========================================================================

void MainWindow::openImdbPage(const QString &title, const QString &year)
{
    QString name = title + " " + year;
    QString url = "https://www.imdb.com/find/?q=" + QUrl::toPercentEncoding(name);
    QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::openPahePage(const QString &title, const QString &year)
{
    QString name = title + " " + year;
    QString url = "https://pahe.ink/?s=" + QUrl::toPercentEncoding(name);
    QDesktopServices::openUrl(QUrl(url));
}

// ========================================================================
// EXPORT
// ========================================================================

void MainWindow::exportToExcel()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as Excel", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Could not open file for writing");
        return;
    }

    QTextStream out(&file);

    // Write headers for visible columns only
    QStringList headers;
    for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
    {
        if (!ui->tableWidget->isColumnHidden(col) && col != 9) // Skip Actions column
            headers << ui->tableWidget->horizontalHeaderItem(col)->text();
    }
    out << headers.join(",") << "\n";

    // Write rows
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        if (ui->tableWidget->isRowHidden(row))
            continue;

        QStringList rowContents;
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
        {
            if (ui->tableWidget->isColumnHidden(col) || col == 9) // Skip Actions column and hidden columns
                continue;

            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            rowContents << (item ? "\"" + item->text().replace("\"", "\"\"") + "\"" : "");
        }
        out << rowContents.join(",") << "\n";
    }

    file.close();
    QMessageBox::information(this, "Export", "Export completed successfully.");
}

// ========================================================================
// OMDB API / MOVIE DATA
// ========================================================================

void MainWindow::onFetchClicked()
{
    qDebug() << "Fetch button clicked. Reading titles and years from table.";

    // Count how many movies need fetching
    totalMoviesToFetch = 0;
    moviesFetched = 0;
    moviesFromDatabase = 0;
    moviesFromImdb = 0;
    moviesFetchedFromImdbList.clear(); // Clear the list for new fetch session

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        QTableWidgetItem *titleItem = ui->tableWidget->item(row, 0);
        if (titleItem && !titleItem->text().trimmed().isEmpty())
        {
            totalMoviesToFetch++;
        }
    }

    if (totalMoviesToFetch == 0)
    {
        QMessageBox::information(this, "No Movies", "No movies found to fetch data for.");
        return;
    }

    // Create progress dialog
    if (progressDialog)
    {
        delete progressDialog;
    }

    progressDialog = new QProgressDialog("Fetching movie data from IMDb...", "Cancel", 0, totalMoviesToFetch, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setMinimumDuration(0);
    progressDialog->setValue(0);
    progressDialog->show();

    // Process movies one by one
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        // Check if user cancelled
        if (progressDialog && progressDialog->wasCanceled())
        {
            ui->statusbar->showMessage("Movie data fetch cancelled by user.", 3000);
            break;
        }

        QTableWidgetItem *titleItem = ui->tableWidget->item(row, 0);
        QTableWidgetItem *yearItem = ui->tableWidget->item(row, 1);

        if (titleItem)
        {
            QString title = titleItem->text().trimmed();
            int year = 0;

            if (yearItem)
                year = yearItem->text().trimmed().toInt();

            if (!title.isEmpty())
            {
                qDebug() << "Fetching movie in row" << row << ":" << title << ", year:" << year;

                // Update progress dialog
                moviesFetched++;
                if (progressDialog)
                {
                    progressDialog->setValue(moviesFetched);
                    progressDialog->setLabelText(QString("Fetching movie %1 of %2: %3")
                                                     .arg(moviesFetched)
                                                     .arg(totalMoviesToFetch)
                                                     .arg(title));
                }

                // Fetch the movie (this is async via QNetworkAccessManager)
                omdbClient->fetchMovie(title, year);

                // Process events to keep UI responsive
                QCoreApplication::processEvents();

                // Small delay to avoid overwhelming the API
                QThread::msleep(100);
            }
        }
    }

    // Clean up progress dialog after a short delay
    if (progressDialog)
    {
        progressDialog->setValue(totalMoviesToFetch);
        QTimer::singleShot(1000, this, &MainWindow::cleanupProgressDialog);
    }

    // Initial status message (will be updated as movies are fetched)
    ui->statusbar->showMessage(QString("Fetching data for %1 movies...").arg(totalMoviesToFetch));
}
QString MainWindow::sanitizeForWindowsFolder(const QString &name)
{
    static const QRegularExpression forbidden(R"([\\/:*?"<>|,.])");
    return QString(name).remove(forbidden);
}

void MainWindow::onMovieFetched(const Movie &movie)
{
    const QString sanitizedTitle = sanitizeForWindowsFolder(movie.title);

    // Track as fetched from IMDb
    moviesFromImdb++;
    moviesFetchedFromImdbList.append(movie.title); // Add to the list

    // Update status bar with statistics (permanent message - no timeout)
    ui->statusbar->showMessage(QString("Fetched %1 of %2 movies | From Database: %3 | From IMDb: %4")
                                   .arg(moviesFromDatabase + moviesFromImdb)
                                   .arg(totalMoviesToFetch)
                                   .arg(moviesFromDatabase)
                                   .arg(moviesFromImdb));

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (!item)
            continue;

        const QString rowTitle = sanitizeForWindowsFolder(item->text());

        if (rowTitle == sanitizedTitle)
        {
            // store imdbID on the row's title item so the IMDb button can open direct URL
            QTableWidgetItem *titleItem = ui->tableWidget->item(row, 0);
            if (titleItem)
            {
                titleItem->setData(ImdbIdRole, movie.imdbID);
                // Update the IMDb button tooltip in the Actions cell if present
                QWidget *actionsWidget = ui->tableWidget->cellWidget(row, 9);
                if (actionsWidget)
                {
                    // find child QPushButton with objectName imdbButton
                    QPushButton *btn = actionsWidget->findChild<QPushButton *>("imdbButton");
                    if (btn)
                        btn->setToolTip(movie.imdbID.isEmpty() ? "Search on IMDb" : "Open IMDb page");
                }
            }
            ui->tableWidget->setItem(row, 10, new QTableWidgetItem(movie.rated));
            ui->tableWidget->setItem(row, 11, new QTableWidgetItem(movie.imdbRating));
            ui->tableWidget->setItem(row, 12, new NumericTableWidgetItem(movie.imdbVotes));
            ui->tableWidget->setItem(row, 13, new QTableWidgetItem(movie.director));
            ui->tableWidget->setItem(row, 14, new QTableWidgetItem(movie.actors));
            ui->tableWidget->setItem(row, 15, new QTableWidgetItem(movie.writer));
            ui->tableWidget->setItem(row, 16, new QTableWidgetItem(movie.awards));
            ui->tableWidget->setItem(row, 17, new QTableWidgetItem(movie.language));
            ui->tableWidget->setItem(row, 18, new QTableWidgetItem(movie.country));
            ui->tableWidget->setItem(row, 19, new QTableWidgetItem(movie.boxOffice));
            ui->tableWidget->setItem(row, 20, new QTableWidgetItem(movie.plot));
            ui->tableWidget->setItem(row, 21, new QTableWidgetItem(movie.genre));
            break;
        }
    }
}

void MainWindow::onMovieFetchedFromDatabase(const Movie &movie)
{
    const QString sanitizedTitle = sanitizeForWindowsFolder(movie.title);

    // Track as fetched from database
    moviesFromDatabase++;

    // Update status bar with statistics (permanent message - no timeout)
    ui->statusbar->showMessage(QString("Fetched %1 of %2 movies | From Database: %3 | From IMDb: %4")
                                   .arg(moviesFromDatabase + moviesFromImdb)
                                   .arg(totalMoviesToFetch)
                                   .arg(moviesFromDatabase)
                                   .arg(moviesFromImdb));

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (!item)
            continue;

        const QString rowTitle = sanitizeForWindowsFolder(item->text());

        if (rowTitle == sanitizedTitle)
        {
            // store imdbID on the row's title item so the IMDb button can open direct URL
            QTableWidgetItem *titleItem = ui->tableWidget->item(row, 0);
            if (titleItem)
            {
                titleItem->setData(ImdbIdRole, movie.imdbID);
                // Update the IMDb button tooltip in the Actions cell if present
                QWidget *actionsWidget = ui->tableWidget->cellWidget(row, 9);
                if (actionsWidget)
                {
                    // find child QPushButton with objectName imdbButton
                    QPushButton *btn = actionsWidget->findChild<QPushButton *>("imdbButton");
                    if (btn)
                        btn->setToolTip(movie.imdbID.isEmpty() ? "Search on IMDb" : "Open IMDb page");
                }
            }
            ui->tableWidget->setItem(row, 10, new QTableWidgetItem(movie.rated));
            ui->tableWidget->setItem(row, 11, new QTableWidgetItem(movie.imdbRating));
            ui->tableWidget->setItem(row, 12, new NumericTableWidgetItem(movie.imdbVotes));
            ui->tableWidget->setItem(row, 13, new QTableWidgetItem(movie.director));
            ui->tableWidget->setItem(row, 14, new QTableWidgetItem(movie.actors));
            ui->tableWidget->setItem(row, 15, new QTableWidgetItem(movie.writer));
            ui->tableWidget->setItem(row, 16, new QTableWidgetItem(movie.awards));
            ui->tableWidget->setItem(row, 17, new QTableWidgetItem(movie.language));
            ui->tableWidget->setItem(row, 18, new QTableWidgetItem(movie.country));
            ui->tableWidget->setItem(row, 19, new QTableWidgetItem(movie.boxOffice));
            ui->tableWidget->setItem(row, 20, new QTableWidgetItem(movie.plot));
            ui->tableWidget->setItem(row, 21, new QTableWidgetItem(movie.genre));
            break;
        }
    }
}

void MainWindow::onRefreshMovieClicked()
{
    if (contextMenuRow < 0 || contextMenuRow >= ui->tableWidget->rowCount())
        return;

    QTableWidgetItem *titleItem = ui->tableWidget->item(contextMenuRow, 0);
    QTableWidgetItem *yearItem = ui->tableWidget->item(contextMenuRow, 1);

    if (!titleItem)
        return;

    QString title = titleItem->text().trimmed();
    int year = 0;
    if (yearItem)
        year = yearItem->text().trimmed().toInt();

    if (title.isEmpty())
        return;

    // Store the pending refresh information
    pendingRefreshMovieName = title;
    pendingRefreshMovieYear = year;

    // Request movie data with forceRefresh=true
    omdbClient->fetchMovie(title, year, true);
}

void MainWindow::onMovieExistsInDatabase(const QString &title, const Movie &existingMovie)
{
    bool confirmed = dataRefresher->showOverwriteDialog(title, existingMovie, this);

    if (confirmed)
    {
        // Re-fetch the movie data
        omdbClient->fetchMovie(pendingRefreshMovieName, pendingRefreshMovieYear, false);
    }
}

void MainWindow::loadExternalStylesheet()
{
    QFile styleFile("styles.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream styleStream(&styleFile);
        QString style = styleStream.readAll();
        styleFile.close();
        this->setStyleSheet(style);
    }
}

// ========================================================================
// DRIVE & YEAR SELECTION
// ========================================================================

QString MainWindow::buildFolderPath(const QString &drive, int year)
{
    QString path = drive;

    if (year >= 2000)
    {
        path += "+2000\\";
        int decade = (year / 10) % 10; // Get decade digit (0s, 10s, 20s)
        path += QString::number(decade) + "0s\\";
    }
    else
    {
        path += "-2000\\";
        int decade = (year / 10) % 10; // Get decade digit (50s, 60s, 70s, 80s, 90s)
        path += QString::number(decade) + "0s\\";
    }

    path += QString::number(year);

    return path;
}

bool MainWindow::meetsHighQualityCriteria(int votes, double rating)
{
    return (votes >= 100000 && rating >= 7.0);
}

void MainWindow::onFetchByDriveYearClicked()
{
    QString drive = driveComboBox->currentText();
    QString yearText = yearLineEdit->text().trimmed();

    if (yearText.isEmpty())
    {
        QMessageBox::warning(this, "Input Required", "Please enter a year (e.g., 2023, 1994)");
        return;
    }

    bool ok;
    int year = yearText.toInt(&ok);

    if (!ok || year < 1900 || year > 2100)
    {
        QMessageBox::warning(this, "Invalid Year", "Please enter a valid year between 1900 and 2100");
        return;
    }

    QString folderPath = buildFolderPath(drive, year);

    QDir dir(folderPath);
    if (!dir.exists())
    {
        QMessageBox::warning(this, "Folder Not Found",
                             QString("The folder does not exist:\n%1\n\nExpected format:\n%2")
                                 .arg(folderPath)
                                 .arg("Drive:\\+2000\\20s\\2023 or Drive:\\-2000\\90s\\1994"));
        return;
    }

    ui->statusbar->showMessage(QString("Scanning %1...").arg(folderPath));

    // Process videos from this folder
    processVideos(folderPath, false);

    ui->statusbar->showMessage(QString("Loaded movies from %1").arg(folderPath), 5000);
}

void MainWindow::onShowMoviesToMoveClicked()
{
    // Check if drive is selected
    if (!driveComboBox || driveComboBox->currentText().isEmpty())
    {
        QMessageBox::warning(this, "No Drive Selected", "Please select a drive (D or E) first");
        return;
    }

    QString currentDrive = driveComboBox->currentText();

    // Collect movies that need to be moved
    QList<MovieToMove> moviesToMove;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        // Skip hidden rows
        if (ui->tableWidget->isRowHidden(row))
            continue;

        QTableWidgetItem *titleItem = ui->tableWidget->item(row, 0);
        QTableWidgetItem *yearItem = ui->tableWidget->item(row, 1);
        QTableWidgetItem *ratingItem = ui->tableWidget->item(row, 11);
        QTableWidgetItem *votesItem = ui->tableWidget->item(row, 12);

        if (!titleItem || !yearItem || !ratingItem || !votesItem)
            continue;

        QString title = titleItem->text();
        QString year = yearItem->text();
        QString ratingStr = ratingItem->text();
        QString votesStr = votesItem->text();

        // Get file path from first column
        QString moviePath = titleItem->data(Qt::UserRole + 1).toString();
        if (moviePath.isEmpty())
            continue;

        // Extract drive from path
        QString movieDrive;
        if (moviePath.startsWith("D:", Qt::CaseInsensitive))
            movieDrive = "D";
        else if (moviePath.startsWith("E:", Qt::CaseInsensitive))
            movieDrive = "E";
        else
            continue;

        // Parse rating and votes
        double rating = ratingStr.toDouble();
        QString votesNumeric = votesStr;
        votesNumeric.remove(',');
        votesNumeric.remove('K');
        int votes = 0;
        if (votesStr.contains('K'))
        {
            votes = static_cast<int>(votesNumeric.toDouble() * 1000);
        }
        else
        {
            votes = votesNumeric.toInt();
        }

        // Check if movie meets criteria for D drive
        bool meetsHighQuality = meetsHighQualityCriteria(votes, rating);

        // Determine if movie is in wrong location
        bool needsMove = false;
        QString targetDrive;

        if (meetsHighQuality && movieDrive == "E")
        {
            // Should be on D but is on E
            needsMove = true;
            targetDrive = "D";
        }
        else if (!meetsHighQuality && movieDrive == "D")
        {
            // Should be on E but is on D
            needsMove = true;
            targetDrive = "E";
        }

        if (needsMove)
        {
            MovieToMove movie;
            movie.title = title;
            movie.year = year;
            movie.imdbRating = ratingStr;
            movie.imdbVotes = votesStr;

            // Get folder path (without movie folder name)
            QFileInfo fileInfo(moviePath);
            QString currentFolderPath = fileInfo.dir().absolutePath();
            movie.currentPath = currentFolderPath;
            movie.targetDrive = targetDrive;

            // Build target path by replacing drive letter
            // E.g., D:\+2000\20s\2023 becomes E:\+2000\20s\2023
            QString targetPath = currentFolderPath;
            if (movieDrive == "D")
            {
                targetPath.replace(0, 2, "E:");
            }
            else if (movieDrive == "E")
            {
                targetPath.replace(0, 2, "D:");
            }
            movie.targetPath = targetPath;

            moviesToMove.append(movie);
        }
    }

    // Create and show the window
    MoviesToMoveWindow *window = new MoviesToMoveWindow(this);
    window->setMoviesToMove(moviesToMove);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();

    ui->statusbar->showMessage(QString("Found %1 movie(s) that need to be moved").arg(moviesToMove.size()), 5000);
}

void MainWindow::onSettingsClicked()
{
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->exec();
    delete dialog;
}

void MainWindow::showFetchSummary()
{
    // Check if setting is enabled
    QSettings settings("MovieInfo", "MovieInfoApp");
    bool showSummary = settings.value("fetch/showSummary", true).toBool();

    if (!showSummary || moviesFetchedFromImdbList.isEmpty())
    {
        return;
    }

    // Create dialog
    QDialog *summaryDialog = new QDialog(this);
    summaryDialog->setWindowTitle("Fetch Summary");
    summaryDialog->setMinimumWidth(600);
    summaryDialog->setMinimumHeight(400);

    QVBoxLayout *layout = new QVBoxLayout(summaryDialog);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel(QString("Movies Fetched from IMDb (%1)").arg(moviesFetchedFromImdbList.size()));
    titleLabel->setStyleSheet(
        "QLabel {"
        "    color: #c9d1d9;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    padding-bottom: 10px;"
        "}");
    layout->addWidget(titleLabel);

    // List of movies
    QTextEdit *movieList = new QTextEdit();
    movieList->setReadOnly(true);
    movieList->setStyleSheet(
        "QTextEdit {"
        "    background-color: #161b22;"
        "    color: #c9d1d9;"
        "    border: 1px solid #30363d;"
        "    border-radius: 6px;"
        "    padding: 10px;"
        "    font-family: 'Consolas', 'Courier New', monospace;"
        "    font-size: 12px;"
        "}");

    QString movieText;
    for (int i = 0; i < moviesFetchedFromImdbList.size(); ++i)
    {
        movieText += QString("%1. %2\n").arg(i + 1).arg(moviesFetchedFromImdbList[i]);
    }
    movieList->setPlainText(movieText);
    layout->addWidget(movieList);

    // Statistics
    QLabel *statsLabel = new QLabel(
        QString("Total: %1 movies | From Database: %2 | From IMDb: %3")
            .arg(moviesFromDatabase + moviesFromImdb)
            .arg(moviesFromDatabase)
            .arg(moviesFromImdb));
    statsLabel->setStyleSheet(
        "QLabel {"
        "    color: #8b949e;"
        "    font-size: 11px;"
        "    padding: 5px;"
        "}");
    layout->addWidget(statsLabel);

    // Close button
    QPushButton *closeButton = new QPushButton("Close");
    closeButton->setMinimumWidth(100);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #238636;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2ea043;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1a7f37;"
        "}");
    connect(closeButton, &QPushButton::clicked, summaryDialog, &QDialog::accept);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);

    // Style the dialog
    summaryDialog->setStyleSheet(
        "QDialog {"
        "    background-color: #0d1117;"
        "}");

    summaryDialog->exec();
    delete summaryDialog;
}

void MainWindow::onClearTableClicked()
{
    // Confirm with user before clearing
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Clear Table",
        "Are you sure you want to clear all movies from the table?\n\nThis will not delete the database or files, only clear the current view.",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Clear the table
        ui->tableWidget->setRowCount(0);

        // Clear combo boxes (keep "All" option)
        ui->comboBoxDecade->clear();
        ui->comboBoxAspectRatio->clear();
        ui->comboBoxQuality->clear();

        addComboBoxItemIfNotExist(ui->comboBoxDecade, "All");
        addComboBoxItemIfNotExist(ui->comboBoxAspectRatio, "All");
        addComboBoxItemIfNotExist(ui->comboBoxQuality, "All");

        // Clear search
        ui->searchLineEdit->clear();

        // Update status bar
        ui->statusbar->showMessage("Table cleared successfully. Total movies: 0", 3000);

        qDebug() << "Table cleared by user";
    }
}
