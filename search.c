
#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

/**
 * Declaration of global variables such as flags to identify args.
*/
struct dirent *dir;
int count;
DIR *parentDir;
bool f_flag = false, s_flag = false, S_flag = false, tf_flag = false, td_flag = false, e_flag = false, E_flag = false;
int s_size, f_depth;
char *f_pattern, *path, *e_pattern, *E_pattern, *E_files; 

/**
 *  getPermissions: returns the permissions of a file/dir
*/
char* getPermissions(struct stat props) {
    char *permissions = malloc(11 * sizeof(char)); 
    permissions[0] = S_ISDIR(props.st_mode) ? 'd' : '-';
    permissions[1] = (props.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (props.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (props.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (props.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (props.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (props.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (props.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (props.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (props.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';
    return permissions;
    free(permissions);
}

/**
 * recursivelyTraverseFileHiearchy : recursively traverse through the file direcory and print the file names and properties.
*/
void recursivelyTraverseFileHiearchy(DIR *parentDir, char *name, int space, int depth) {
  count = 1;
  struct dirent *dir;
  char currentDirName[1024];

  while(parentDir && (dir = readdir(parentDir)) != NULL){
    sprintf(currentDirName, "%s/%s", name, dir->d_name);
    if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) {
      struct stat fileProps;
      if (stat(currentDirName, &fileProps) == -1) {
        perror("Error: Could not open given file due to some issue.");
      }
      else {
        //recurse through the directory by appending the previous file path to the child one
        if (S_ISDIR(fileProps.st_mode)) {
          DIR *currentDir = opendir(currentDirName); 
          if(currentDir != NULL)
          {
            recursivelyTraverseFileHiearchy(currentDir, currentDirName, space + 1, depth + 1); 
            closedir(currentDir);
          }
        } 

        /** Files checking based on the flag vales(arguments)*/

        if (f_flag &&( depth > f_depth || (f_pattern != NULL && strstr(dir->d_name, f_pattern) == NULL))) {
          continue;
        }
        if(s_flag && fileProps.st_size > s_size) { //check whether -s flag condition satisfies
          continue;
        }

        /**
         * Linkable files checking
        */
        if (S_ISLNK(fileProps.st_mode)) {
          char fileLinkName[1024];
          ssize_t r = readlink(name, fileLinkName, sizeof(fileLinkName) - 1);
          if (r == -1) {
              perror("readlink");
          }
          fileLinkName[r] = '\0';
          printf("%*s[%d] %s %s\n", 4 * space, " ", count, dir->d_name, fileLinkName);
        } else {
          //check whether -t f flag condition satisfies
          if(tf_flag && !S_ISREG(fileProps.st_mode)) { 
            continue;
          }
          //check whether -t d flag condition satisfies
          if(td_flag && !S_ISDIR(fileProps.st_mode)) {
            continue;
          }
          if(e_flag && S_ISREG(fileProps.st_mode)) {
            char cmd[1024];
            sprintf(cmd, "%s %s", e_pattern, currentDirName);
            system(cmd);
          }
          if(E_flag) {
            sprintf(E_pattern, "%s %s", E_pattern, currentDirName);
          }
          // check whether -S flag condition satisfies
          if(S_flag) printf("%*s[%d] %s, [%s], size:(%llu), Time:%s \n",4*space, " ", count, dir->d_name, getPermissions(fileProps), fileProps.st_size, ctime(&fileProps.st_atime));
          else {
            printf("%*s[%d] %s\n", 4 * space, " ", count, dir->d_name); 
          }
        }
      }
    } 
  }
}

/**
 * validateArgs: It'll validate args and assigns flag values.
*/
void validateArgs(int count, char **arg_list) {
  int i = 1;
  while(i< count) {
    if(strcmp(arg_list[i],(char *)("-S")) == 0) S_flag = true;
    else if(strcmp(arg_list[i],(char *)("-s")) == 0) {
      s_flag = true;
      s_size = atoi(arg_list[++i]);
    }
    else if(strcmp(arg_list[i],(char *)("-f")) == 0) {
      f_flag = true;
      f_pattern = arg_list[++i];
      f_depth = atoi(arg_list[++i]);
    }
    else if(strcmp(arg_list[i],(char *)("-t")) == 0) {
      if(strcmp(arg_list[++i],(char *)("f")) == 0) tf_flag = true;
      else if(strcmp(arg_list[i],(char *)("d")) == 0) td_flag = true;
      else {
        printf("Invalid Arguments passed for -t flag");
        exit(-1);
      }
    }
    else if(strcmp(arg_list[i],(char *)("-e")) == 0) {
      e_flag = true;
      e_pattern = arg_list[++i];
    }
    else if(strcmp(arg_list[i],(char *)("-E")) == 0) {
      E_flag = true;
      E_pattern = arg_list[++i];
    }
    else {
      printf("Invalid Arguments passed");
      exit(-1);
    }
    i++;
  }
}

int main (int argc, char **argv) { 
  
  DIR *parentDir; 
  int res = 1;
  if(argc > 1) {
    res = access(argv[argc - 1], R_OK);
    if(res == 0) validateArgs(argc - 1, argv);
    else validateArgs(argc, argv);
  }
  // function pointer for recursivelyTraverseFileHiearchy method
  void (*traverse)(DIR *, char *, int, int);
  traverse = &recursivelyTraverseFileHiearchy;

  int align = 0;

  //if no directory path case
  if(res == 0 && argc > 1) {
    parentDir = opendir (argv[argc - 1]); 
    traverse(parentDir, argv[argc - 1], align, 0);
  }
  else {
    parentDir = opendir ("./"); 
    traverse(parentDir, "./", align, 0);
  }
  if(E_flag) system(E_pattern);
  if (parentDir == NULL) { 
    printf ("Error opening directory '%s'\n", argv[1]); 
    exit (-1);
  } 

  closedir (parentDir);
  return 0; 
} 

