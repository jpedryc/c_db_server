#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#define HEADER_MAGIC 0x4c4c4144

struct database_header_t {
  unsigned int magic; // A fixed magic number used for file type identification
  unsigned short version; // A fixed version number used for maintenance
  unsigned short count;   // Count of all database entries
  unsigned int filesize;  // Database files filesize used for validation
};

struct database_manager_t {
  int fd; // File descriptor for the opened database file
  struct database_header_t
      *header; // Pointer to the in-memory database header structure
};

/**
 * Initializes the database manager
 *
 * It will read the header of the database file pointed by "fd" or, if not
 * existing, initialize a new header and save to the database file. The manager
 * will be allocated and initialized with the file descriptor and header
 * information.
 *
 * @param manager Double pointer of the manager to be initialized
 * @param fd      File descriptor of the opened database file
 * @return        STATUS_SUCCESS on success,
 *                or STATUS_ERROR on failure.
 */
int init_database_manager(struct database_manager_t **manager, int fd);

/**
 * Show database manager information
 *
 * @param manager  Pointer to the manager to be shown
 */
void show_database_manager(struct database_manager_t *manager);

/**
 * Cleans up any dangling manager pointers.
 *
 * Frees the underlying header pointer and clears out the file descriptor
 * to the database file.
 *
 * @param manager  Pointer to the database manager structure
 */
void clean_up_database_manager(struct database_manager_t *manager);

#endif
