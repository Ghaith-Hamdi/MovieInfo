#include "MovesToMoveDialog.h"
#include "ui_MovesToMoveDialog.h"
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QProcess>
#include <QDir>

namespace UI
{

    MovesToMoveDialog::MovesToMoveDialog(QWidget *parent)
        : QDialog(parent), ui(new Ui::MovesToMoveDialog)
    {
        ui->setupUi(this);
        ui->moviesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        connect(ui->moveAllBtn, &QPushButton::clicked, this, &MovesToMoveDialog::onMoveAllClicked);
        connect(ui->closeBtn, &QPushButton::clicked, this, &QDialog::close);
    }

    MovesToMoveDialog::~MovesToMoveDialog()
    {
        delete ui;
    }

    void MovesToMoveDialog::setMovies(const QList<Core::MoveRequest> &movies)
    {
        m_movies = movies;
        ui->countLabel->setText(QString("Found %1 movie(s) that need to be moved").arg(movies.size()));
        populateTable();
    }

    void MovesToMoveDialog::populateTable()
    {
        ui->moviesTable->setRowCount(m_movies.size());

        for (int i = 0; i < m_movies.size(); ++i)
        {
            const auto &m = m_movies[i];
            ui->moviesTable->setItem(i, 0, new QTableWidgetItem(m.title));
            ui->moviesTable->setItem(i, 1, new QTableWidgetItem(m.year));
            ui->moviesTable->setItem(i, 2, new QTableWidgetItem(m.imdbRating));
            ui->moviesTable->setItem(i, 3, new QTableWidgetItem(m.imdbVotes));
            ui->moviesTable->setItem(i, 4, new QTableWidgetItem(m.currentPath));
            ui->moviesTable->setItem(i, 5, new QTableWidgetItem(m.targetDrive + ":"));

            QPushButton *moveBtn = new QPushButton("Move", this);
            moveBtn->setProperty("row", i);
            connect(moveBtn, &QPushButton::clicked, this, &MovesToMoveDialog::onMoveClicked);
            ui->moviesTable->setCellWidget(i, 6, moveBtn);
        }
    }

    void MovesToMoveDialog::onMoveClicked()
    {
        QPushButton *btn = qobject_cast<QPushButton *>(sender());
        if (!btn)
            return;

        int row = btn->property("row").toInt();
        if (row < 0 || row >= m_movies.size())
            return;

        const auto &m = m_movies[row];
        emit moveRequested(m.currentPath, m.targetPath);
    }

    void MovesToMoveDialog::onMoveAllClicked()
    {
        if (m_movies.isEmpty())
            return;

        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Confirm Move All",
            QString("Move all %1 movies using TeraCopy?").arg(m_movies.size()),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (reply == QMessageBox::Yes)
            emit moveAllWithTeraCopy(m_movies);
    }

} // namespace UI
