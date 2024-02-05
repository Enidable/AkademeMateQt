#pragma once

#include <chrono>                               // Library for working with time related functions
#include <vector>
#include <QWidget>
#include <QObject>

class QTimer;                                   // Forward declaration of QTimer class

class StudyTimer : public QObject {
    Q_OBJECT

private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using Duration = Clock::duration;

    TimePoint startTime;
    TimePoint pauseTime;
    Duration elapsedTime;
    bool isPaused;                              // Flag to track pause state, previous error exclusion
    std::vector<Duration> laps;

    QTimer* timer;                              // Add QTimer pointer for handling intervals

public:
    explicit StudyTimer(QObject* parent = nullptr);
    ~StudyTimer();

    void displayElapsedTime() const;

    QStringList displayLaps() const;            // Return QStringList

public slots:
    void start();
    void pause();
    void reset();
    void lap();

signals:
    void timerUpdated(int seconds);             // Signal for updating timer display
    void lapsUpdated(const QStringList &laps);  // Signal for updating laps display
};