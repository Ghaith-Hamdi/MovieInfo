#include "mainwindow.h"
#include "ui_mainwindow.h"

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
#include <QRegularExpression>
#include <QSet>
#include <QSettings>
#include <QTextStream>
#include <QUrl>
#include <QInputDialog>
#include <windows.h> // For GetLastError()
#include <QThread>
#include <QToolBar>
#include <QDragEnterEvent>

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

// ===================== Constructor / Destructor =====================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    omdbClient = new OmdbClient("5af6b86e", movieDb, this);
    // omdbClient = new OmdbClient("c774e520", movieDb, this);
    //  Setup table
    ui->tableWidget->setColumnCount(22);
    ui->tableWidget->setHorizontalHeaderLabels({"Title", "Year", "Decade", "Resolution", "Aspect Ratio", "Quality", "Size", "Duration", "Language",
                                                "Actions", "Rated", "Rating", "Votes", "Director", "Actors", "Writers", "Awards", "Language", "Country", "Box Office", "Plot", "Genre"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Initialize column visibility features
    loadColumnVisibilitySettings();
    setupColumnVisibilityMenu();

    // Initialize column ordering features
    loadColumnOrderSettings();
    setupColumnReorderingMenu();

    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(ui->selectFolderButton, &QPushButton::clicked, this, &MainWindow::onSelectFolderClicked);

    // Initialize combo boxes with "All"
    for (auto combo : {ui->comboBoxDecade, ui->comboBoxAspectRatio, ui->comboBoxQuality})
        addComboBoxItemIfNotExist(combo, "All");

    setAcceptDrops(true);

    // Connect combo boxes to filterTable
    auto connectCombo = [this](QComboBox *box)
    {
        connect(box, &QComboBox::currentTextChanged, this, &MainWindow::filterTable);
    };
    connectCombo(ui->comboBoxDecade);
    connectCombo(ui->comboBoxAspectRatio);
    connectCombo(ui->comboBoxQuality);

    // Other signal connections
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportToExcel);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterTableRows);
    connect(omdbClient, &OmdbClient::movieFetched, this, &MainWindow::onMovieFetched);
    connect(ui->fetchButton, &QPushButton::clicked, this, &MainWindow::onFetchClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    // movieDb and omdbClient will be deleted automatically by Qt parent system
}

void MainWindow::onSelectFolderClicked()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    QString lastFolder = settings.value("lastFolder").toString();
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", lastFolder);
    if (!folderPath.isEmpty())
    {
        settings.setValue("lastFolder", folderPath);
        processVideos(folderPath, false); // Process as folder
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    for (const QUrl &url : urls)
    {
        QString localFile = url.toLocalFile();
        if (QFileInfo(localFile).isFile())
        {
            QString ext = QFileInfo(localFile).suffix().toLower();
            if (ext == "mp4" || ext == "mkv" || ext == "avi" || ext == "mov" || ext == "flv" || ext == "wmv")
            {
                processVideos(localFile, true); // Process as single file
            }
        }
    }
}

// ===================== Video Processing =====================

void MainWindow::processVideos(const QStringList &filePaths)
{
    for (const QString &filePath : filePaths)
    {
        processVideos(filePath, true);
    }
}

void MainWindow::processVideos(const QString &path, bool isSingleFile)
{
    QStringList videoExtensions = {".mp4", ".mkv", ".avi", ".mov", ".flv", ".wmv"};
    QSet<QString> decades, aspectRatios, qualities;
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

    for (const QString &filePath : filesToProcess)
    {
        QString resolution = getVideoResolution(filePath);
        QString aspectRatio = getAspectRatio(resolution);
        QString folderName = QFileInfo(filePath).dir().dirName();
        auto [title, year] = parseFolderName(folderName);
        QString decade = getDecade(year);
        QString quality = getVideoQuality(filePath);
        QString duration = getVideoDuration(filePath);
        QString fileSize = getFileSize(filePath);
        QString audioLanguage = getAudioLanguage(filePath);

        decades.insert(decade);
        aspectRatios.insert(aspectRatio);
        qualities.insert(quality);

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setData(FilePathRole, filePath);
        ui->tableWidget->setItem(row, 0, titleItem);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(year));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(decade));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(resolution));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(aspectRatio));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(quality));
        ui->tableWidget->setItem(row, 6, new NumericTableWidgetItem(fileSize));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(duration));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(audioLanguage));

        QPushButton *openButton = new QPushButton("Open");
        connect(openButton, &QPushButton::clicked, this, [filePath]()
                { QDesktopServices::openUrl(QUrl::fromLocalFile(filePath)); });

        QPushButton *imdbButton = new QPushButton("IMDb");
        connect(imdbButton, &QPushButton::clicked, this, [this, title, year]()
                { openImdbPage(title, year); });

        QPushButton *paheButton = new QPushButton("Pahe");
        connect(paheButton, &QPushButton::clicked, this, [this, title, year]()
                { openPahePage(title, year); });

        QWidget *buttonsWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(buttonsWidget);
        layout->addWidget(openButton);
        layout->addWidget(imdbButton);
        layout->addWidget(paheButton);
        layout->setContentsMargins(0, 0, 0, 0);
        ui->tableWidget->setCellWidget(row, 9, buttonsWidget);
    }

    addComboBoxItemsSorted(ui->comboBoxDecade, decades);
    addComboBoxItemsSorted(ui->comboBoxAspectRatio, aspectRatios, "UltraWide");
    addComboBoxItemsSorted(ui->comboBoxQuality, qualities);

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

