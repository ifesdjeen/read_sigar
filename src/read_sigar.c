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
submit_percent(const char *metric_type, const char *type_instance, gauge_t percent)
{
  value_t value;

  value.gauge = percent;
  submit_value (metric_type, type_instance, value);
}

static void
submit_absolute(const char *metric_type, const char *type_instance, absolute_t absolute)
{
  value_t value;

  value.absolute = absolute;
  submit_value (metric_type, type_instance, value);
}

static void
submit_derive(const char *metric_type, const char *type_instance, derive_t derive)
{
  value_t value;

  value.derive = derive;
  submit_value ("derive", type_instance, value);
}

static int sigar_read (void)
{
  int ret;

  {
    sigar_cpu_t cpu_jiffles_current;

    if (SIGAR_OK == (ret = sigar_cpu_get(sigar, &cpu_jiffles_current))) {
      sigar_cpu_perc_t perc;
      sigar_cpu_perc_calculate(&cpu_jiffles_snapshot, &cpu_jiffles_current, &perc);

      submit_percent("system.cpu", "user",      perc.user);
      submit_percent("system.cpu", "sys",       perc.sys);
      submit_percent("system.cpu", "nice",      perc.nice);
      submit_percent("system.cpu", "idle",      perc.idle);
      submit_percent("system.cpu", "wait",      perc.wait);
      submit_percent("system.cpu", "irq",       perc.irq);
      submit_percent("system.cpu", "soft_irq",  perc.soft_irq);
      submit_percent("system.cpu", "stolen",    perc.stolen);
      submit_percent("system.cpu", "combined",  perc.combined);
    } else {
      DEBUG ("sigar_cpu_get returned %i", res);
      return (-1);
    }
  }

  {
    sigar_mem_t memory;
    if (SIGAR_OK == sigar_mem_get(sigar, &memory)) {
      submit_absolute ("system.mem", "ram",           memory.ram);
      submit_absolute ("system.mem", "total",         memory.total);
      submit_absolute ("system.mem", "used",          memory.used);
      submit_absolute ("system.mem", "free",          memory.free);
      submit_absolute ("system.mem", "actual_used",   memory.actual_used);
      submit_absolute ("system.mem", "actual_free",   memory.actual_free);
      submit_percent  ("system.mem", "used_percent",  memory.used_percent);
      submit_percent  ("system.mem", "free_percent",  memory.free_percent);
    } else {
      DEBUG ("sigar_mem_get returned %i", res);
      return (-1);
    }
  }

  {
    sigar_net_stat_t network;
    if (SIGAR_OK == sigar_net_stat_get(sigar, &network, SIGAR_NETCONN_SERVER|SIGAR_NETCONN_TCP)) {
      submit_absolute ("system.net", "tcp_inbound_total",  network.tcp_inbound_total);
      submit_absolute ("system.net", "tcp_outbound_total", network.tcp_outbound_total);
      submit_absolute ("system.net", "all_inbound_total",  network.all_inbound_total);
      submit_absolute ("system.net", "all_outbound_total", network.all_outbound_total);
    } else {
      DEBUG ("sigar_mem_get returned %i", res);
      return (-1);
    }
  }

  {
    sigar_tcp_t tcp;
    if (SIGAR_OK == sigar_tcp_get(sigar, &tcp)) {
      submit_absolute ("system.tcp", "active_opens",   tcp.active_opens);
      submit_absolute ("system.tcp", "passive_opens",  tcp.passive_opens);
      submit_absolute ("system.tcp", "attempt_fails",  tcp.attempt_fails);
      submit_absolute ("system.tcp", "estab_resets",   tcp.estab_resets);
      submit_absolute ("system.tcp", "curr_estab",     tcp.curr_estab);
      submit_absolute ("system.tcp", "in_segs",        tcp.in_segs);
      submit_absolute ("system.tcp", "out_segs",       tcp.out_segs);
      submit_absolute ("system.tcp", "retrans_segs",   tcp.retrans_segs);
      submit_absolute ("system.tcp", "in_errs",        tcp.in_errs);
      submit_absolute ("system.tcp", "out_rsts",       tcp.out_rsts);
    } else {
      DEBUG ("sigar_mem_get returned %i", res);
      return (-1);
    }
  }


  return (0);
}

void
module_register (void)
{
  plugin_register_init ("read_sigar", init);
  plugin_register_read ("read_sigar", sigar_read);
}
