#include "MovieDataRefresher.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QStatusBar>

MovieDataRefresher::MovieDataRefresher(MovieDB *db, OmdbClient *client, QStatusBar *statusBar, QWidget *parent)
    : QObject(parent), movieDb(db), omdbClient(client), statusBar(statusBar), parentWidget(parent)
{
}

bool MovieDataRefresher::showOverwriteDialog(const QString &title, const Movie &existingMovie, QWidget *parentWidget)
{
    QDialog dialog(parentWidget);
    dialog.setWindowTitle("Overwrite Existing Movie Data?");
    dialog.setMinimumWidth(600);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel(QString("The movie '<b>%1</b>' already has data in the database. Do you want to overwrite it with fresh data from IMDb?").arg(title));
    label->setWordWrap(true);
    layout->addWidget(label);

    QLabel *currentDataLabel = new QLabel("<br><b>Current data in database:</b>");
    layout->addWidget(currentDataLabel);

    QString movieInfo = "<table style='border-collapse: collapse; width: 100%;'>";
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Title:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.title);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Year:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.year);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Rated:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.rated);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>IMDb Rating:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.imdbRating);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>IMDb Votes:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.imdbVotes);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Director:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.director);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Actors:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.actors);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Writer:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.writer);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Awards:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.awards);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Language:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.language);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Country:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.country);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Box Office:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.boxOffice);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Genre:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.genre);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>Plot:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.plot);
    movieInfo += QString("<tr><td style='padding: 4px; border: 1px solid #ccc;'><b>IMDb ID:</b></td><td style='padding: 4px; border: 1px solid #ccc;'>%1</td></tr>").arg(existingMovie.imdbID);
    movieInfo += "</table>";

    QTextEdit *textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    textEdit->setHtml(movieInfo);
    textEdit->setMaximumHeight(300);
    layout->addWidget(textEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    int result = dialog.exec();

    if (result == QDialog::Accepted)
    {
        // User confirmed - delete the existing entry
        movieDb->deleteMovie(title);
        if (statusBar)
            statusBar->showMessage(QString("Refreshing data for '%1' from IMDb...").arg(title), 3000);
        return true;
    }
    else
    {
        if (statusBar)
            statusBar->showMessage("Refresh cancelled.", 2000);
        return false;
    }
}
