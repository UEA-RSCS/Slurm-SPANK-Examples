//
// Purpose:
// Set LD_PRELOAD for job. Expects preloader.so to be in /tmp/, see src/preload/preloader.c 
// for the source
//
// Compile Instructions:
// gcc -fPIC -shared -o preload.so preload.c
//
#include <stdlib.h>
#include <slurm/spank.h>
#include <slurm/slurm.h>
#include <sys/time.h>
#include <string.h>

// define the plugin name - we'll use this to refer to
#ifndef PLUGIN_NAME
  #define PLUGIN_NAME "preload"
#endif

// required macro
SPANK_PLUGIN(PLUGIN_NAME, 1);

int slurm_spank_user_init(spank_t sp, int ac, char **av) {
  // store the location of the preloader
  char preloader[18] = "/tmp/preloader.so";
  // variable to store the current LD_PRELOAD value
  char preload_list[32760];
  // variable to store the updated LD_PRELOAD value
  char u_preload_list[32760];

  // use slurm_getenv to get the current LD_PRELOAD list
  if (spank_getenv(sp, "LD_PRELOAD", preload_list, sizeof(preload_list)) == ESPANK_SUCCESS) {
    // if it's successful then LD_PRELOAD is already defined and likely contains something so we
    // prepend to it
    snprintf(u_preload_list, sizeof(u_preload_list), "%s:%s", preloader, preload_list);
  } else {
    // if the LD_PRELOAD isn't already define just add our preloader
    snprintf(u_preload_list, sizeof(u_preload_list), "%s", preloader);
  }

  // define the LD_PRELOAD environment variable with our updated preload list
  spank_err_t err = spank_setenv(sp, "LD_PRELOAD", u_preload_list, 1);

	if (err != ESPANK_SUCCESS)
		slurm_error("%s %s: %s", PLUGIN_NAME, __func__, spank_strerror(err));

  return(ESPANK_SUCCESS);
}