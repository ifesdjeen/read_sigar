/**
 * Copyright (C) 20014 Oleksandr Petrov
 *
 * Author:
 *   Oleksandr Petrov <oleksandr.petrov at gmail.com>
 */

#include "config.h"
#include "math.h"
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

static void
submit_value (const char *plugin_instance,
              const char *type_instance,
              const char *type,
              value_t value)
{
  value_t values[1];
  value_list_t vl = VALUE_LIST_INIT;

  memcpy(&values[0], &value, sizeof(value));

  vl.values = values;
  vl.values_len = 1;

  sstrncpy (vl.host,            hostname_g,      sizeof (vl.host));
  sstrncpy (vl.plugin,          "read_sigar",    sizeof (vl.plugin));
  sstrncpy (vl.plugin_instance, plugin_instance, sizeof (vl.plugin));
  sstrncpy (vl.type,            type,            sizeof (vl.type));
  sstrncpy (vl.type_instance,   type_instance,   sizeof (vl.type_instance));

  plugin_dispatch_values (&vl);
}

static void
submit_gauge(const char *plugin_instance,
             const char *type_instance,
             const char *type,
             gauge_t percent)
{
  value_t value;
  value.gauge = percent;
  submit_value (plugin_instance, type_instance, type, value);
}

static void
submit_absolute(const char *plugin_instance,
                const char *type_instance,
                const char *type,
                absolute_t absolute)
{
  value_t value;

  value.absolute = absolute;
  submit_value (plugin_instance, type_instance, type, value);
}

static void
submit_derive(const char *plugin_instance,
              const char *type_instance,
              const char *type,
              derive_t derive)
{
  value_t value;

  value.derive = derive;
  submit_value (plugin_instance, type_instance, type, value);
}

static int sigar_read (void)
{
  int ret;

  {
    sigar_cpu_t cpu_jiffles_current;

    if (SIGAR_OK == (ret = sigar_cpu_get(sigar, &cpu_jiffles_current))) {

      sigar_cpu_perc_t perc;
      sigar_cpu_perc_calculate(&cpu_jiffles_snapshot, &cpu_jiffles_current, &perc);

      if (!isnan(perc.combined)) {
        value_t values[1];
        value_list_t vl = VALUE_LIST_INIT;

        vl.values = values;
        vl.values_len = STATIC_ARRAY_SIZE (values);

        sstrncpy (vl.host,            hostname_g,      sizeof (vl.host));
        sstrncpy (vl.plugin,          "read_sigar",    sizeof (vl.plugin));
        sstrncpy (vl.plugin_instance, "system.cpu",    sizeof (vl.plugin));
        sstrncpy (vl.type,            "percent",       sizeof (vl.type));
        /* sstrncpy (vl.type_instance,   "percent",       sizeof (vl.type_instance)); */
        vl.time = cdtime ();

        gauge_t user_value = (gauge_t) perc.user;
        gauge_t sys_value = (gauge_t) perc.sys;
        gauge_t nice_value = (gauge_t) perc.nice;
        gauge_t idle_value = (gauge_t) perc.idle;
        gauge_t wait_value = (gauge_t) perc.wait;
        gauge_t irq_value = (gauge_t) perc.irq;
        gauge_t soft_irq_value = (gauge_t) perc.soft_irq;
        gauge_t stolen_value = (gauge_t) perc.stolen;
        gauge_t combined_value = (gauge_t) perc.combined;

        plugin_dispatch_multivalue (&vl, 1,
                                    "user",      user_value,
                                    "sys",       sys_value,
                                    "nice",      nice_value,
                                    "idle",      idle_value,
                                    "wait",      wait_value,
                                    "irq",       irq_value,
                                    "soft_irq",  soft_irq_value,
                                    "stolen",    stolen_value,
                                    "combined",  combined_value,
                                    NULL);


      }

    } else {
      DEBUG ("sigar_cpu_get returned %i", res);
      return (-1);
    }
  }



  /* { */
  /*   sigar_mem_t memory; */
  /*   if (SIGAR_OK == sigar_mem_get(sigar, &memory)) { */
  /*     submit_absolute ("system.mem", "ram",           "absolute", memory.ram); */
  /*     submit_absolute ("system.mem", "total",         "absolute", memory.total); */
  /*     submit_absolute ("system.mem", "used",          "absolute", memory.used); */
  /*     submit_absolute ("system.mem", "free",          "absolute", memory.free); */
  /*     submit_absolute ("system.mem", "actual_used",   "absolute", memory.actual_used); */
  /*     submit_absolute ("system.mem", "actual_free",   "absolute", memory.actual_free); */

  /*     submit_gauge    ("system.mem.percent", "used_percent",  "percent", memory.used_percent); */
  /*     submit_gauge    ("system.mem.percent", "free_percent",  "percent", memory.free_percent); */
  /*   } else { */
  /*     DEBUG ("sigar_mem_get returned %i", res); */
  /*     return (-1); */
  /*   } */
  /* } */

  /* { */
  /*   sigar_net_stat_t network; */
  /*   if (SIGAR_OK == sigar_net_stat_get(sigar, &network, SIGAR_NETCONN_SERVER|SIGAR_NETCONN_TCP)) { */
  /*     submit_absolute ("system.net", "tcp_inbound_total",  "absolute", network.tcp_inbound_total); */
  /*     submit_absolute ("system.net", "tcp_outbound_total", "absolute", network.tcp_outbound_total); */
  /*     submit_absolute ("system.net", "all_inbound_total",  "absolute", network.all_inbound_total); */
  /*     submit_absolute ("system.net", "all_outbound_total", "absolute", network.all_outbound_total); */
  /*   } else { */
  /*     DEBUG ("sigar_mem_get returned %i", res); */
  /*     return (-1); */
  /*   } */
  /* } */

  /* { */
  /*   sigar_tcp_t tcp; */
  /*   if (SIGAR_OK == sigar_tcp_get(sigar, &tcp)) { */
  /*     submit_absolute ("system.tcp", "active_opens",   "absolute", tcp.active_opens); */
  /*     submit_absolute ("system.tcp", "passive_opens",  "absolute", tcp.passive_opens); */
  /*     submit_absolute ("system.tcp", "attempt_fails",  "absolute", tcp.attempt_fails); */
  /*     submit_absolute ("system.tcp", "estab_resets",   "absolute", tcp.estab_resets); */
  /*     submit_absolute ("system.tcp", "curr_estab",     "absolute", tcp.curr_estab); */
  /*     submit_absolute ("system.tcp", "in_segs",        "absolute", tcp.in_segs); */
  /*     submit_absolute ("system.tcp", "out_segs",       "absolute", tcp.out_segs); */
  /*     submit_absolute ("system.tcp", "retrans_segs",   "absolute", tcp.retrans_segs); */
  /*     submit_absolute ("system.tcp", "in_errs",        "absolute", tcp.in_errs); */
  /*     submit_absolute ("system.tcp", "out_rsts",       "absolute", tcp.out_rsts); */
  /*   } else { */
  /*     DEBUG ("sigar_mem_get returned %i", res); */
  /*     return (-1); */
  /*   } */
  /* } */

  return (0);
}

void
module_register (void)
{
  plugin_register_init ("read_sigar", init);
  plugin_register_read ("read_sigar", sigar_read);
}
