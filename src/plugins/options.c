//
// Purpose:
// Register and parse two options (opt1 and opt2). Option opt1 is set as environment 
// variable. Option opt2 is compared against a global runtime argument
// To call use these options just add either or both --opt1=<ARG> and/or --opt2=<ARG> to 
// sbatch, srun, etc.
// Note: opt1 takes an ascii argument, while opt2 takes a numeric argument - this important 
// as there is no proper error checking - top programming!
//
// Compile Instructions:
// gcc -fPIC -shared -o options.so options.c
//
#include <stdlib.h>
#include <slurm/spank.h>
#include <slurm/slurm.h>
#include <sys/time.h>
#include <string.h>

// define the plugin name - we'll use this to refer to
#ifndef PLUGIN_NAME
  #define PLUGIN_NAME "options"
#endif

// required macro
SPANK_PLUGIN(PLUGIN_NAME, 1);

// create global variables to store our arguments - these will be referenced by other entrypoints
char *opt1_value;
int opt2_value;

// define functions
static int _opt1_reg(int val, const char *optarg, int remote);
static int _opt2_reg(int val, const char *optarg, int remote);

// spank option structures for opt1 and opt2 - we could group these into a table, but I think this 
// method of individual structures is better as sbatch and salloc doesn't support the table method
struct spank_option plugin_opt1 = {
  "opt1",                     // name
  NULL,                       // description of the argument
  "Option One",               // usage
  1,                          // 0 if no args, 1 if takes arg, 2 if it takes an optional arg
  0,                          // local value to return to the callback
  (spank_opt_cb_f)_opt1_reg   // name of the callback function
};

struct spank_option plugin_opt2 = {
  "opt2",
  NULL,
  "Option Two",
  1,
  0,
  (spank_opt_cb_f)_opt2_reg
};

// in here we register our options - this is the only place it can occur
int slurm_spank_init(spank_t sp, int ac, char **av) {
  // we pass the option structure to the register function - remember the option structure 
  // contains the function that will actually run
  if (spank_option_register(sp, &plugin_opt1) != ESPANK_SUCCESS)
    slurm_error("%s %s: Failed to register %s", PLUGIN_NAME, __func__, plugin_opt1.name);

  if (spank_option_register(sp, &plugin_opt2) != ESPANK_SUCCESS)
    slurm_error("%s %s: Failed to register %s", PLUGIN_NAME, __func__, plugin_opt2.name);
  
  return(ESPANK_SUCCESS);
}

int slurm_spank_user_init(spank_t sp, int ac, char **av) {
  // if opt1 isn't null we use slurm_setenv to define our environment variable called SLURM_MYOPT
  if (opt1_value) {
    spank_err_t err = spank_setenv(sp, "SLURM_MYOPT", opt1_value, 1);
    if (err != ESPANK_SUCCESS)
      slurm_error("%s %s: %s", PLUGIN_NAME, __func__, spank_strerror(err));
  }

  // here we loop around the argument vector to check that the global runtime option min_opt2 has 
  // been set - if it has we compare it to the argument provide by the user in opt2 - we then report 
  // if it's greater or less then min_opt2
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

// opt1 callback - just stores argument into opt1_value
static int _opt1_reg(int val, const char *optarg, int remote) {
  opt1_value = malloc(strlen(optarg) + 1);
  strcpy(opt1_value, optarg);
  
  return(ESPANK_SUCCESS);
}

// opt2 callback - just stores argument into opt2_value
static int _opt2_reg(int val, const char *optarg, int remote) {
  opt2_value = atoi(optarg);
  
  return(ESPANK_SUCCESS);
}