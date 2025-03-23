#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"
#include "database_manager.h"

void create_database_header(struct database_header_t *header, int fd) {
  header->version = htons(0x1);
  header->count = htons(0);
  header->magic = htonl(HEADER_MAGIC);
  header->filesize = htonl(sizeof(struct database_header_t));

  lseek(fd, 0, SEEK_SET);

  write(fd, header, sizeof(struct database_header_t));
}

int validate_database_header(struct database_header_t *header, int fd) {
  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);

  if (header->version != 1) {
    printf("Database header mismatch\n");

    return STATUS_ERROR;
  }

  if (header->magic != HEADER_MAGIC) {
    printf("Database header magic mismatch\n");

    return STATUS_ERROR;
  }

  struct stat db_stat = {0};
  fstat(fd, &db_stat);

  if (header->filesize != db_stat.st_size) {
    printf("Database filesize mistmatch\n");

    return STATUS_ERROR;
  }

  return STATUS_SUCCESS;
}

int open_database_header(struct database_header_t **header_out, int fd) {
  struct database_header_t *header =
      calloc(1, sizeof(struct database_header_t));

  if (header == NULL) {
    return STATUS_ERROR;
  }

  if (read(fd, header, sizeof(struct database_header_t)) !=
      sizeof(struct database_header_t)) {
    create_database_header(header, fd);
  }

  if (validate_database_header(header, fd) == STATUS_ERROR) {
    printf("Database file header did not pass validation.\n");
    free(header);

    return STATUS_ERROR;
  }

  *header_out = header;

  return STATUS_SUCCESS;
}

int init_database_manager(struct database_manager_t **manager_out, int fd) {
  struct database_manager_t *manager =
      calloc(1, sizeof(struct database_manager_t));

  if (manager == NULL) {
    return STATUS_ERROR;
  }

  manager->fd = fd;
  manager->header = NULL;

  if (open_database_header(&manager->header, manager->fd) == STATUS_ERROR) {
    printf("Could not create database header\n");
    free(manager);

    return STATUS_ERROR;
  }

  *manager_out = manager;

  return STATUS_SUCCESS;
}

void show_database_manager(struct database_manager_t *manager) {
  printf("-- Database manger\n");
  printf("-- FD: %d\n", manager->fd);
  printf("---- Version:\t%d\n", manager->header->version);
  printf("---- Magic:\t%x\n", manager->header->magic);
  printf("---- Count:\t%d entries\n", manager->header->count);
  printf("---- Filesize:\t%dB\n\n", manager->header->filesize);
}

void clean_up_database_manager(struct database_manager_t *manager) {
  if (manager->header != NULL) {
    free(manager->header);
    manager->header = NULL;
  }

  if (manager->fd != STATUS_ERROR) {
    close(manager->fd);
    manager->fd = STATUS_ERROR;
  }

  free(manager);
  manager = NULL;
}
