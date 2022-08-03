#pragma once

#include <QAbstractTableModel>
#include <json.hpp>

class KVTable : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit KVTable(QObject *parent = nullptr);

	// Header:
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	nlohmann::json table_data = {};
	std::map<std::string,std::string> updated_map = {};

private:

};
