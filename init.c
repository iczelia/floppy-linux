#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
  pid_t pid = fork();
  if (pid == 0) {
    char * args[] = { "/bin/busybox", "--install", "-s", NULL };
    execv(args[0], args);
    perror("execv failed"), exit(1);
  } else if (pid < 0)
    perror("fork failed"), exit(1);
  int status; waitpid(pid, &status, 0);
  if (!WIFEXITED(status))
    perror("waitpid"), exit(1);
  if (mount("none", "/dev", "devtmpfs", 0, "")) perror("mount"), exit(1);
  if (mount("none", "/proc", "proc", 0, "")) perror("mount"), exit(1);
  if (mount("none", "/sys", "sysfs", 0, "")) perror("mount"), exit(1);
  // /bin/init is dmesg -n 1;exec sh
  char * args[] = { "/bin/busybox", "sh", "/bin/init", NULL };
  execv(args[0], args);  perror("execv"); exit(1);
}