#ifndef INFRASTRUCTURE_DATABASE_H
#define INFRASTRUCTURE_DATABASE_H

#include <QObject>
#include <QSqlDatabase>

namespace Infrastructure
{

    class Database : public QObject
    {
        Q_OBJECT

    public:
        explicit Database(const QString &dbPath, QObject *parent = nullptr);
        ~Database() override;

        bool open();
        bool isOpen() const;
        QSqlDatabase &connection();

    private:
        bool createTables();
        QSqlDatabase m_db;
        QString m_connectionName;
    };

} // namespace Infrastructure

#endif // INFRASTRUCTURE_DATABASE_H
