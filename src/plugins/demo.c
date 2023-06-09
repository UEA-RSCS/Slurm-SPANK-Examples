//
// Purpose:
// Basic plugin that reads job environment variables and prints them to Slurmd's log
//
// Compile Instructions:
// gcc -fPIC -shared -o demo.so demo.c
//
#include <stdlib.h>
#include <string.h>
#include <slurm/spank.h>

// required macro
SPANK_PLUGIN(demo, 1);

// plugin handle, arg count and arg vector
int slurm_spank_init(spank_t sp, int ac, char **av) {
  slurm_info("demo: slurm_spank_init");
  
  // get job environment and print it out
  char **env;
  if (spank_get_item(sp, S_JOB_ENV, &env) == ESPANK_SUCCESS) {
    while (*env != NULL) {
      slurm_info("spank_get_item: %s", *env);
      ++env;
    }
  }
  
  // home for tea and biscuits
  return (0);
}