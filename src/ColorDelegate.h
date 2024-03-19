#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>
#include <QtGui/QColor>
#include <QtGui/QBrush>
#include <QtGui/QPalette>
#include <QtCore/QSettings>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>


/**
 * @brief The ColorDelegate class provides a custom item delegate for coloring rows based on module status in the MainTable.
 */
class ColorDelegate : public QStyledItemDelegate {
public:
    ColorDelegate(QObject* parent = nullptr);

/**
 * @brief Initializes the style option for the given index based on the status of the item.
 * 
 * This function customizes the appearance of the modules based on their status.
 * It retrieves the status text from the TableView at the specified index and sets the background color
 * of the item accordingly. If the status is "active", the background color is set to the "active color" (purple);
 * if the status is "completed", the background color is set to the "completed color"(green).
 * 
 * @param option A pointer to the style option to initialize.
 * @param index The model index representing the item for which the style option is being initialized.
 */
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;

private:
    // Default colors
    QColor completedColor;
    QColor activeColor;

    // Function to load and save color preferences
    void loadColorPreferences();
    void saveColorPreferences() const;
};

#endif // COLORDELEGATE_H