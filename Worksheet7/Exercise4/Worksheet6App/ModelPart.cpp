#include "ModelPart.h"

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data), m_parentItem(parent) {}

ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item) {
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row) {
    if (row < 0 || row >= m_childItems.size()) return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const { return m_childItems.count(); }
int ModelPart::columnCount() const { return m_itemData.count(); }

QVariant ModelPart::data(int column) const {
    if (column < 0 || column >= m_itemData.size()) return QVariant();
    return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant& value) {
    if (column < 0 || column >= m_itemData.size()) return;
    m_itemData.replace(column, value);
}

ModelPart* ModelPart::parentItem() { return m_parentItem; }

int ModelPart::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(unsigned char R, unsigned char G, unsigned char B) {
    colourR = R;
    colourG = G;
    colourB = B;
}

void ModelPart::setVisible(bool visible) { isVisible = visible; }
bool ModelPart::visible() { return isVisible; }
void ModelPart::loadSTL(QString fileName) { }
