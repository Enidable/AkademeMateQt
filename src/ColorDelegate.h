#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>
#include <QtGui/QColor>
#include <QtGui/QBrush>
#include <QtCore/QSettings>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>

class ColorDelegate : public QStyledItemDelegate {
public:
    ColorDelegate(QObject* parent = nullptr);

    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;

    // Functions to get and set colors
    QColor getCompletedColor() const;
    void setCompletedColor(const QColor& color);

    QColor getActiveColor() const;
    void setActiveColor(const QColor& color);

private:
    // Default colors
    QColor completedColor;
    QColor activeColor;

    // Function to load and save color preferences
    void loadColorPreferences();
    void saveColorPreferences() const;
};

#endif // COLORDELEGATE_H