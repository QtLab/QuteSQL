# QuteSQL

## Todo

- Create wrapper QSqlDatabase class (with functions like executeQuery(), getName(), etc)
- Create log tab that shows history of all SQL commands executed.
- Store settings in SQLite Database (Storing using QSettings is inconvenient with arrays.)
- Export selected
- SQL Splitter

## SQLite Database Schema

databases
id | host | port | username | password | ssh_hostname | ssh_port

queries
id | database_id | type (SAVED/HISTORY) | created_at | query

relations
id | database_id | table | column | foreign_table | foreign_column | display_column*

* This field is important and is the reason we need to store this in the database (as opposed to detecting it each time upon opening the database)
