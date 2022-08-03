#include "kvtable.h"
#include "cnet_utility.h"

KVTable::KVTable(QObject *parent)
	: QAbstractTableModel(parent)
{
}

QVariant KVTable::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		if (section == 0) {
			return QString("Key");
		} else if (section == 1) {
			return QString("Value");
		} else if (section == 2) {
			return QString("Updated by");
		}
	}
	return QVariant();
}

int KVTable::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return this->table_data.size();
}

int KVTable::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	return 3;
}

QVariant KVTable::data(const QModelIndex &index, int role) const {
	if (!index.isValid() || role != Qt::DisplayRole) {
		return QVariant();
	}

	int i=0;
	std::string val;
	for(auto &item : this->table_data.items()) {
		if(i == index.row()) {
			if(index.column() == 0) {
				val = item.key();
			} else if(index.column() == 1) {
				val = item.value();
			} else if(index.column() == 2) {
				if(this->updated_map.find(item.key()) != this->updated_map.end()) {
					val = this->updated_map.at(item.key());
				} else {
					val = "-";
				}
			}
			return QString::fromStdString(val);
		}
		i++;
	}
	return QString("Error");
}
