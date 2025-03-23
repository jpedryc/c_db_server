#ifndef DATABASE_FILE_H
#define DATABASE_FILE_H

/**
 * Opens an existing database file or creates a new one if it doesn't exist.
 *
 * @return File descriptor to the opened database file on success,
 *         or STATUS_ERROR on failure.
 */
int open_database_file();

#endif
