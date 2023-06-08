// gcc -fPIC -shared -o preload.so preload.c
#include <stdlib.h>
#include <slurm/spank.h>
#include <slurm/slurm.h>
#include <sys/time.h>
#include <string.h>

#ifndef PLUGIN_NAME
  #define PLUGIN_NAME "preload"
#endif

// required macro
SPANK_PLUGIN(PLUGIN_NAME, 1);

//int slurm_spank_init(spank_t sp, int ac, char **av) {
//  return(ESPANK_SUCCESS);
//}

int slurm_spank_user_init(spank_t sp, int ac, char **av) {
  char preloader[18] = "/tmp/preloader.so";
  char preload_list[32760];
  char u_preload_list[32760];

  slurm_info("1");
  if (spank_getenv(sp, "LD_PRELOAD", preload_list, sizeof(preload_list)) == ESPANK_SUCCESS) {
    slurm_info("2");
    snprintf(u_preload_list, sizeof(u_preload_list), "%s:%s", preloader, preload_list);
  } else {
    slurm_info("3");
    snprintf(u_preload_list, sizeof(u_preload_list), "%s", preloader);
  }

  spank_err_t err = spank_setenv(sp, "LD_PRELOAD", u_preload_list, 1);

	if (err != ESPANK_SUCCESS)
		slurm_error("%s %s: %s", PLUGIN_NAME, __func__, spank_strerror(err));

  return(ESPANK_SUCCESS);
}