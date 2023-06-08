// gcc -fPIC -shared -o entrypoints.so entrypoints.c
#include <stdlib.h>
#include <slurm/spank.h>
#include <slurm/slurm.h>
#include <sys/time.h>

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

static void _entrypoints(const char *entrypoint) {
  // get the current time
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  // calculate milliseconds
  int milli = current_time.tv_usec / 1000;

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

int slurm_spank_init(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_job_prolog(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_init_post_opt(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_local_user_init(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_user_init(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_task_init_privileged(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_task_init(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_task_post_fork(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_task_exit(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_exit(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_job_epilog(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}

int slurm_spank_slurmd_exit(spank_t sp, int ac, char **av) {
  _entrypoints(__func__);
  return(0);
}