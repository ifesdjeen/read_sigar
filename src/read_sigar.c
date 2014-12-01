/**
 * Copyright (C) 20014 Oleksandr Petrov
 *
 * Author:
 *   Oleksandr Petrov <oleksandr.petrov at gmail.com>
 */

#include "collectd.h"
#include "plugin.h"
#include "common.h"
#include "network.h"

#include "sigar.h"


static int
sigar_config (oconfig_item_t *ci)
{
  return (0);
}

void
module_register (void)
{
  plugin_register_complex_config ("read_sigar", sigar_config);
}


/* sigar_t *sigar_cpu; */
/* sigar_cpu_t old; */
/* sigar_cpu_t current; */

/* sigar_open(&sigar_cpu); */
/* sigar_cpu_get(sigar_cpu, &old); */

/* sigar_cpu_perc_t perc; */

/* while(1) */
/* { */
/*     sigar_cpu_get(sigar_cpu, &current); */
/*     sigar_cpu_perc_calculate(&old, &current, &perc); */

/*     std::cout << "CPU " << perc.combined * 100 << "%\n"; */
/*     old = current; */
/*     Sleep(100); */
/* } */

/* sigar_close(sigar_cpu); */
/* return 0; */
