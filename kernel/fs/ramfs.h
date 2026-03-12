#ifndef RAMFS_H
#define RAMFS_H

void ramfs_init();
int ramfs_mkdir(const char* name);
void ramfs_ls();
int ramfs_cd(const char* name);
int ramfs_cf(const char* name);
int ramfs_write(const char* name, const char* content);
void ramfs_open(const char* name);
int ramfs_rm(const char* name);
#endif
