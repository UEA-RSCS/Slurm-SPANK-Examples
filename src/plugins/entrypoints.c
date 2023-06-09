//
// Purpose:
// Trace each entrypoint
//
// Compile Instructions:
// gcc -fPIC -shared -o entrypoints.so entrypoints.c
//
#include <stdlib.h>
#include <slurm/spank.h>
#include <slurm/slurm.h>
#include <sys/time.h>

// define the plugin name - we'll use this to refer to
#ifndef PLUGIN_NAME
  #define PLUGIN_NAME "entrypoints"
#endif

// required macro
SPANK_PLUGIN(PLUGIN_NAME, 1);

// define functions
static void _entrypoints(const char *entrypoint);

// enumerate context strings
static const char *spank_context_str[] = {
  "S_CTX_ERROR",
  "S_CTX_LOCAL",
  "S_CTX_REMOTE",
  "S_CTX_ALLOCATOR",
  "S_CTX_SLURMD",
  "S_CTX_JOB_SCRIPT"
};

// function to display the calling function, current time, context and uids
static void _entrypoints(const char *entrypoint) {
  // get the current time
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  // calculate milliseconds
  int milli = current_time.tv_usec / 1000;

  // format timestamp
  char * timestamp = malloc(sizeof(char) * 80);
  strftime(timestamp, 80, "%Y-%m-%d %H:%M:%S:%03d", localtime(&current_time.tv_sec));
  char * timestamp_milli = malloc(sizeof(char) * 84);
  sprintf(timestamp_milli, "%s:%03d", timestamp, milli);

  // output
  slurm_info("%s [%s] [%s] [%lu] [%lu]: %s", 
    PLUGIN_NAME,
    timestamp_milli, 
    spank_context_str[spank_context()], 
    (unsigned long int)getuid(), 
    (unsigned long int)geteuid(),
    entrypoint);
}

// called after plugins are loaded but before option processing
int slurm_spank_init(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called at same time as job prolog
int slurm_spank_job_prolog(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called after plugins are loaded and after option processing
int slurm_spank_init_post_opt(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called after option processing but by srun only
int slurm_spank_local_user_init(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called in remote context after privileges dropped
int slurm_spank_user_init(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called after each task in forked when they still have elevated privileges
int slurm_spank_task_init_privileged(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called before execve - uses the jobs cgroup
int slurm_spank_task_init(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called from the parent process after forking
int slurm_spank_task_post_fork(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called on the exit of each task
int slurm_spank_task_exit(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called before slurmstepd exits
int slurm_spank_exit(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called at same time as job epilog
int slurm_spank_job_epilog(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

// called when slurmd is shutdown
int slurm_spank_slurmd_exit(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}