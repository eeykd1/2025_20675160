#include "ModelPartList.h"
#include "ModelPart.h"

ModelPartList::ModelPartList(const QString& data, QObject* parent) : QAbstractItemModel(parent) {
    rootItem = new ModelPart({ tr("Part"), tr("Visible?") });
}

ModelPartList::~ModelPartList() { delete rootItem; }

int ModelPartList::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

QVariant ModelPartList::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags ModelPartList::flags(const QModelIndex& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant ModelPartList::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

QModelIndex ModelPartList::index(int row, int column, const QModelIndex& parent) const {
    ModelPart* parentItem;
    if (!parent.isValid() || !hasIndex(row, column, parent))
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    ModelPart* childItem = parentItem->child(row);
    if (childItem) return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ModelPartList::parent(const QModelIndex& index) const {
    if (!index.isValid()) return QModelIndex();
    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parentItem = childItem->parentItem();
    if (parentItem == rootItem) return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

int ModelPartList::rowCount(const QModelIndex& parent) const {
    ModelPart* parentItem;
    if (parent.column() > 0) return 0;
    if (!parent.isValid()) parentItem = rootItem;
    else parentItem = static_cast<ModelPart*>(parent.internalPointer());
    return parentItem->childCount();
}

ModelPart* ModelPartList::getRootItem() { return rootItem; }

QModelIndex ModelPartList::appendChild(QModelIndex& parent, const QList<QVariant>& data) {
    ModelPart* parentPart;
    if (parent.isValid()) parentPart = static_cast<ModelPart*>(parent.internalPointer());
    else {
        parentPart = rootItem;
        parent = createIndex(0, 0, rootItem);
    }
    beginInsertRows(parent, rowCount(parent), rowCount(parent));
    ModelPart* childPart = new ModelPart(data, parentPart);
    parentPart->appendChild(childPart);
    QModelIndex child = createIndex(0, 0, childPart);
    endInsertRows();
    return child;
}

bool ModelPartList::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole) return false;
    ModelPart *item = static_cast<ModelPart*>(index.internalPointer());
    if (item) {
        item->set(index.column(), value);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}
