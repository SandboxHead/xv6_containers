#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

struct {
  char name[14];
  struct stat st;
} kernel_file[64];

struct {
  char name[14];
  struct stat st;
} container_file[64];

int kernel_len = 0;
int conatiner_len = 0;

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls()
{
  char* path = ".";
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  int cid = get_container();

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      char *fname = fmtname(buf);
      int n = 0;
      for(n = 0;;n++){
        if (fname[n] == ' ') break;
      }
      char c[2];
      if (n >= 6){
        c[0] = fname[n-6];
        c[1] = fname[n-5];
      }
      else {
        c[0] = '^';
        c[1] = '^';
      }
      char one = cid/10 + '0';
      char two = cid%10 + '0';
      // printf(1, "%d \n", n);
      if (c[0] == one && c[1] == two){
        char fprint[n - 7];
        for (int i = 0; i < 14; i++){
          if(i<n-7) fprint[i] = fname[i];
          else fprint[i] = ' ';
        }
        // container_file[conatiner_len].name = fprint;
        // printf("%s\n",  );
        strcpy(container_file[conatiner_len].name, fprint);
        // printf(1, "%s\n", container_file[conatiner_len].name);
        container_file[conatiner_len++].st = st;
        // printf(1, "%s %d %d %d\n", fprint, st.type, st.ino, st.size);
      }
      // else if (cid == -1 && c[0] != one && c[1] != two)
      //   printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
      if ((c[0] < '0' || c[0] > '9') && (c[1] < '0' || c[1] > '9')){
        // kernel_file[kernel_len].name = fname;
        strcpy(kernel_file[kernel_len].name, fname);
        // printf(1, "%s %s\n", kernel_file[kernel_len].name, fname);
        kernel_file[kernel_len++].st = st;
      }


    }
    break;
  }
  close(fd);
}

void print_ls(){
  for(int i = 0; i < kernel_len; i++) {
    int j;
    for(j = 0; j < conatiner_len; j++) {
      if (strcmp(kernel_file[i].name, container_file[j].name) == 0) break;
    }
    if(j == conatiner_len){
      printf(1, "%s %d %d %d\n", kernel_file[i].name, kernel_file[i].st.type, kernel_file[i].st.ino, kernel_file[i].st.size);
    }
  }

  for(int i = 0; i < conatiner_len; i++) {
    printf(1, "%s %d %d %d\n", container_file[i].name, container_file[i].st.type, container_file[i].st.ino, container_file[i].st.size);
  }

  kernel_len = 0;
  conatiner_len = 0;
}

// int
// main(int argc, char *argv[])
// {
//   int i;

//   if(argc < 2){
//     ls(".");
//     print_ls();
//     exit();
//   }
//   for(i=1; i<argc; i++){
//     ls(argv[i]);
//     print_ls();
//   }
//   exit();
// }
