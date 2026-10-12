#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define MAX_PATH 1024

//Name: Christopher Omondi
// BlaerID: coomondi
// Project Number: 2
//

// Global variables for filter options
int size_limit = -1;  // -1 means no size limit
char *pattern = NULL;
int max_depth = -1;   // -1 means no depth limit
int show_details = 0;
int type_filter = 0;  // 0=all, 1=files only, 2=directories only

// Function pointer types
typedef void (*file_processor_t)(const char *path, const char *name, int indent, int depth);
typedef void (*dir_processor_t)(const char *dir_path, int indent, int depth, file_processor_t file_proc);

// Function to check if file matches the size filter
int matches_size_filter(const char *path) {
    if (size_limit == -1) return 1;  // No size filter
    
    struct stat file_stat;
    if (lstat(path, &file_stat) == -1) {
        return 0;
    }
    
    if (S_ISDIR(file_stat.st_mode)) {
        return 1;  // Always include directories
    }
    
    return file_stat.st_size <= size_limit;
}

// Function to check if file matches the pattern and depth filter
int matches_pattern_filter(const char *name, int depth) {
    if (pattern == NULL) return 1;  // No pattern filter
    
    if (max_depth != -1 && depth > max_depth) {
        return 0;  // Exceeds depth limit
    }
    
    return strstr(name, pattern) != NULL;
}

// Function to check if file matches the type filter
int matches_type_filter(const char *path) {
    if (type_filter == 0) return 1;  // No type filter
    
    struct stat file_stat;
    if (lstat(path, &file_stat) == -1) {
        return 0;
    }
    
    if (type_filter == 1 && S_ISREG(file_stat.st_mode)) {
        return 1;  // Regular file
    }
    
    if (type_filter == 2 && S_ISDIR(file_stat.st_mode)) {
        return 1;  // Directory
    }
    
    return 0;  // Doesn't match
}

// Function to check if file should be included based on all filters
int should_include_file(const char *path, const char *name, int depth) {
    // Apply all filters
    if (!matches_size_filter(path)) {
        return 0;
    }
    
    if (!matches_pattern_filter(name, depth)) {
        return 0;
    }
    
    if (!matches_type_filter(path)) {
        return 0;
    }
    
    return 1;
}

// Function to print file details when -S option is used
void print_file_details(const char *path, const char *name, int indent, int depth) {
    // Apply filters
    if (!should_include_file(path, name, depth)) {
        return;
    }
    
    struct stat file_stat;
    
    if (lstat(path, &file_stat) == -1) {
        perror("lstat");
        return;
    }
    
    // Print indentation
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    
    // Check if it's a symbolic link
    if (S_ISLNK(file_stat.st_mode)) {
        char target[MAX_PATH];
        ssize_t len = readlink(path, target, sizeof(target)-1);
        if (len != -1) {
            target[len] = '\0';
            printf("%s -> %s (", name, target);
        } else {
            printf("%s -> (invalid link) (", name);
        }
    } else {
        printf("%s (", name);
    }
    
    // Print size (0 for directories)
    if (S_ISDIR(file_stat.st_mode)) {
        printf("0 ");
    } else {
        printf("%ld ", file_stat.st_size);
    }
    
    // Print permissions
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    
    // Print last access time
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", localtime(&file_stat.st_atime));
    printf(" %s)\n", time_str);
}

// Function to print file names without details
void print_file_simple(const char *path, const char *name, int indent, int depth) {
    // Apply filters
    if (!should_include_file(path, name, depth)) {
        return;
    }
    
    struct stat file_stat;
    
    if (lstat(path, &file_stat) == -1) {
        perror("lstat");
        return;
    }
    
    // Print indentation
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    
    // Check if it's a symbolic link
    if (S_ISLNK(file_stat.st_mode)) {
        char target[MAX_PATH];
        ssize_t len = readlink(path, target, sizeof(target)-1);
        if (len != -1) {
            target[len] = '\0';
            printf("%s (%s)\n", name, target);
        } else {
            printf("%s (invalid link)\n", name);
        }
    } else {
        printf("%s\n", name);
    }
}

// Function to traverse directory recursively using function pointers
void traverse_directory(const char *dir_path, int indent, int depth, file_processor_t file_proc) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[MAX_PATH];
    
    // Check depth limit for pattern filter
    if (max_depth != -1 && depth > max_depth) {
        return;
    }
    
    if ((dir = opendir(dir_path)) == NULL) {
        perror("opendir");
        return;
    }
    
    // Print directory name
    for (int i = 0; i < indent - 1; i++) {
        printf("\t");
    }
    printf("%s\n", dir_path);
    
    // Read each entry in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Construct full path
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        
        // Get file status
        if (lstat(path, &statbuf) == -1) {
            perror("lstat");
            continue;
        }
        
        // Process the file using the provided function pointer
        file_proc(path, entry->d_name, indent, depth);
        
        // If it's a directory, recursively traverse it
        if (S_ISDIR(statbuf.st_mode)) {
            traverse_directory(path, indent + 1, depth + 1, file_proc);
        }
    }
    
    closedir(dir);
}

int main(int argc, char *argv[]) {
    char start_dir[MAX_PATH];
    
    // Parse command line arguments using getopt
    int opt;
    while ((opt = getopt(argc, argv, "Ss:f:t:")) != -1) {
        switch (opt) {
            case 'S':
                show_details = 1;
                break;
            case 's':
                size_limit = atoi(optarg);
                break;
            case 'f':
                pattern = optarg;
                // Check if depth argument is provided
                if (optind < argc && argv[optind][0] != '-') {
                    max_depth = atoi(argv[optind]);
                    optind++;  // Skip the depth argument
                }
                break;
            case 't':
                if (strcmp(optarg, "f") == 0) {
                    type_filter = 1;  // Files only
                } else if (strcmp(optarg, "d") == 0) {
                    type_filter = 2;  // Directories only
                } else {
                    fprintf(stderr, "Error: Invalid type filter. Use 'f' for files or 'd' for directories.\n");
                    return 1;
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-S] [-s size] [-f pattern depth] [-t f|d] [directory]\n", argv[0]);
                return 1;
        }
    }
    
    // Validate arguments
    if (pattern != NULL && max_depth == -1) {
        fprintf(stderr, "Error: -f option requires both pattern and depth arguments\n");
        return 1;
    }
    
    // Determine starting directory
    if (optind < argc) {
        // Use directory provided by user
        strncpy(start_dir, argv[optind], sizeof(start_dir) - 1);
        start_dir[sizeof(start_dir) - 1] = '\0';
    } else {
        // Use current directory
        getcwd(start_dir, sizeof(start_dir));
    }
    
    // Select the appropriate file processing function based on the -S flag
    file_processor_t file_processor = show_details ? print_file_details : print_file_simple;
    
    // Start traversal using the function pointer
    traverse_directory(start_dir, 1, 0, file_processor);
    
    return 0;
}
