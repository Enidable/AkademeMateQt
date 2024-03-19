#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include <QList>
#include <QDebug>
#include <QVector>
#include <QString>

/**
 * @brief The Calculations class provides static methods for various calculations.
 *
 * This class is currently unused and serves as a rough draft for future implementation
 * into the application. It includes methods for converting minutes to hours, calculating
 * average time, total time, relative progress, and estimated time.
 * Most likely the calculations will be realised with SQL functions and not with this type of code.
 */
class Calculations {
public:
    static double convertMinutesToHours(int minutes);
    static double calculateAverageTime(const QVector<double>& times, const QVector<QString>& statuses);
    static int calculateTotalTime(const QVector<int>& times);
    static double calculateRelativeProgress(double investedTime, double estimatedTime);
    static double calculateEstimatedTime(const QVector<double>& assignmentTimes, const QVector<double>& examTimes);
};

#endif // CALCULATIONS_H