// ===================== Utility Functions =====================

QString MainWindow::runFfprobe(const QStringList &args)
{
    QProcess ffprobe;
    ffprobe.start("ffprobe", args);
    ffprobe.waitForFinished();
    return ffprobe.readAllStandardOutput().trimmed();
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

// ===================== Filtering / Sorting =====================
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

// ===================== Context Menu =====================

void MainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->tableWidget->indexAt(pos);
    if (!index.isValid())
        return;

    QMenu menu(this);
    QAction *openFolderAction = menu.addAction("Open Containing Folder");
    QAction *renameFolderAction = menu.addAction("Rename Folder");
    QAction *moveFolderAction = menu.addAction("Move to Archive Folder");

    const QString kArchiveFolderPath = "D:/New folder";

    auto getFileInfo = [this](int row) -> std::optional<QFileInfo>
    {
        auto *item = ui->tableWidget->item(row, 0);
        if (!item)
            return std::nullopt;

        QString filePath = item->data(FilePathRole).toString();
        if (filePath.isEmpty())
            return std::nullopt;

        QFileInfo fileInfo(filePath);
        if (!fileInfo.exists())
            return std::nullopt;

        return fileInfo;
    };

    connect(openFolderAction, &QAction::triggered, this, [=]()
            {
        auto fileInfoOpt = getFileInfo(index.row());
        if (!fileInfoOpt) {
            QMessageBox::warning(this, "Error", "Could not determine file path.");
            return;
        }
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfoOpt->absolutePath())); });

    connect(renameFolderAction, &QAction::triggered, this, [=]()
            {
        auto fileInfoOpt = getFileInfo(index.row());
        if (!fileInfoOpt)
            return;

        const QFileInfo &fileInfo = *fileInfoOpt;
        QDir parentDir = fileInfo.dir();
        QString currentPath = parentDir.absolutePath();
        QString title = ui->tableWidget->item(index.row(), 0)->text();
        QString year = ui->tableWidget->item(index.row(), 1)->text();
        QString suggestedName = QString("%1 (%2)").arg(title, year);

        bool ok;
        QString newName = QInputDialog::getText(this, "Rename Folder", "Enter new folder name:",
                                                QLineEdit::Normal, suggestedName, &ok);

        if (!ok || newName.trimmed().isEmpty())
            return;

        newName = sanitizeForWindowsFolder(newName).replace('\n', ' ').trimmed();
        if (newName.isEmpty()) {
            QMessageBox::warning(this, "Error", "Invalid folder name after sanitization.");
            return;
        }

        QDir baseDir = parentDir;
        baseDir.cdUp();
        QString newPath = QDir::cleanPath(baseDir.absoluteFilePath(newName));

        if (QDir(newPath).exists()) {
            QMessageBox::warning(this, "Error", "Target folder already exists.");
            return;
        }

        QString newPathNative = QDir::toNativeSeparators(newPath);
        QString currentPathNative = QDir::toNativeSeparators(currentPath);

        bool renamed = QDir().rename(currentPathNative, newPathNative);
        if (!renamed) {
            QProcess elevate;
            QStringList args = {"/c", "move",
                                QString("\"%1\"").arg(currentPathNative),
                                QString("\"%1\"").arg(newPathNative)};
            elevate.start("cmd.exe", args);
            elevate.waitForFinished();

            renamed = QDir(newPathNative).exists() && !QDir(currentPathNative).exists();
        }

        if (renamed) {
            QString newFilePath = newPathNative + QDir::separator() + fileInfo.fileName();
            ui->tableWidget->item(index.row(), 0)->setData(FilePathRole, newFilePath);
            QMessageBox::information(this, "Success", "Folder renamed successfully.");
        } else {
            auto error = GetLastError();
            QString errorMsg = QString("Failed to rename folder.\nError code: %1 (0x%2)\n%3\n\nFrom: %4\nTo: %5")
                                   .arg(error)
                                   .arg(error, 8, 16, QChar('0'))
                                   .arg(QString::fromLocal8Bit(strerror(error)))
                                   .arg(currentPathNative)
                                   .arg(newPathNative);
            QMessageBox::warning(this, "Error", errorMsg);
        } });

    connect(moveFolderAction, &QAction::triggered, this, [=]()
            {
        QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
        if (ranges.isEmpty()) {
            QMessageBox::warning(this, "No Selection", "Please select at least one movie.");
            return;
        }

        QDir archiveDir(kArchiveFolderPath);
        if (!archiveDir.exists() && !archiveDir.mkpath(".")) {
            QMessageBox::warning(this, "Error", "Failed to create archive folder.");
            return;
        }

        int movedCount = 0;

        for (const QTableWidgetSelectionRange &range : ranges) {
            for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
                auto fileInfoOpt = getFileInfo(row);
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
                if (moved) {
                    QString newFilePath = newPathNative + QDir::separator() + fileInfo.fileName();
                    ui->tableWidget->item(row, 0)->setData(FilePathRole, newFilePath);
                    ++movedCount;
                }
            }
        }

        if (movedCount > 0) {
            QMessageBox::information(this, "Success", QString("Moved %1 folder(s) to archive.").arg(movedCount));
        } else {
            QMessageBox::warning(this, "No Folders Moved", "No folders were moved. They may already exist in the archive.");
        } });

    menu.exec(ui->tableWidget->viewport()->mapToGlobal(pos));
}

