#include "MoviesToMoveWindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QProcess>

MoviesToMoveWindow::MoviesToMoveWindow(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("Movies to Move");
    resize(1000, 600);
}

MoviesToMoveWindow::~MoviesToMoveWindow()
{
}

void MoviesToMoveWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Count label
    countLabel = new QLabel("No movies need to be moved", this);
    countLabel->setStyleSheet(
        "font-size: 16px; "
        "font-weight: bold; "
        "padding: 12px; "
        "color: #c9d1d9; "
        "background-color: #161b22; "
        "border-radius: 6px;");
    mainLayout->addWidget(countLabel);

    // Table widget
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels(
        QStringList() << "Title" << "Year" << "Rating" << "Votes"
                      << "Current Location" << "Target Location" << "Action");

    tableWidget->horizontalHeader()->setStretchLastSection(false);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    tableWidget->horizontalHeader()->resizeSection(6, 100);

    tableWidget->setAlternatingRowColors(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->verticalHeader()->setDefaultSectionSize(40);

    mainLayout->addWidget(tableWidget);

    // Close button
    closeButton = new QPushButton("Close", this);
    closeButton->setMinimumWidth(120);
    closeButton->setMinimumHeight(36);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #30363d, stop:1 #21262d);"
        "    color: #c9d1d9;"
        "    border: 1px solid #6e7681;"
        "    border-radius: 6px;"
        "    padding: 8px 16px;"
        "    font-weight: 600;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3a424b, stop:1 #30363d);"
        "    border-color: #8b949e;"
        "}"
        "QPushButton:pressed {"
        "    background: #21262d;"
        "}");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);
}

void MoviesToMoveWindow::setMoviesToMove(const QList<MovieToMove> &movies)
{
    moviesToMove = movies;
    populateTable();

    if (movies.isEmpty())
    {
        countLabel->setText("No movies need to be moved");
    }
    else
    {
        countLabel->setText(QString("%1 movie(s) need to be moved to the correct drive").arg(movies.size()));
    }
}

void MoviesToMoveWindow::populateTable()
{
    tableWidget->setRowCount(0);

    for (int i = 0; i < moviesToMove.size(); ++i)
    {
        const MovieToMove &movie = moviesToMove[i];

        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        // Title
        QTableWidgetItem *titleItem = new QTableWidgetItem(movie.title);
        tableWidget->setItem(row, 0, titleItem);

        // Year
        QTableWidgetItem *yearItem = new QTableWidgetItem(movie.year);
        tableWidget->setItem(row, 1, yearItem);

        // Rating
        QTableWidgetItem *ratingItem = new QTableWidgetItem(movie.imdbRating);
        tableWidget->setItem(row, 2, ratingItem);

        // Votes
        QTableWidgetItem *votesItem = new QTableWidgetItem(movie.imdbVotes);
        tableWidget->setItem(row, 3, votesItem);

        // Current Location
        QTableWidgetItem *currentItem = new QTableWidgetItem(movie.currentPath);
        tableWidget->setItem(row, 4, currentItem);

        // Target Location
        QTableWidgetItem *targetItem = new QTableWidgetItem(movie.targetPath);
        tableWidget->setItem(row, 5, targetItem);

        // Move button
        addMoveButtonToRow(row, movie.currentPath, movie.targetPath);
    }
}

void MoviesToMoveWindow::addMoveButtonToRow(int row, const QString &moviePath, const QString &targetPath)
{
    QPushButton *moveButton = new QPushButton("➡️ Move");
    moveButton->setMinimumHeight(32);
    moveButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #238636, stop:1 #1a7f37);"
        "    color: white;"
        "    border: 1px solid #2ea043;"
        "    border-radius: 6px;"
        "    padding: 6px 16px;"
        "    font-weight: 600;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2ea043, stop:1 #238636);"
        "    border-color: #3fb950;"
        "}"
        "QPushButton:pressed {"
        "    background: #1a7f37;"
        "    box-shadow: inset 0 1px 0 rgba(0, 0, 0, 0.2);"
        "}");

    moveButton->setProperty("moviePath", moviePath);
    moveButton->setProperty("targetPath", targetPath);
    moveButton->setProperty("tableRow", row);

    connect(moveButton, &QPushButton::clicked, this, &MoviesToMoveWindow::onMoveButtonClicked);

    tableWidget->setCellWidget(row, 6, moveButton);
}

