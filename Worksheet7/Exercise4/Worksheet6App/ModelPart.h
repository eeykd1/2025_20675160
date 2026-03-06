#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

class ModelPart {
public:
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);
    ~ModelPart();

    void appendChild(ModelPart* item);
    ModelPart* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void set(int column, const QVariant& value);
    ModelPart* parentItem();
    int row() const;

    void setColour(unsigned char R, unsigned char G, unsigned char B);
    unsigned char getColourR() const { return colourR; }
    unsigned char getColourG() const { return colourG; }
    unsigned char getColourB() const { return colourB; }

    void setVisible(bool isVisible);
    bool visible();
    void loadSTL(QString fileName);

private:
    QList<ModelPart*> m_childItems;
    QList<QVariant> m_itemData;
    ModelPart* m_parentItem;

    unsigned char colourR = 255;
    unsigned char colourG = 255;
    unsigned char colourB = 255;
    bool isVisible = false;
};

#endif