// ===================== Utility / Helper Functions =====================

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

// ===================== External Site Integration =====================

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

// ===================== Export =====================

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

// ===================== OMDB API Integration =====================

void MainWindow::onFetchClicked()
{
    qDebug() << "Fetch button clicked. Reading titles and years from table.";

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
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
                qDebug() << "Found movie in row" << row << ":" << title << ", year:" << year;
                omdbClient->fetchMovie(title, year);
            }
        }
    }
}

QString MainWindow::sanitizeForWindowsFolder(const QString &name)
{
    static const QRegularExpression forbidden(R"([\\/:*?"<>|,.])");
    return QString(name).remove(forbidden);
}

void MainWindow::onMovieFetched(const Movie &movie)
{
    const QString sanitizedTitle = sanitizeForWindowsFolder(movie.title);

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (!item)
            continue;

        const QString rowTitle = sanitizeForWindowsFolder(item->text());

        if (rowTitle == sanitizedTitle)
        {
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

void MainWindow::setupColumnVisibilityMenu()
{
    // Set up column name mapping
    columnIndexToName = {
        {0, "Title"}, {1, "Year"}, {2, "Decade"}, {3, "Resolution"}, {4, "Aspect Ratio"}, {5, "Quality"}, {6, "Size"}, {7, "Duration"}, {8, "Language"}, {9, "Actions"}, {10, "Rated"}, {11, "Rating"}, {12, "Votes"}, {13, "Director"}, {14, "Actors"}, {15, "Writers"}, {16, "Awards"}, {17, "Language"}, {18, "Country"}, {19, "Box Office"}, {20, "Plot"}, {21, "Genre"}};

    // Create a button for column visibility
    QPushButton *columnVisibilityButton = new QPushButton("Columns", this);
    ui->toolBar->addWidget(columnVisibilityButton);

    // Create a menu for column visibility options
    QMenu *columnMenu = new QMenu(this);
    columnVisibilityButton->setMenu(columnMenu);

    // Add "Select All" option
    QAction *selectAllAction = columnMenu->addAction("Select All");
    connect(selectAllAction, &QAction::triggered, this, [this]()
            {
        for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
            columnVisibility[i] = true;
        }
        updateColumnVisibility();
        saveColumnVisibilitySettings(); });

    // Add "Deselect All" option
    QAction *deselectAllAction = columnMenu->addAction("Deselect All");
    connect(deselectAllAction, &QAction::triggered, this, [this]()
            {
        // Always keep Title column visible
        for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
            columnVisibility[i] = (i == 0); // Only Title column remains visible
        }
        updateColumnVisibility();
        saveColumnVisibilitySettings(); });

    columnMenu->addSeparator();

    // Add checkboxes for each column
    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        QString columnName = columnIndexToName.value(i, ui->tableWidget->horizontalHeaderItem(i)->text());
        QAction *action = columnMenu->addAction(columnName);
        action->setCheckable(true);
        action->setChecked(columnVisibility.value(i, true));

        connect(action, &QAction::toggled, this, [this, i](bool checked)
                {
            columnVisibility[i] = checked;
            updateColumnVisibility();
            saveColumnVisibilitySettings(); });
    }
}

