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

    // Get the status text from the current row
    QString statusText = index.model()->data(index.model()->index(index.row(), 13), Qt::DisplayRole).toString().trimmed();

    // Create a copy of the option's palette
    QPalette palette = option->palette;

    if (statusText == "active") {
        option->backgroundBrush = QBrush(activeColor);
    } else if (statusText == "completed") {
        option->backgroundBrush = QBrush(completedColor);
    }

    // Set the modified palette to the option
    option->palette = palette;
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