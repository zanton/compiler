#pragma once

#include <stdio.h>

#include "syntree.h"
#include "util2.h"

/*******************************************************/
/* Environment Struct */

typedef struct env * env_t;

struct env
{
  var_list_t varl;
  env_t penv;
};

// make an empty environment object
env_t mk_env();

// make an environment object with given parent-environment-object
env_t mk_env_with_penv(env_t pe);

// add an variable (function) into env
int env_add(env_t env, char * name, storage_t addr); // also check identity

// get the address of variable x from env
storage_t env_get_addr(env_t e, char * x);

int env_set_params(var_decl_list_t params, env_t env);
