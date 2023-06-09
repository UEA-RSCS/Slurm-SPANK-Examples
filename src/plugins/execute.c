//
// Purpose:
// Execute a command within the same control group as the job step, and terminate the 
// command when the job step ends
//
// Compile Instructions:
// gcc -fPIC -shared -o execute.so execute.c
//
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <slurm/spank.h>
#include <signal.h>

// define the plugin name - we'll use this to refer to
#ifndef PLUGIN_NAME
  #define PLUGIN_NAME "execute"
#endif

// required macro
SPANK_PLUGIN(PLUGIN_NAME, 1);

// create a global variable to store the pid - this will be used by different entrypoints
pid_t pid;

// invoked prior to the task being execve'd
int slurm_spank_task_init(spank_t sp, int ac, char **av) {
  // setup our command with args - here we're using sleep as we all like a nap
  // from the manpage "by convention, the first of these strings (i.e., argv[0]) 
  // should contain the filename associated with the file being executed"
  char *args[] = {"/usr/bin/sleep", "600", NULL};

  // fork our process
  pid = fork();
  if (pid < 0) {
    // we errored, lets record that and leave
    slurm_error("%s %s: Fork failure", PLUGIN_NAME, __func__);
    return(-1);
  } else if (pid > 0) {
    // successfully forked, let store the child pid for later
    slurm_info("%s %s: Forked process %d", PLUGIN_NAME, __func__, pid);
    // don't wait / ignore the child process should it exit (avoids defunct processes)
    signal(SIGCHLD, SIG_IGN);
    return 0;
  }

  // we should be in the child process now - so lets close off the standard in, out and error
  // if we don't do this we'll print to the remote context, i.e. slurmd's log - so this could be 
  // helpful for debugging
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  // create a session and process group for itself - this could be helpful if the new process 
  // spawns children as you can just kill off the process group
  if (setsid() == -1)
    slurm_error("%s %s: Unable to setsid", PLUGIN_NAME, __func__);

  // execute the command passing the arguments
  if (execv("/usr/bin/sleep", args) == -1)
    slurm_error("%s %s: Unable to exec", PLUGIN_NAME, __func__);

  return(0);
}

// called as the task exits
int slurm_spank_task_exit(spank_t sp, int ac, char **av) {
  // kill our command via the process group
  // this also allows for the cgroup can be torned down successfully
  killpg(pid, SIGTERM);

  return(0);
}