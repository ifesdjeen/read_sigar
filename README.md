# Read Sigar

Read Sigar is a small and simple plugin for Collectd collects data from
Sigar and puts it into Collectd.

# Currently implemented metrics

  * `system.cpu.user`
  * `system.cpu.sys`
  * `system.cpu.nice`
  * `system.cpu.idle`
  * `system.cpu.wait`
  * `system.cpu.combined`

# Building

```
./setup.sh # For fetching collectd 
autoreconf --install
./configure --prefix /path/to/your_collectd
make
make install
```

# License

Licensed under the MIT License. 

Copyright (c) 2014 Oleksandr Petrov

