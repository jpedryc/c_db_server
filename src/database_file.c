#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"

int create_new_database_file() {
  int fd = open(DATABASE_FILENAME, O_RDWR | O_CREAT, 0644);

  if (fd == STATUS_ERROR) {
    perror("Could not create a new database file\n");

    return STATUS_ERROR;
  }

  return fd;
}

int open_existing_database_file() {
  int fd = open(DATABASE_FILENAME, O_RDWR, 0644);

  if (fd == STATUS_ERROR) {
    perror("Could not open existing database file\n");

    return STATUS_ERROR;
  }

  return fd;
}

int open_database_file() {
  int fd;

  // Check if database file exists and if not - create a new one
  if (access(DATABASE_FILENAME, R_OK | W_OK) != 0) {
    return create_new_database_file();
  }

  return open_existing_database_file();
}
