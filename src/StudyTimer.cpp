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
    if (isPaused) {
        // If paused, resume from the paused time
        TimePoint now = Clock::now();
        Duration pausedDuration = now - pauseTime;
        startTime += pausedDuration;
        pauseTime = TimePoint();
        isPaused = false;
    } 
    else if (timer->isActive()) {
        return;  // Timer is already running, do nothing
    } else {
        // If not paused, start as usual
        startTime = Clock::now();
        pauseTime = TimePoint();
        timer->start(1000);
    }

    timer->start(1000);  // Start the timer interval to 1 second
}

void StudyTimer::pause() {
    if (!isPaused && startTime != TimePoint()) {
        pauseTime = Clock::now();
        isPaused = true;
        timer->stop();
    }
}

void StudyTimer::reset() {
    timer->stop();  // Stop the timer first

    // Reset timer values
    startTime = TimePoint();
    pauseTime = TimePoint();
    elapsedTime = Duration::zero();
    isPaused = false;  // Reset the paused state
    laps.clear();

    // Emit signals
    emit timerUpdated(0);
    emit lapsUpdated(displayLaps());
}

void StudyTimer::lap() {
    if (startTime != TimePoint() && !isPaused) {
        TimePoint lapTime = Clock::now();

        if (laps.empty()) {
            Duration lapDuration = lapTime - startTime;
            laps.push_back(std::chrono::duration_cast<std::chrono::seconds>(lapDuration));
            emit lapsUpdated(displayLaps());
        } else {
            // Calculate the lap duration by subtracting the sum of previous laps from lapTime
            Duration lapDuration = lapTime - (startTime + std::accumulate(laps.begin(), laps.end(), Duration::zero()));
            laps.push_back(std::chrono::duration_cast<std::chrono::seconds>(lapDuration));
            emit lapsUpdated(displayLaps());
        }
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