void MainWindow::updateColumnVisibility()
{
    // Ensure at least one column (Title) is always visible
    if (!columnVisibility.value(0, true))
    {
        columnVisibility[0] = true;
    }

    // Update column visibility
    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        bool isVisible = columnVisibility.value(i, true);
        ui->tableWidget->setColumnHidden(i, !isVisible);
    }

    // Resize columns to fit content
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::saveColumnVisibilitySettings()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    settings.beginGroup("ColumnVisibility");

    // Clear existing settings
    settings.remove("");

    // Save each column's visibility state
    for (auto it = columnVisibility.constBegin(); it != columnVisibility.constEnd(); ++it)
    {
        settings.setValue(QString::number(it.key()), it.value());
    }

    settings.endGroup();
}

void MainWindow::loadColumnVisibilitySettings()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    settings.beginGroup("ColumnVisibility");

    // Check if settings exist, if not, set all columns to visible by default
    QStringList keys = settings.childKeys();
    if (keys.isEmpty())
    {
        for (int i = 0; i < ui->tableWidget->columnCount(); i++)
        {
            columnVisibility[i] = true;
        }
    }
    else
    {
        // Load settings for each column
        for (int i = 0; i < ui->tableWidget->columnCount(); i++)
        {
            bool isVisible = settings.value(QString::number(i), true).toBool();
            columnVisibility[i] = isVisible;
        }
    }

    settings.endGroup();

    // Apply loaded settings
    updateColumnVisibility();
}

void MainWindow::setupColumnReorderingMenu()
{
    // Create a "Reorder Columns" button
    QPushButton *reorderButton = new QPushButton("Reorder", this);
    ui->toolBar->addWidget(reorderButton);

    QMenu *reorderMenu = new QMenu(this);
    reorderButton->setMenu(reorderMenu);

    // Add "Reset Order" option
    QAction *resetOrderAction = reorderMenu->addAction("Reset to Default Order");
    connect(resetOrderAction, &QAction::triggered, this, [this]()
            {
        // Reset column order to default
        QMap<int, int> defaultOrder;
        for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
            defaultOrder[i] = i;
        }
        
        // Update maps
        columnOriginalToCurrentMap.clear();
        columnCurrentToOriginalMap.clear();
        for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
            columnOriginalToCurrentMap[i] = i;
            columnCurrentToOriginalMap[i] = i;
        }
        
        // Apply column order
        for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
            ui->tableWidget->horizontalHeader()->moveSection(
                ui->tableWidget->horizontalHeader()->visualIndex(i), i);
        }
        
        // Save the default order
        saveColumnOrderSettings();
        
        // Update header labels
        updateHeaderLabels(); });

    reorderMenu->addSeparator();

    // Add move options for each column
    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        QString columnName = columnIndexToName.value(i, ui->tableWidget->horizontalHeaderItem(i)->text());

        QMenu *columnMenu = reorderMenu->addMenu(columnName);

        // Move Left action
        QAction *moveLeftAction = columnMenu->addAction("Move Left");
        connect(moveLeftAction, &QAction::triggered, this, [this, i]()
                {
            int visualIndex = ui->tableWidget->horizontalHeader()->visualIndex(i);
            if (visualIndex > 0) {
                reorderColumn(i, visualIndex - 1);
                saveColumnOrderSettings();
            } });

        // Move Right action
        QAction *moveRightAction = columnMenu->addAction("Move Right");
        connect(moveRightAction, &QAction::triggered, this, [this, i]()
                {
            int visualIndex = ui->tableWidget->horizontalHeader()->visualIndex(i);
            if (visualIndex < ui->tableWidget->columnCount() - 1) {
                reorderColumn(i, visualIndex + 1);
                saveColumnOrderSettings();
            } });

        // Move to Position action
        QMenu *moveToMenu = columnMenu->addMenu("Move to Position");
        for (int pos = 0; pos < ui->tableWidget->columnCount(); pos++)
        {
            QAction *moveToAction = moveToMenu->addAction(QString::number(pos + 1));
            connect(moveToAction, &QAction::triggered, this, [this, i, pos]()
                    {
                reorderColumn(i, pos);
                saveColumnOrderSettings(); });
        }
    }

    // Make headers also reorderable by drag and drop
    ui->tableWidget->horizontalHeader()->setSectionsMovable(true);

    // Connect signal when user drags headers to reorder
    connect(ui->tableWidget->horizontalHeader(), &QHeaderView::sectionMoved,
            this, [this](int logicalIndex, int oldVisualIndex, int newVisualIndex)
            {
        Q_UNUSED(logicalIndex);
        Q_UNUSED(oldVisualIndex);
        
        // Update our mappings
        columnOriginalToCurrentMap.clear();
        columnCurrentToOriginalMap.clear();
        
        for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
            int visualIndex = ui->tableWidget->horizontalHeader()->visualIndex(i);
            columnOriginalToCurrentMap[i] = visualIndex;
            columnCurrentToOriginalMap[visualIndex] = i;
        }
        
        // Save new order
        saveColumnOrderSettings(); });
}

