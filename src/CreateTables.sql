CREATE TABLE Module (
  ModuleID INTEGER PRIMARY KEY AUTOINCREMENT,
  Module TEXT,
  Abbreviation TEXT KEY,
  Semester INTEGER,
  Start DATE,
  End DATE,
  Minutes INTEGER,
  Note TEXT,
  SOK BOOLEAN,
  TOK BOOLEAN,
  ASS INTEGER,
  LAB BOOLEAN,
  ECTS INTEGER,
  StatusID INTEGER,
  FOREIGN KEY(StatusID) REFERENCES Status(StatusID)
);

CREATE TABLE TimeCategory (
  TimeCatID INTEGER PRIMARY KEY AUTOINCREMENT,
  Category TEXT
);

CREATE TABLE Status (
  StatusID INTEGER PRIMARY KEY AUTOINCREMENT,
  Status TEXT
);

CREATE TABLE PerformanceType (
  PerfTypeID INTEGER PRIMARY KEY AUTOINCREMENT,
  PerfType TEXT
);

CREATE TABLE PerformanceAssessment (
  PerfAssID INTEGER PRIMARY KEY AUTOINCREMENT,
  PerfTypeID INTEGER,
  PerfAssName TEXT,
  StatusID INTEGER,
  FOREIGN KEY (PerfTypeID) REFERENCES PerformanceType(PerfTypeID),
  FOREIGN KEY (StatusID) REFERENCES Status(StatusID)
);

CREATE TABLE Times (
  TimeID INTEGER PRIMARY KEY AUTOINCREMENT,
  ModuleID INTEGER,
  TimeCatID INTEGER,
  PerfAssID INTEGER,
  Investment INTEGER,
  Estimation INTEGER,
  FOREIGN KEY (ModuleID) REFERENCES Module(ModuleID),
  FOREIGN KEY (PerfAssID) REFERENCES PerformanceAssessment(PerfAssID),
  FOREIGN KEY (TimeCatID) REFERENCES TimeCategory(TimeCatID)
);

INSERT INTO Status (StatusID, Status) VALUES (1, 'Inactive');
INSERT INTO Status (StatusID, Status) VALUES (2, 'Active');
INSERT INTO Status (StatusID, Status) VALUES (3, 'Completed');

INSERT INTO PerformanceType (PerfTypeID, PerfType) VALUES (1, 'Exam');
INSERT INTO PerformanceType (PerfTypeID, PerfType) VALUES (2, 'Assignment');
INSERT INTO PerformanceType (PerfTypeID, PerfType) VALUES (3, 'Laboratory Report');

INSERT INTO TimeCategory (Category) VALUES ('Research');
INSERT INTO TimeCategory (Category) VALUES ('Application');
INSERT INTO TimeCategory (Category) VALUES ('Write and Create');
INSERT INTO TimeCategory (Category) VALUES ('Proofreading');
INSERT INTO TimeCategory (Category) VALUES ('Studying');
INSERT INTO TimeCategory (Category) VALUES ('Study letters');
INSERT INTO TimeCategory (Category) VALUES ('Practice');
INSERT INTO TimeCategory (Category) VALUES ('Studying for Exam');
INSERT INTO TimeCategory (Category) VALUES ('Exam');
INSERT INTO TimeCategory (Category) VALUES ('Total time');
