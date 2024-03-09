#include "ColorDelegate.h"

ColorDelegate::ColorDelegate(QObject* parent) : QStyledItemDelegate(parent) {
    // Set default colors
    completedColor = QColor("#d9ead3");  // Light green
    activeColor = QColor("#feefff");     // Light purple

    // Load color preferences
    loadColorPreferences();
}

void ColorDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const {
    QStyledItemDelegate::initStyleOption(option, index);

    // Status column has to be the last column of the table for the program to select the right column
QString status = index.model()->data(index.model()->index(index.row(), index.model()->columnCount() - 1), Qt::UserRole + 1).toString();
/* QString statusColumnName = "Status";
int statusColumnIndex = index.model()->query().record().indexOf(statusColumnName);
QString status = index.model()->data(index.model()->index(index.row(), statusColumnIndex), Qt::DisplayRole).toString(); */

 if (status == "completed") {
        option->backgroundBrush = QBrush(completedColor);
        option->palette.setColor(QPalette::Text, Qt::black);  // Set text color to black
    } else if (status == "active") {
        option->backgroundBrush = QBrush(activeColor);
        option->palette.setColor(QPalette::Text, Qt::black);  // Set text color to black
    }
    // Possibly add more conditions for other statuses or colors
}


QColor ColorDelegate::getCompletedColor() const {
    return completedColor;
}
// The color should be set with the hexadecimal representation
void ColorDelegate::setCompletedColor(const QColor& color) {
    completedColor = color;
    saveColorPreferences();
}

QColor ColorDelegate::getActiveColor() const {
    return activeColor;
}
// The color should be set with the hexadecimal representation
void ColorDelegate::setActiveColor(const QColor& color) {
    activeColor = color;
    saveColorPreferences();
}

// Load program with the previous color preference
void ColorDelegate::loadColorPreferences() {
    QSettings settings;

    // Load completed color preference
    completedColor = settings.value("CompletedColor", completedColor).value<QColor>();

    // Load active color preference
    activeColor = settings.value("ActiveColor", activeColor).value<QColor>();
}

void ColorDelegate::saveColorPreferences() const {
    QSettings settings;

    // Save completed color preference
    settings.setValue("CompletedColor", completedColor);

    // Save active color preference
    settings.setValue("ActiveColor", activeColor);
}