#include "module.h"

Module::Module(): shortName(""), longName(""), semester(0), startDate(QDate()), endDate(QDate()), timeMin(0), note(0.0), ects(0), sok(false), tok(false), ass(0), lab(false), statusID(1)
{
}


Module::Module(const QString& shortName, const QString& longName, int semester, const QDate& startDate, const QDate& endDate, int timeMin, double note, int ects, bool sok, bool tok, int ass, bool lab, int statusID)
    : shortName(shortName), longName(longName), semester(semester), startDate(startDate), endDate(endDate), timeMin(timeMin), note(note), ects(ects), sok(sok), tok(tok), ass(ass), lab(lab), statusID(statusID)
{}

QString Module::getShortName() const { return shortName; }
QString Module::getLongName() const { return longName; }
int Module::getSemester() const { return semester; }
QDate Module::getStartDate() const { return startDate; }
QDate Module::getEndDate() const { return endDate; }
int Module::getTimeMin() const { return timeMin; }
double Module::getNote() const { return note; }
int Module::getEcts() const { return ects; }
bool Module::getSok() const { return sok; }
bool Module::getTok() const { return tok; }
int Module::getAss() const { return ass; }
bool Module::getLab() const { return lab; }

// use getStatusID in DbInputWindow instead if needed
// QString Module::getStatusID() const { return status; } 

void Module::setShortName(const QString& shortName) { this->shortName = shortName; }
void Module::setLongName(const QString& longName) { this->longName = longName; }
void Module::setSemester(int semester) { this->semester = semester; }
void Module::setStartDate(const QDate& startDate) { this->startDate = startDate; }
void Module::setEndDate(const QDate& endDate) { this->endDate = endDate; }
void Module::setTimeMin(int timeMin) { this->timeMin = timeMin; }
void Module::setNote(double note) { this->note = note; }
void Module::setEcts(int ects) { this->ects = ects; }
void Module::setSok(bool sok) { this->sok = sok; }
void Module::setTok(bool tok) { this->tok = tok; }
void Module::setAss(int ass) { this->ass = ass; }
void Module::setLab(bool lab) { this->lab = lab; }
void Module::setStatus(int statusID) { this->statusID = statusID; }
