//
// Purpose:
// Basic plugin that reads job environment variables and prints them to Slurmd's log
//
// Compile Instructions:
// gcc -fPIC -shared -o envs.so envs.c
//
#include <stdlib.h>
#include <string.h>
#include <slurm/spank.h>

// define the plugin name - we'll use this to refer to
#ifndef PLUGIN_NAME
  #define PLUGIN_NAME "envs"
#endif

// required macro
SPANK_PLUGIN(PLUGIN_NAME, 1);

// plugin handle, arg count and arg vector
int slurm_spank_init(spank_t sp, int ac, char **av) {
  // get job environment and print it out
  char **env;
  if (spank_get_item(sp, S_JOB_ENV, &env) == ESPANK_SUCCESS) {
    while (*env != NULL) {
      // look for SLURM_JOB_NAME - if we find it squirrel it off and print it out
      if (strncmp(*env, "SLURM_JOB_NAME", 14) == 0) {
        char *s_job_name_env;
        char *s_job_name;
        s_job_name_env = malloc(strlen(*env) + 1);
        strcpy(s_job_name_env, *env);
        s_job_name = strtok(s_job_name_env, "=");
        s_job_name = strtok(NULL, "=");
        slurm_info("%s %s job name: %s", PLUGIN_NAME, __func__, s_job_name);
      }
      slurm_info("%s %s spank_get_item: %s", PLUGIN_NAME, __func__, *env);
      ++env;
    }
  }
  
  // home for tea and biscuits
  return (0);
}