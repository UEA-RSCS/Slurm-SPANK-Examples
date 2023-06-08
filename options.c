// gcc -fPIC -shared -o options.so options.c
#include <stdlib.h>
#include <slurm/spank.h>
#include <slurm/slurm.h>
#include <sys/time.h>
#include <string.h>

#ifndef PLUGIN_NAME
  #define PLUGIN_NAME "options"
#endif

// required macro
SPANK_PLUGIN(PLUGIN_NAME, 1);

char *opt1_value;
int opt2_value;

// define functions
static int _opt1_reg(int val, const char *optarg, int remote);
static int _opt2_reg(int val, const char *optarg, int remote);

// spank options structure
struct spank_option plugin_opt1 = {
  "opt1",
  NULL,
  "Option One",
  1,
  0,
  (spank_opt_cb_f)_opt1_reg
};

// spank options structure
struct spank_option plugin_opt2 = {
  "opt2",
  NULL,
  "Option Two",
  1,
  0,
  (spank_opt_cb_f)_opt2_reg
};

int slurm_spank_init(spank_t sp, int ac, char **av) {
  if (spank_option_register(sp, &plugin_opt1) != ESPANK_SUCCESS)
    slurm_error("%s %s: Failed to register %s", PLUGIN_NAME, __func__, plugin_opt1.name);

  if (spank_option_register(sp, &plugin_opt2) != ESPANK_SUCCESS)
    slurm_error("%s %s: Failed to register %s", PLUGIN_NAME, __func__, plugin_opt2.name);
  
  return(ESPANK_SUCCESS);
}

int slurm_spank_user_init(spank_t sp, int ac, char **av) {
  if (opt1_value) {
    spank_err_t err = spank_setenv(sp, "SLURM_MYOPT", opt1_value, 1);
    if (err != ESPANK_SUCCESS)
      slurm_error("%s %s: %s", PLUGIN_NAME, __func__, spank_strerror(err));
  }

  for (int i = 0; i < ac; i++) {
    if (strncmp ("min_opt2=", av[i], 9) == 0) {
      int min_opt2 = atoi(av[i] + 9);
      if (opt2_value < min_opt2)
        slurm_info("%s %s: Option2 (%d) is less than minimium (%d)", 
          PLUGIN_NAME, __func__, opt2_value, min_opt2);
      else
        slurm_info("%s %s: Option2 (%d) is greater/equal to minimum (%d)", 
          PLUGIN_NAME, __func__, opt2_value, min_opt2);
    }
  }

  return(ESPANK_SUCCESS);
}

static int _opt1_reg(int val, const char *optarg, int remote) {
  opt1_value = malloc(strlen(optarg) + 1);
  strcpy(opt1_value, optarg);
  
  return(ESPANK_SUCCESS);
}

static int _opt2_reg(int val, const char *optarg, int remote) {
  opt2_value = atoi(optarg);
  
  return(ESPANK_SUCCESS);
}