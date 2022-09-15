#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

int k = 0;
int t = 0;
int r = 0;
int cannot_open = 0;
int cannot_stat = 0;

int isdigit(int c)
{
	return (c >= '0' && c <= '9' ? 1 : 0);
}

int isNumber(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

char *fmtname(char *path) {
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

const char * du(char *path) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  int totalsize = 0;
 
  if ((fd = open(path, 0)) < 0) {
    printf(2, "du: cannot open %s\n", path);
    cannot_open = 1;
    return path;
  }

  if (fstat(fd, &st) < 0) {
    printf(2, "du: cannot stat %s\n", path);
    cannot_stat = 1;
    close(fd);
    return path;
  }

  switch (st.type) {
  case T_FILE:
    if (k==1){
        int blocks_occupied = st.size / BSIZE;
        printf(1, "%d %s\n", blocks_occupied, fmtname(path));
        printf(1,"%d %s\n",blocks_occupied,path);
    }
    else{
        printf(1, "%d %s\n", st.size, fmtname(path));
        printf(1,"%d %s\n",st.size,path);
    }
    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf(1, "du: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0) {
        printf(1, "du: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == 2){
          if (k == 1){
              int blocks_occupied = st.size / BSIZE;
              totalsize += blocks_occupied;
              printf(1, "%d %s\n", blocks_occupied, fmtname(buf));
          }
          else{
                totalsize += st.size;
                printf(1, "%d %s\n", st.size, fmtname(buf));
          }
      }
    }
    printf(1,"%d %s\n",totalsize,path);
    break;
  }
  close(fd);
  return "3";
}

int main(int argc, char *argv[]) {
  int i;

  if (argc < 2) {
    du(".");
    exit();
  }
  for (i = 1; i < argc; i++){
      
    if (strcmp(argv[i],"-k")==0){
        k = 1;
        continue;
    }
    else if(strcmp(argv[i],"-t")==0){
        if(isNumber(argv[i+1]) != 1){
            printf(2,"Threshold value not provided.\nCheck Usage\n");
            exit();
        }
        t = 1;
        continue;
    }
    else if(strcmp(argv[i], "-r")==0){
        r = 1;
        continue;
    }
    else{
        continue;
    }
  }
        if(strcmp(argv[i-1],"-k")==0 || strcmp(argv[i-1],"-t")==0 || strcmp(argv[i-1],"-r")==0 || isNumber(argv[i-1])==1){
            printf(1,"Inside if\n");
            du(".");
            exit();
        }
        else{
            printf(1,"Inside else\n");
            du(argv[i-1]);
            }
  exit();
}
