# Read Sigar

Read Sigar is a small and simple plugin for Collectd collects data from
Sigar and puts it into Collectd.

# Currently implemented metrics

## CPU 

  * `system.cpu.user`: percent (gauge)
  * `system.cpu.sys`: percent (gauge)
  * `system.cpu.nice`: percent (gauge)
  * `system.cpu.idle`: percent (gauge)
  * `system.cpu.wait`: percent (gauge)
  * `system.cpu.irq`: percent (gauge)
  * `system.cpu.sof`: percent (gauge)
  * `system.cpu.stolen`: percent (gauge)
  * `system.cpu.combined`: percent (gauge)
  
## Memory

  * `system.mem.ram`: bytes (absolute)
  * `system.mem.total`: bytes (absolute)
  * `system.mem.used`: bytes (absolute)
  * `system.mem.free`: bytes (absolute)
  * `system.mem.actual_used`: bytes (absolute)
  * `system.mem.actual_free`: bytes (absolute)
  * `system.mem.used_percent`: percent (gauge)
  * `system.mem.free_percent`: percent (gauge)

## Network

  * `system.net.tcp_inbound_total`: count (absolute)
  * `system.net.tcp_outbound_total`: count (absolute)
  * `system.net.all_inbound_total`: count (absolute)
  * `system.net.all_outbound_total`: count (absolute)

## TCP

  * `system.tcp.active_opens`: count (absolute)
  * `system.tcp.passive_opens`: count (absolute)
  * `system.tcp.attempt_fails`: count (absolute)
  * `system.tcp.estab_resets`: count (absolute)
  * `system.tcp.curr_estab`: count (absolute)
  * `system.tcp.in_segs`: count (absolute)
  * `system.tcp.out_segs`: count (absolute)
  * `system.tcp.retrans_segs`: count (absolute)
  * `system.tcp.in_errs`: count (absolute)
  * `system.tcp.out_rsts`: count (absolute)

# Building

```
./setup.sh # For fetching collectd 
autoreconf --install
./configure --prefix /path/to/your_collectd
make
make install
```

In order for the package to work with sigar itself, you'll have to have sigar
installed on your machine. 

# License

Licensed under the MIT License. 

Copyright (c) 2014 Oleksandr Petrov

