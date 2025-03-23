#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "database_file.h"
#include "database_manager.h"

int init(struct database_manager_t **manager) {
  int fd = open_database_file();

  if (fd == STATUS_ERROR) {
    printf("An error occurred while opening the database file %s\n",
           DATABASE_FILENAME);

    return STATUS_ERROR;
  }

  if (init_database_manager(manager, fd) == STATUS_ERROR) {
    printf("An error occurred while creating the database manager\n");

    return STATUS_ERROR;
  }

  if (*manager == NULL) {
    printf("An error occurred while creating the database manager\n");

    return STATUS_ERROR;
  }

  return STATUS_SUCCESS;
}

void clean_up(struct database_manager_t *manager) {
  if (manager != NULL) {
    clean_up_database_manager(manager);
  }
}

int main(int argc, char *argv[]) {
  struct database_manager_t *manager = NULL;

  if (init(&manager) == STATUS_ERROR) {
    clean_up(manager);

    return STATUS_ERROR;
  }

  show_database_manager(manager);

  clean_up(manager);

  return STATUS_SUCCESS;
}
