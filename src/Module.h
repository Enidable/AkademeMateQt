#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QDate>

class Module
{
public:
    // Constructors
    Module();   // Default constructor
    Module(const QString& shortName, const QString& longName, int semester, const QDate& startDate, const QDate& endDate, int timeMin, double note, int ects, bool sok, bool tok, int ass, bool lab, int statusID);

    // Getters
    QString getShortName() const;
    QString getLongName() const;
    int getSemester() const;
    QDate getStartDate() const;
    QDate getEndDate() const;
    int getTimeMin() const;
    double getNote() const;
    int getEcts() const;
    bool getSok() const;
    bool getTok() const;
    int getAss() const;
    bool getLab() const;
    int getStatusID() const { return statusID; }

    // Setters
    void setShortName(const QString& shortName);
    void setLongName(const QString& longName);
    void setSemester(int semester);
    void setStartDate(const QDate& startDate);
    void setEndDate(const QDate& endDate);
    void setTimeMin(int timeMin);
    void setNote(double note);
    void setEcts(int ects);
    void setSok(bool sok);
    void setTok(bool tok);
    void setAss(int ass);
    void setLab(bool lab);
    void setStatus(int statusID);

private:
    QString shortName;
    QString longName;
    int semester;
    QDate startDate;
    QDate endDate;
    int timeMin;
    double note;
    int ects;
    bool sok;
    bool tok;
    int ass;
    bool lab;
    int statusID;
};

#endif // MODULE_H