void MainWindow::reorderColumn(int column, int newPosition)
{
    int currentVisualIndex = ui->tableWidget->horizontalHeader()->visualIndex(column);
    ui->tableWidget->horizontalHeader()->moveSection(currentVisualIndex, newPosition);

    // Update our mappings
    columnOriginalToCurrentMap.clear();
    columnCurrentToOriginalMap.clear();

    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        int visualIndex = ui->tableWidget->horizontalHeader()->visualIndex(i);
        columnOriginalToCurrentMap[i] = visualIndex;
        columnCurrentToOriginalMap[visualIndex] = i;
    }
}

void MainWindow::saveColumnOrderSettings()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    settings.beginGroup("ColumnOrder");

    // Clear existing settings
    settings.remove("");

    // Save current order mapping
    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        int visualIndex = ui->tableWidget->horizontalHeader()->visualIndex(i);
        settings.setValue(QString::number(i), visualIndex);
    }

    settings.endGroup();
}

void MainWindow::loadColumnOrderSettings()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    settings.beginGroup("ColumnOrder");

    // Check if settings exist
    QStringList keys = settings.childKeys();

    // Initialize with default mapping (identity map)
    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        columnOriginalToCurrentMap[i] = i;
        columnCurrentToOriginalMap[i] = i;
    }

    if (!keys.isEmpty())
    {
        // Load custom column order
        QMap<int, int> loadedOrder;

        for (int i = 0; i < ui->tableWidget->columnCount(); i++)
        {
            int visualIndex = settings.value(QString::number(i), i).toInt();
            loadedOrder[i] = visualIndex;
        }

        // Apply loaded order
        // First sort by visual index to ensure correct order
        QList<int> sortedColumns;
        for (int i = 0; i < ui->tableWidget->columnCount(); i++)
        {
            sortedColumns.append(i);
        }

        std::sort(sortedColumns.begin(), sortedColumns.end(), [&loadedOrder](int a, int b)
                  { return loadedOrder[a] < loadedOrder[b]; });

        // Now apply the order
        for (int i = 0; i < sortedColumns.size(); i++)
        {
            int logicalIndex = sortedColumns[i];
            int currentVisualIndex = ui->tableWidget->horizontalHeader()->visualIndex(logicalIndex);
            ui->tableWidget->horizontalHeader()->moveSection(currentVisualIndex, i);
        }

        // Update our mappings
        for (int i = 0; i < ui->tableWidget->columnCount(); i++)
        {
            int visualIndex = ui->tableWidget->horizontalHeader()->visualIndex(i);
            columnOriginalToCurrentMap[i] = visualIndex;
            columnCurrentToOriginalMap[visualIndex] = i;
        }
    }

    settings.endGroup();
}

void MainWindow::updateHeaderLabels()
{
    QStringList headerLabels = {"Title", "Year", "Decade", "Resolution", "Aspect Ratio", "Quality", "Size", "Duration", "Language",
                                "Actions", "Rated", "Rating", "Votes", "Director", "Actors", "Writers", "Awards", "Language", "Country", "Box Office", "Plot", "Genre"};

    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
}
