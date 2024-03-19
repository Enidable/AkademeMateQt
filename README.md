# Branch "Redesign-Database"
See [Issue #15](https://github.com/Enidable/AkademeMateQt/issues/15)

Cleanup note:
Branch already merged. Left for Documentation.
Setup for standard AkademMate database can be created from file data/CreateTables.sql
## Database structure:

```mermaid
erDiagram
  Module ||--|{ Times : ""
  Module ||--|{ Status : ""
  Times}|--|| TimeCategory : ""
  
  PerformanceType }|--|| PerformanceAssessment : ""
  Status }|--|{ PerformanceAssessment : ""
  
  
  PerformanceAssessment ||--|{ Times : ""

  Module {
    INTEGER ModuleID PK
    TEXT Module
    TEXT Abbreviation
    INTEGER Semester
    DATE Start
    DATE End
    INTEGER Minutes
    INTEGER Note
    BOOL SOK
    BOOL TOK
    INTEGER ASS
    BOOL LAB
    INTEGER ECTS
    INTEGER StatusID FK
  }

  Status {
    INTEGER StatusID PK
    TEXT Status
  }

  PerformanceType {
    INTEGER PerfTypeID PK
    TEXT PerfType
  }

  PerformanceAssessment {
    INTEGER PerfAssID PK
    INTEGER PerfTypeID FK
    TEXT PerfAssName
    INTEGER StatusID FK
  }

  TimeCategory {
    INTEGER TimeCatID PK
    TEXT Category
  }

  Times {
    INTEGER TimeID PK
    INTEGER ModuleID FK
    INTEGER TimeCatID FK
    INTEGER PerfAssID FK
    INTEGER Investment
    INTEGER Estimation
  }
```
