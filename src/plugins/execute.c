// gcc -fPIC -shared -o execute.so execute.c
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <slurm/spank.h>
#include <signal.h>

#ifndef PLUGIN_NAME
  #define PLUGIN_NAME "execute"
#endif

// required macro
SPANK_PLUGIN(PLUGIN_NAME, 1);

pid_t pid;

int slurm_spank_task_init(spank_t sp, int ac, char **av) {
  char *args[] = {"/usr/bin/sleep", "600", NULL};

  pid = fork();
  if (pid < 0) {
    slurm_error("%s %s: Fork failure", PLUGIN_NAME, __func__);
    return(-1);
  } else if (pid > 0) {
    slurm_info("%s %s: Forked process %d", PLUGIN_NAME, __func__, pid);
    return 0;
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  setpgid(0, 0);

  if ((execv("/usr/bin/sleep", args)) == -1)
    slurm_error("%s %s: Unable to exec", PLUGIN_NAME, __func__);

  return(0);
}

int slurm_spank_task_exit(spank_t sp, int ac, char **av) {
  kill(pid, SIGTERM);

  return(0);
}