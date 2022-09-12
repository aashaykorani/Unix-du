#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

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
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_FILE:
    printf(1, "%d %s\n", st.size, fmtname(path));
    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        printf(1,"This is the st type %d\n",st.type);
    //   if (*de.name == '.'){
    //     printf(1,"TRUE %s\n",de.name);
    //     continue;
    //   }
      if (de.inum == 0 || *de.name == '.')
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0) {
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      totalsize += st.size;
      printf(1, "%d %s\n", st.size, fmtname(buf));
    }
    break;
  }
  printf(1,"%d .\n",totalsize);
  close(fd);
}

int main(int argc, char *argv[]) {
  int i;

  if (argc < 2) {
    du(".");
    exit();
  }
  for (i = 1; i < argc; i++)
    du(argv[i]);
  exit();
}
