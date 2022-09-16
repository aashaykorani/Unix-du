#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "myfuntions.h"

int k = 0,t = 0,r = 0;
int threshold;
int recursive_call = 0;
int potential_full_path = 0, slash_removed = 0;
int recursive_totalsize = 0;
// char subdir[15] = {'\0'};

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

void du(char *path) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  int totalsize = 0;
 
  if ((fd = open(path, 0)) < 0) {
    printf(2, "du: cannot open %s\nNo such file or directory\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    printf(2, "du: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_FILE:
    if(t==1 && k==0){
        if(st.size > threshold){
            if(potential_full_path == 0 && recursive_call == 0)
                printf(1, "%d %s\n", st.size, fmtname(path));
            else
                printf(1, "%d %s\n", st.size,path);
            printf(1,"%d %s\n",st.size,path);
        }
    }
    else if(t==1 && k==1){
        if(st.size > threshold){
            int blocks_occupied = st.size / BSIZE;
            if(potential_full_path == 0 && recursive_call == 0)
                printf(1, "%d %s\n", blocks_occupied, fmtname(path));
            else
                printf(1, "%d %s\n", blocks_occupied,path);
            printf(1,"%d %s\n",blocks_occupied,path);
        }
    }
    else if (k==1){
        int blocks_occupied = st.size / BSIZE;
        if(potential_full_path == 0 && recursive_call == 0)
            printf(1, "%d %s\n", blocks_occupied, fmtname(path));
        else
            printf(1, "%d %s\n", blocks_occupied,path);
        printf(1,"%d %s\n",blocks_occupied,path);
    }
    else{
        if(potential_full_path == 0 && recursive_call == 0)
            printf(1, "%d %s\n", st.size, fmtname(path));
        else
            printf(1, "%d %s\n", st.size,path);
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
          if(t == 1 && k == 0){
              if(st.size > threshold){
                  totalsize += st.size;
                  if(potential_full_path == 0 && recursive_call == 0)
                    printf(1, "%d %s\n", st.size, fmtname(buf));
                  else
                    printf(1, "%d %s\n", st.size,buf);
              }
          }
          else if (t == 1 && k == 1){
            if(st.size > threshold){
                int blocks_occupied = st.size / BSIZE;
                totalsize += blocks_occupied;
                if(potential_full_path == 0 && recursive_call == 0)
                    printf(1, "%d %s\n", blocks_occupied, fmtname(buf));
                else
                    printf(1, "%d %s\n", blocks_occupied,buf);
            }
          }
          else if (k == 1){
              int blocks_occupied = st.size / BSIZE;
              totalsize += blocks_occupied;
              if(potential_full_path == 0 && recursive_call == 0)
                printf(1, "%d %s\n", blocks_occupied, fmtname(buf));
              else
                printf(1, "%d %s\n", blocks_occupied,buf);
          }
          else{
                // printf(1,"Recrursive call = %d\n",recursive_call);
                totalsize += st.size;
                if(potential_full_path == 0 && recursive_call == 0)
                    printf(1, "%d %s\n", st.size,fmtname(buf));
                else
                    printf(1, "%d %s\n", st.size,buf);
          }
      }
      else if(st.type == T_DIR && r == 1){
          if(strstr(buf,"/.") != NULL)
            continue;
          recursive_call = 1;
          du(buf);
          recursive_call = 0;
      }
    }
    if(r==1)
        recursive_totalsize += totalsize;
    else
        recursive_totalsize = totalsize;
    if(slash_removed == 1)
        printf(1,"%d %s\n",recursive_totalsize,strcat(path,"/"));
    else
        printf(1,"%d %s\n",recursive_totalsize,path);
    break;
  }
  close(fd);
  return;
}

int main(int argc, char *argv[]) {
  int i,n;
  if (argc < 2) {
    du(".");
    // du(subdir);
    exit();
  }
  for (i = 1; i < argc; i++){
      
    if (strcmp(argv[i],"-k")==0){
        k += 1;
        continue;
    }
    else if(strcmp(argv[i],"-t")==0){
        if(isNumber(argv[i+1]) != 1){
            printf(2,"Threshold value not provided.\nCheck Usage\n");
            exit();
        }
        t += 1;
        i+=1;
        n=i;
        threshold = atoi(argv[i]);
        // printf(1,"This is the th value %d\n",threshold);
        continue;
    }
    else if(strcmp(argv[i], "-r")==0){
        r += 1;
        continue;
    }
    else{
        if(argv[i][0] == '-'){
            printf(2,"Unrecognized argument.\nCheck Usage\n");
            exit();
        }
        continue;
    }
  }
        if(k>1 || t>1 || r>1){
            printf(2,"Flag repeated.\nCheck Usage\n");
            exit();
        }
        if(strcmp(argv[i-1],"-k")==0 || strcmp(argv[i-1],"-t")==0 || strcmp(argv[i-1],"-r")==0 || argv[i-1]==argv[n]){
            // printf(1,"Inside if\n");
            du(".");
            exit();
        }
        else{
            // printf(1,"Inside else\n");
            potential_full_path = 1;
            if(argv[i-1][strlen(argv[i-1])-1] == '/'){
                argv[i-1][strlen(argv[i-1])-1] = '\0';
                slash_removed = 1;            
            }
            // printf(1,"New argv = %s\n",argv[i-1]);
            du(argv[i-1]);
            }
  exit();
}