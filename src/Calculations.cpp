#include "calculations.h"

double Calculations::convertMinutesToHours(int minutes) {
    return static_cast<double>(minutes) / 60.0;
}

double Calculations::calculateAverageTime(const QVector<double>& times, const QVector<QString>& statuses) {
    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < times.size(); ++i) {
        if (statuses[i] == "completed") {
            sum += times[i];
            ++count;
        }
    }

    return count > 0 ? sum / count : 0.0;
}

int Calculations::calculateTotalTime(const QVector<int>& times) {
    int total = 0;

    for (int time : times) {
        total += time;
    }

    return total;
}

double Calculations::calculateRelativeProgress(double investedTime, double estimatedTime) {
    return investedTime / estimatedTime;
}

double Calculations::calculateEstimatedTime(const QVector<double>& assignmentTimes, const QVector<double>& examTimes) {
    double assignmentAverage = calculateAverageTime(assignmentTimes, {"completed"});
    double examAverage = calculateAverageTime(examTimes, {"completed"});

    return assignmentAverage + examAverage;
}
