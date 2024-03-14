#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QDate>

class Module
{
public:
    // Constructor
    Module(const QString& shortName, const QString& longName, int semester, const QString& mKA, const QDate& startDate, const QDate& endDate, int timeMin, double note, int ects, bool sok, bool tok, int ass, bool lab, const QString& status)
        : m_shortName(shortName), m_longName(longName), m_semester(semester), m_mKA(mKA), m_startDate(startDate), m_endDate(endDate), m_timeMin(timeMin), m_note(note), m_ects(ects), m_sok(sok), m_tok(tok), m_ass(ass), m_lab(lab), m_status(status)
    {}

    // Getters
    QString getShortName() const { return m_shortName; }
    QString getLongName() const { return m_longName; }
    int getSemester() const { return m_semester; }
    QString getMKA() const { return m_mKA; }                    //REMOVE!!!! / LOOK INTO
    QDate getStartDate() const { return m_startDate; }
    QDate getEndDate() const { return m_endDate; }
    int getTimeMin() const { return m_timeMin; }
    double getNote() const { return m_note; }
    int getEcts() const { return m_ects; }
    bool getSok() const { return m_sok; }
    bool getTok() const { return m_tok; }
    int getAss() const { return m_ass; }
    bool getLab() const { return m_lab; }
    QString getStatus() const { return m_status; }

    // Setters
    void setShortName(const QString& short_name) { m_shortName = short_name; }
    void setLongName(const QString& long_name) { m_longName = long_name; }
    void setSemester(int semester) { m_semester = semester; }
    void setMKA(const QString& mKA) { m_mKA = mKA; }
    void setStartDate(const QDate& start_date) { m_startDate = start_date; }
    void setEndDate(const QDate& end_date) { m_endDate = end_date; }
    void setTimeMin(int time_min) { m_timeMin = time_min; }
    void setNote(double note) { m_note = note; }
    void setECTS(int ects) { m_ects = ects; }
    void setSOK(bool sok) { m_sok = sok; }
    void setTOK(bool tok) { m_tok = tok; }
    void setASS(int ass) { m_ass = ass; }
    void setLAB(bool lab) { m_lab = lab; }
    void setStatus(const QString& status) { m_status = status; }

private:
    QString m_shortName;
    QString m_longName;
    int m_semester;
    QString m_mKA;
    QDate m_startDate;
    QDate m_endDate;
    int m_timeMin;
    double m_note;
    int m_ects;
    bool m_sok;
    bool m_tok;
    int m_ass;
    bool m_lab;
    QString m_status;
};

#endif // MODULE_H