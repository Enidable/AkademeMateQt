#include "StudyTimer.h"
#include <QTimer>
#include <QCoreApplication>

StudyTimer::StudyTimer(QObject* parent) : QObject(parent), elapsedTime(Duration::zero()), timer(new QTimer(this)) {
    // Connect the timer's timeout signal to update the elapsed time
    connect(timer, &QTimer::timeout, this, [this]() {
        elapsedTime += std::chrono::seconds(1);
        emit timerUpdated(static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count()));
    });
}

StudyTimer::~StudyTimer() {
    delete timer;  // Cleanup the timer when the StudyTimer is destroyed
}

void StudyTimer::start() {
    startTime = Clock::now();
    pauseTime = TimePoint();
    timer->start(1000);  // Set the timer interval to 1 second, without this the timer does not run!
}

void StudyTimer::pause() {
    if (startTime != TimePoint()) {
        pauseTime = Clock::now();
        elapsedTime += pauseTime - startTime;
        startTime = TimePoint();
        timer->stop();  // Stop the timer when paused
    }
}

void StudyTimer::reset() {
    startTime = TimePoint();
    pauseTime = TimePoint();
    elapsedTime = Duration::zero();
    laps.clear();
    timer->stop();  // Stop the timer when resetting
    emit timerUpdated(0);  // Reset the displayed timer
    emit lapsUpdated(displayLaps());
}

void StudyTimer::lap() {
    if (startTime != TimePoint()) {
        TimePoint lapTime = Clock::now();
        Duration lapDuration = lapTime - startTime;
        laps.push_back(std::chrono::duration_cast<std::chrono::seconds>(lapDuration));
        emit lapsUpdated(displayLaps());
    } 
}

void StudyTimer::displayElapsedTime() const {
    // Function unchanged, as it doesn't directly relate to QTimer
}

QStringList StudyTimer::displayLaps() const {
    QStringList lapList;
    for (size_t i = 0; i < laps.size(); ++i) {
        auto lapDuration = laps[i];
        auto hours = std::chrono::duration_cast<std::chrono::hours>(lapDuration);
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(lapDuration % std::chrono::hours(1));
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(lapDuration % std::chrono::minutes(1));

        QString lapString = QString("%1:%2:%3")
                                                .arg(hours.count(), 2, 10, QChar('0'))
                                                .arg(minutes.count(), 2, 10, QChar('0'))
                                                .arg(seconds.count(), 2, 10, QChar('0'));
        lapList.append(lapString);
    }
    return lapList;
}