#ifndef VFS_H
#define VFS_H

#include <stdint.h>

#define VFS_NAME_MAX 32
#define VFS_MAX_CHILDREN 16

typedef enum {
    VFS_FILE,
    VFS_DIRECTORY
} vfs_node_type;

typedef struct vfs_node {
    char name[VFS_NAME_MAX];
    vfs_node_type type;

    struct vfs_node* parent;

    // Directory fields
    struct vfs_node* children[VFS_MAX_CHILDREN];
    uint32_t child_count;

    // File fields
    char* data;
    uint32_t size;

} vfs_node;
extern vfs_node* vfs_root;
extern vfs_node* vfs_current;

#endif

