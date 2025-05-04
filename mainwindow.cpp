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

    // Initialize database
    movieDb = new MovieDB(this);
    if (!movieDb->init())
    {
        QMessageBox::warning(this, "Database Error", "Failed to initialize movie database");
    }

    // Initialize OMDb client
    omdbClient = new OmdbClient("5af6b86e", movieDb, this);

    // Setup table
    ui->tableWidget->setColumnCount(22);
    ui->tableWidget->setHorizontalHeaderLabels({"Title", "Year", "Decade", "Resolution", "Aspect Ratio", "Quality", "Size", "Duration", "Language",
                                                "Actions","Rated", "Rating", "Votes", "Director", "Actors", "Writers", "Awards", "Language", "Country", "Box Office", "Plot", "Genre"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

    // Initialize combo boxes with "All"
    for (auto combo : {ui->comboBoxDecade, ui->comboBoxAspectRatio, ui->comboBoxQuality})
        addComboBoxItemIfNotExist(combo, "All");

    // Load last folder
    QSettings settings("YourCompany", "VideoBrowserApp");
    QString lastFolder = settings.value("lastFolder").toString();
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", lastFolder);
    if (!folderPath.isEmpty())
    {
        settings.setValue("lastFolder", folderPath);
        processVideos(folderPath);
    }

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

// ===================== Video Processing =====================

void MainWindow::processVideos(const QString &folderPath)
{
    QStringList videoExtensions = {".mp4", ".mkv", ".avi", ".mov", ".flv", ".wmv"};
    int row = 0;

    QSet<QString> decades, aspectRatios, qualities;

    QDirIterator it(folderPath, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString filePath = it.next();
        QFileInfo fileInfo(filePath);
        if (!videoExtensions.contains("." + fileInfo.suffix().toLower()))
            continue;

        QString resolution = getVideoResolution(filePath);
        QString aspectRatio = getAspectRatio(resolution);
        QString folderName = fileInfo.dir().dirName();
        auto [title, year] = parseFolderName(folderName);
        QString decade = getDecade(year);
        QString quality = getVideoQuality(filePath);
        QString duration = getVideoDuration(filePath);
        QString fileSize = getFileSize(filePath);
        QString audioLanguage = getAudioLanguage(filePath);

        decades.insert(decade);
        aspectRatios.insert(aspectRatio);
        qualities.insert(quality);

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

        row++;
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

    auto getFileInfo = [this, index]() -> std::optional<QFileInfo>
    {
        auto *item = ui->tableWidget->item(index.row(), 0);
        if (!item)
            return std::nullopt;

        QString filePath = item->data(FilePathRole).toString();
        if (filePath.isEmpty())
        {
            QMessageBox::warning(this, "Error", "No path information available.");
            return std::nullopt;
        }

        QFileInfo fileInfo(filePath);
        if (!fileInfo.exists())
        {
            QMessageBox::warning(this, "Error", "The file does not exist.");
            return std::nullopt;
        }

        return fileInfo;
    };

    connect(openFolderAction, &QAction::triggered, this, [=]()
            {
        auto fileInfoOpt = getFileInfo();
        if (!fileInfoOpt) return;

        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfoOpt->absolutePath())); });

    connect(renameFolderAction, &QAction::triggered, this, [=]()
            {
        auto fileInfoOpt = getFileInfo();
        if (!fileInfoOpt) return;

        const QFileInfo &fileInfo = *fileInfoOpt;
        QDir parentDir = fileInfo.dir();
        QString currentPath = parentDir.absolutePath();
        QString currentName = parentDir.dirName();
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

        qDebug() << "Current path:" << currentPathNative;
        qDebug() << "New path:" << newPathNative;

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

    // Write headers
    QStringList headers;
    for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
    {
        if (col != 10) // Skip Actions column
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
            if (col == 10)
                continue; // Skip Actions column

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
    QStringList titleList;

    qDebug() << "Fetch button clicked. Reading titles from table.";

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (item)
        {
            const QString title = item->text().trimmed();
            if (!title.isEmpty())
            {
                qDebug() << "Found title in row" << row << ":" << title;
                titleList.append(title);
            }
        }
    }

    if (titleList.isEmpty())
    {
        qDebug() << "No titles found in table.";
        return;
    }

    qDebug() << "Fetching movie data for" << titleList.size() << "titles.";
    for (const auto &title : titleList)
    {
        qDebug() << "Requesting movie:" << title;
        omdbClient->fetchMovie(title);
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
