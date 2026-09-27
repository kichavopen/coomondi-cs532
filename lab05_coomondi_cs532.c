#include <stdio.h> 
/* This is the program for recursive traversal
 * Recursively lists all files and directories
 * Specifically based on the readdir_v2.c file
 */
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 

char *filetype(unsigned char type) {
  char *str;
  switch(type) {
  case DT_BLK: str = "block device"; break;
  case DT_CHR: str = "character device"; break;
  case DT_DIR: str = "directory"; break;
  case DT_FIFO: str = "named pipe (FIFO)"; break;
  case DT_LNK: str = "symbolic link"; break;
  case DT_REG: str = "regular file"; break;
  case DT_SOCK: str = "UNIX domain socket"; break;
  case DT_UNKNOWN: str = "unknown file type"; break;
  default: str = "UNKNOWN";
  }
  return str;
}

/* Specific function for recursive traversal */
void traverse(const char *path, int level) {
  DIR *dir;
  struct dirent *entry;
  char new_path[1024];
  int i;

  dir = opendir(path);
  if (dir == NULL) {
    printf("Error opening directory '%s'\n", path);
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    /* Skip . and .. to avoid infinite recursion */
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    /* Print indentation based on depth level */
    for (i = 0; i < level; i++)
      printf("  ");

    printf("%s (%s)\n", entry->d_name, filetype(entry->d_type));

    /* If entry is a directory, recurse into it */
    if (entry->d_type == DT_DIR) {
      snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
      traverse(new_path, level + 1);
    }
  }

  closedir(dir);
}

int main(int argc, char **argv) { 
  if (argc < 2) { 
    printf("Usage: %s <dirname>\n", argv[0]); 
    exit(-1);
  } 

  traverse(argv[1], 0);

  return 0; 
}
