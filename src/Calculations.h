#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include <QList>
#include <QDebug>
#include <QVector>
#include <QString>


class Calculations {
public:
    static double convertMinutesToHours(int minutes);
    static double calculateAverageTime(const QVector<double>& times, const QVector<QString>& statuses);
    static int calculateTotalTime(const QVector<int>& times);
    static double calculateRelativeProgress(double investedTime, double estimatedTime);
    static double calculateEstimatedTime(const QVector<double>& assignmentTimes, const QVector<double>& examTimes);
};

#endif // CALCULATIONS_H