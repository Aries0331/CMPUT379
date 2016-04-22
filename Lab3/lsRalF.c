//
//  cmput379
//  Name: JINZHU LI
//  Student ID: 1461696
//  lab3
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

void displayDir(char *file, int cut);
void displayFileInfo(char *file);

int main(int argc, char * argv[]) {
    
    struct stat buf;
    char path[PATH_MAX + 1];
    char *currWorkDir;
    int len;

    if(argc != 2) {
      argv[1] = ".";
    }

    if(lstat(argv[1], &buf) < 0) {
        perror("stat error");
        exit(EXIT_FAILURE);
    }
    
    currWorkDir = getcwd(path, PATH_MAX + 1);
    len = (int)strlen(currWorkDir) + 1;
    
    // If directory, recursively print out the directory/file information
    if(S_ISDIR(buf.st_mode)) {
        printf("%s\n", argv[1]);
        displayDir(argv[1], len);
    }
    
    // If regular file, print out the file information directly
    else if (S_ISREG(buf.st_mode)) {
        displayFileInfo(argv[1]);
    }

    return 0;
}

/** @brief display all the relevant information for the file
 *         
 *  @param file accept the file name which the is current 
 *         path name
 *
 *  @return void.
 */
void displayFileInfo(char *file)
{
    struct stat fileStat;
    struct stat link;
    char *linkname;
    uid_t uid;
    gid_t gid;
    struct passwd *pw;
    struct group *gr;
    time_t t;
    ssize_t r;
    
    if(lstat(file, &fileStat) < 0) {
        perror("stat error");
        exit(EXIT_FAILURE);
    }
    
    /* Mode bits and access permissions */
    if(S_ISLNK(fileStat.st_mode))
        printf("l");
    else if(S_ISFIFO(fileStat.st_mode))
      printf("p");
    else
        printf(S_ISDIR(fileStat.st_mode) ? "d" : "-");
    
    // user
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    if((fileStat.st_mode & S_ISUID) && (fileStat.st_mode & S_IRUSR))
        printf("s");
    else if ((fileStat.st_mode & S_ISUID) && !(fileStat.st_mode & S_IRUSR))
        printf("S");
    else
        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    
    // group
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    if((fileStat.st_mode & S_ISGID) && (fileStat.st_mode & S_IXGRP))
        printf("s");
    else if ((fileStat.st_mode & S_ISGID) && !(fileStat.st_mode & S_IXGRP))
        printf("S");
    else
        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    
    // others
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    if((fileStat.st_mode & S_ISVTX) && (fileStat.st_mode & S_IXOTH))
        printf("t");
    else if ((fileStat.st_mode & S_ISVTX) && !(fileStat.st_mode & S_IXOTH))
        printf("T");
    else
        printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
    
    /* Number of links */
    printf("%2d\t", (int)fileStat.st_nlink);
    
    /* owner name and group name */
    uid = fileStat.st_uid;
    gid = fileStat.st_gid;
    if((pw = getpwuid(uid)) == NULL) {
        printf("Fail: uid %d does not exits.\n", uid);
        exit(1);
    }
    else
        printf("%s\t", pw->pw_name);
    if((gr = getgrgid(gid)) == NULL) {
        printf("Fail: gid %d does not exits.\n", gid);
        exit(1);
    }
    else
        printf("%s\t", gr->gr_name);
    
    /* Calculate the file size */
    printf("%ld\t",(long)fileStat.st_size);
    
    /* Time of last modification */
    t = fileStat.st_mtime;
    char ltime[20];
    strftime(ltime,20,"%b %d %H:%M", localtime(&t));
    printf("%s\t", ltime);
    
    /* file/directory name */
    printf("%s", file);
    
    // link
    if(S_ISLNK(fileStat.st_mode)) {
        printf(" -> ");
        linkname = malloc(fileStat.st_size + 1);
        if(linkname == NULL) {
            fprintf(stderr, "insufficirnt memory\n");
            exit(EXIT_FAILURE);
        }
        r = readlink(file, linkname, fileStat.st_size + 1);
        if(r < 0) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }
        if(r > fileStat.st_size) {
            fprintf(stderr, "symlink increased in size between lstat() and readlink()\n");
            exit(EXIT_FAILURE);
        }
        linkname[fileStat.st_size] = '\0';
        
        printf("%s", linkname);
	if(stat(file, &link) >= 0) {
	 if((((link.st_mode) & S_IXUSR) || ((link.st_mode) & S_IXGRP) || ((link.st_mode) & S_IXOTH)) && !(S_ISDIR(link.st_mode))) {
	    printf("*");
	}
	} 
       	else {
	  // if directory
	  if(S_ISDIR(fileStat.st_mode))
	    printf("/");
	  // if FIFO
	  else if(S_ISFIFO(fileStat.st_mode))
	    printf("|");
	}

        free(linkname);
    }

    // if directory
    else if(S_ISDIR(fileStat.st_mode))
        printf("/");
    // if executable
    else if((fileStat.st_mode) & S_IXUSR)
        printf("*");
    // if FIFO
    else if(S_ISFIFO(fileStat.st_mode))
        printf("|");

    printf("\n");
}

/** @brief print out any and all files/directory
 *         contained within the directory
 *  @param file  accept a filenme
 *        
 *  @param cut len of the full directory before
 *         current directory. used when print out
 *         the directory
 *  @return void.
 */
void displayDir(char *file, int cut)
{
    struct stat buf;
    DIR *pDir;
    char path[PATH_MAX + 1];
    char *currWorkDir;
    struct dirent **namelist;
    int i, count;
    int blocks = 0;
    
    pDir = opendir(file);
    if(pDir == NULL) {
        fprintf(stderr, "Cannot open directory '%s'\n", file);
        exit(EXIT_FAILURE);
    }
    if(lstat(file, &buf) < 0) {
        perror("stat error");
        exit(EXIT_FAILURE);
    }
    
    count = scandir(file, &namelist, NULL, alphasort);
    
    /* change to the current directory */
    if(chdir(file) < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    /* total disk allocation for all files in this directory */
    for(i=0; i<count; i++) {
        if(lstat(namelist[i]->d_name, &buf) < 0) {
            perror("stat error");
            exit(EXIT_FAILURE);
        }
        else {
            blocks += (int)buf.st_blocks;
        }
    }
    printf("total %d\n", (blocks+1)/2);
    
    /* read current directroy and display fileinfo */
    for(i=0; i<count; i++) {
        displayFileInfo(namelist[i]->d_name);
    }
    
    if(count < 0) {
        perror("scandir error");
        exit(EXIT_FAILURE);
    }
    else {
        for(i=0; i<count; i++) {
            if(lstat(namelist[i]->d_name, &buf) < 0) {
                perror("stat error");
                exit(EXIT_FAILURE);
            }
            if(S_ISDIR(buf.st_mode)) {
                if(S_ISDIR(buf.st_mode) && (strcmp(".", namelist[i]->d_name) == 0 || strcmp("..", namelist[i]->d_name) == 0))
                    continue;
                else {
                    printf("\n");
                    currWorkDir = getcwd(path, PATH_MAX + 1);
                    printf("%s", currWorkDir + cut);
                    printf("/%s\n", namelist[i]->d_name);
                    displayDir(namelist[i]->d_name, cut);
                    if(chdir("..") < 0) {
                        perror("chdir error");
                        exit(EXIT_FAILURE);
                    }
                }
            }
	    free(namelist[i]);
        }
	free(namelist);
    }
  
    closedir(pDir);
}
