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
#include "sigar_format.h"

sigar_t *sigar;
sigar_cpu_t cpu_jiffles_snapshot;

static int init (void)
{
  // TODO: add error handling
  sigar_open(&sigar);

  if (SIGAR_OK != sigar_cpu_get(sigar, &cpu_jiffles_snapshot)) {
    return (-1);
  }
  return (0);
}

static void submit_value (const char *type, const char *type_instance, value_t value)
{
	value_t values[1];
	value_list_t vl = VALUE_LIST_INIT;

	memcpy(&values[0], &value, sizeof(value));

	vl.values = values;
	vl.values_len = 1;

	sstrncpy (vl.host,          hostname_g,    sizeof (vl.host));
	sstrncpy (vl.plugin,        "read_sigar",  sizeof (vl.plugin));
	sstrncpy (vl.type,          type,          sizeof (vl.type));
  sstrncpy (vl.type_instance, type_instance, sizeof (vl.type_instance));

	plugin_dispatch_values (&vl);
}

static void
submit_percent(const char *type_instance, gauge_t percent)
{
	value_t value;

	value.gauge = percent;
	submit_value ("percent", type_instance, value);
}

static void
submit_absolute(const char *type_instance, absolute_t absolute)
{
	value_t value;

	value.absolute = absolute;
	submit_value ("absolute", type_instance, value);
}

static void
submit_derive(const char *type_instance, derive_t derive)
{
	value_t value;

	value.derive = derive;
	submit_value ("derive", type_instance, value);
}

static int sigar_read (void)
{
  sigar_cpu_t cpu_jiffles_current;

  if (SIGAR_OK == sigar_cpu_get(sigar, &cpu_jiffles_current)) {
    sigar_cpu_perc_t perc;
    sigar_cpu_perc_calculate(&cpu_jiffles_snapshot, &cpu_jiffles_current, &perc);

    submit_percent("system.cpu.user",     perc.user);
    submit_percent("system.cpu.sys",      perc.sys);
    submit_percent("system.cpu.nice",     perc.nice);
    submit_percent("system.cpu.idle",     perc.idle);
    submit_percent("system.cpu.wait",     perc.wait);
    submit_percent("system.cpu.combined", perc.combined);
  }

  return (0);
}

void
module_register (void)
{
	plugin_register_init ("read_sigar", init);
	plugin_register_read ("read_sigar", sigar_read);
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
