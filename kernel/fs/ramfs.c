#include "../memory/heap.h"
#include "vfs.h"

extern vfs_node* vfs_root;
extern vfs_node* vfs_current;

static vfs_node* ramfs_create_node(const char* name, vfs_node_type type) {
    vfs_node* node = (vfs_node*) kmalloc(sizeof(vfs_node));

    // Clear memory
    for (int i = 0; i < sizeof(vfs_node); i++) {
        ((char*)node)[i] = 0;
    }

    // Copy name
    int i = 0;
    while (name[i] && i < VFS_NAME_MAX - 1) {
        node->name[i] = name[i];
        i++;
    }
    node->name[i] = 0;

    node->type = type;
    node->child_count = 0;
    node->parent = 0;
    node->data = 0;
    node->size = 0;

    return node;
}

void ramfs_init() {
    vfs_root = ramfs_create_node("/", VFS_DIRECTORY);
    vfs_current = vfs_root;
}

int ramfs_add_child(vfs_node* parent, vfs_node* child) {
    if (parent->type != VFS_DIRECTORY)
        return -1;

    if (parent->child_count >= VFS_MAX_CHILDREN)
        return -2;

    parent->children[parent->child_count++] = child;
    child->parent = parent;

    return 0;
}


int ramfs_mkdir(const char* name) {

    for (uint32_t i = 0; i < vfs_current->child_count; i++) {

        char* existing = vfs_current->children[i]->name;

        int j = 0;
        int equal = 1;

        while (name[j] || existing[j]) {
            if (name[j] != existing[j]) {
                equal = 0;
                break;
            }
            j++;
        }

        if (equal) {
            return -1;  // directory already exists
        }
    }

    vfs_node* new_dir = ramfs_create_node(name, VFS_DIRECTORY);
    return ramfs_add_child(vfs_current, new_dir);
}

void ramfs_ls() {

    for (uint32_t i = 0; i < vfs_current->child_count; i++) {

        vfs_node* node = vfs_current->children[i];

        if (node->type == VFS_DIRECTORY)
            print("[DIR] ");
        else
            print("[FILE] ");

        print(node->name);
        print("\n");
    }
}

int ramfs_cd(const char* name) {

    // handle ".."
    if (name[0] == '.' && name[1] == '.' && name[2] == 0) {
        if (vfs_current->parent)
            vfs_current = vfs_current->parent;
        return 0;
    }

    for (uint32_t i = 0; i < vfs_current->child_count; i++) {

        vfs_node* node = vfs_current->children[i];

        if (node->type != VFS_DIRECTORY)
            continue;

        int j = 0;
        int equal = 1;

        while (name[j] || node->name[j]) {
            if (name[j] != node->name[j]) {
                equal = 0;
                break;
            }
            j++;
        }

        if (equal) {
            vfs_current = node;
            return 0;
        }
    }

    return -1; // not found
}

int ramfs_cf(const char* name) {

    // check duplicate
    for (uint32_t i = 0; i < vfs_current->child_count; i++) {

        vfs_node* node = vfs_current->children[i];

        int j = 0;
        int equal = 1;

        while (name[j] || node->name[j]) {
            if (name[j] != node->name[j]) {
                equal = 0;
                break;
            }
            j++;
        }

        if (equal)
            return -1; // already exists
    }

    vfs_node* file = ramfs_create_node(name, VFS_FILE);
    return ramfs_add_child(vfs_current, file);
}

int ramfs_write(const char* name, const char* content) {

    for (uint32_t i = 0; i < vfs_current->child_count; i++) {

        vfs_node* node = vfs_current->children[i];

        if (node->type != VFS_FILE)
            continue;

        int j = 0;
        int equal = 1;

        while (name[j] || node->name[j]) {
            if (name[j] != node->name[j]) {
                equal = 0;
                break;
            }
            j++;
        }

        if (equal) {

            uint32_t len = 0;
            while (content[len]) len++;

            node->data = (char*) kmalloc(len + 1);

            for (uint32_t k = 0; k < len; k++)
                node->data[k] = content[k];

            node->data[len] = 0;
            node->size = len;

            return 0;
        }
    }

    return -1;
}

void ramfs_open(const char* name) {

    for (uint32_t i = 0; i < vfs_current->child_count; i++) {

        vfs_node* node = vfs_current->children[i];

        if (node->type != VFS_FILE)
            continue;

        int j = 0;
        int equal = 1;

        while (name[j] || node->name[j]) {
            if (name[j] != node->name[j]) {
                equal = 0;
                break;
            }
            j++;
        }

        if (equal) {

            if (node->data)
                print(node->data);

            print("\n");
            return;
        }
    }

    print("File not found\n");
}

int ramfs_rm(const char* name) {

    for (uint32_t i = 0; i < vfs_current->child_count; i++) {

        vfs_node* node = vfs_current->children[i];

        int j = 0;
        int equal = 1;

        while (name[j] || node->name[j]) {
            if (name[j] != node->name[j]) {
                equal = 0;
                break;
            }
            j++;
        }

        if (equal) {

            // shift children left
            for (uint32_t k = i; k < vfs_current->child_count - 1; k++)
                vfs_current->children[k] = vfs_current->children[k + 1];

            vfs_current->child_count--;

            return 0;
        }
    }

    return -1;
}