void MoviesToMoveWindow::onMoveButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    QString currentMovieFolderPath = button->property("moviePath").toString();
    QString targetParentFolderPath = button->property("targetPath").toString();
    int row = button->property("tableRow").toInt();

    // Get the movie folder name
    QDir currentDir(currentMovieFolderPath);
    QString movieFolderName = currentDir.dirName();

    // Build full target path
    QString fullTargetPath = targetParentFolderPath + "/" + movieFolderName;

    // Confirm move
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Move",
        QString("Move '%1' from:\n%2\n\nTo:\n%3\n\nContinue?")
            .arg(movieFolderName, currentMovieFolderPath, fullTargetPath),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
    {
        return;
    }

    // Create target directory
    QDir targetDir(targetParentFolderPath);
    if (!targetDir.exists())
    {
        if (!targetDir.mkpath("."))
        {
            QMessageBox::critical(this, "Error", "Failed to create target directory.");
            return;
        }
    }

    // Check if destination already exists
    if (QDir(fullTargetPath).exists())
    {
        QMessageBox::StandardButton overwrite = QMessageBox::question(
            this,
            "Folder Exists",
            QString("A folder with this name already exists in:\n%1\n\nDo you want to overwrite it?").arg(fullTargetPath),
            QMessageBox::Yes | QMessageBox::No);

        if (overwrite == QMessageBox::No)
        {
            return;
        }

        // Remove existing folder
        QDir(fullTargetPath).removeRecursively();
    }

    // Try QFile::rename first (works for same drive)
    bool success = false;
    QString errorDetails = "";

    if (QFile::rename(currentMovieFolderPath, fullTargetPath))
    {
        success = true;
    }
    else
    {
        // Use TeraCopy for cross-drive moves
        // TeraCopy command: TeraCopy.exe Move <source> <destination> /SkipAll /Close
        QString source = QDir::toNativeSeparators(currentMovieFolderPath);
        QString dest = QDir::toNativeSeparators(fullTargetPath);

        // Check if TeraCopy is installed
        QString teraCopyPath = "C:\\Program Files\\TeraCopy\\TeraCopy.exe";
        if (!QFile::exists(teraCopyPath))
        {
            // Try alternate location
            teraCopyPath = "C:\\Program Files (x86)\\TeraCopy\\TeraCopy.exe";
            if (!QFile::exists(teraCopyPath))
            {
                // TeraCopy not found, show error
                errorDetails = "TeraCopy not found. Please install TeraCopy or specify the correct path.";
                QMessageBox::critical(this, "TeraCopy Not Found", errorDetails);
                return;
            }
        }

        QStringList args = {
            "Move",
            QString("\"%1\"").arg(source),
            QString("\"%1\"").arg(dest),
            "/SkipAll",
            "/Close"};

        QProcess process;
        process.start(teraCopyPath, args);

        // Wait for TeraCopy to finish (it may take a while for large files)
        if (process.waitForFinished(-1))
        {
            int exitCode = process.exitCode();
            QString output = process.readAllStandardOutput();
            QString errorOutput = process.readAllStandardError();

            // TeraCopy exit code 0 means success
            if (exitCode == 0)
            {
                success = true;
            }
            else
            {
                errorDetails = QString("TeraCopy exit code: %1\n%2\n%3")
                                   .arg(exitCode)
                                   .arg(output)
                                   .arg(errorOutput);
            }
        }
        else
        {
            errorDetails = "TeraCopy process failed to start or finish.";
        }
    }

    if (success)
    {
        QMessageBox::information(this, "Success",
                                 QString("Successfully moved '%1' to:\n%2").arg(movieFolderName, fullTargetPath));

        // Remove from list and table
        moviesToMove.removeAt(row);
        tableWidget->removeRow(row);

        // Update count
        if (moviesToMove.isEmpty())
        {
            countLabel->setText("No movies need to be moved");
        }
        else
        {
            countLabel->setText(QString("%1 movie(s) need to be moved to the correct drive").arg(moviesToMove.size()));
        }

        // Update row property for remaining buttons
        for (int i = row; i < tableWidget->rowCount(); ++i)
        {
            QPushButton *btn = qobject_cast<QPushButton *>(tableWidget->cellWidget(i, 6));
            if (btn)
            {
                btn->setProperty("tableRow", i);
            }
        }
    }
    else
    {
        QString errorMsg = "Failed to move folder. The folder may be in use or you may lack permissions.";
        if (!errorDetails.isEmpty())
        {
            errorMsg += "\n\nDetails:\n" + errorDetails;
        }
        QMessageBox::critical(this, "Error", errorMsg);
    }
}
