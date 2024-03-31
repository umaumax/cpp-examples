# stack-study

``` bash
g++ --std=c++20 -O3 main.cpp -o main
```

on ubuntu
``` bash
$ ./main
-----main----------------------
         Thread ID: 0x7f462a0ac740(139939329787712)
       arg address: 0x7ffc2b6f50cc
local var1 address: 0x7ffc2b6f50d8
local var2 address: 0x7ffc2b6f50e0
-------------------------------
-----pthread-------------------
[1]      Thread ID: 0x7f462a0ab640(139939329783360)
       arg address: 0x7f462a0aad98
local var1 address: 0x7f462a0aada8
local var2 address: 0x7f462a0aadb0
name=   main range=[0x7f46298ac000-0x7f462a0abfff] size=8388608 guard size=4096
-------------------------------
[0]      Thread ID: 0x7f462a0ab640(139939329783360)
       arg address: 0x7f462a0aacc8
local var1 address: 0x7f462a0aacd8
local var2 address: 0x7f462a0aace0
name=   main range=[0x7f46298ac000-0x7f462a0abfff] size=8388608 guard size=4096
-------------------------------
[1]      Thread ID: 0x7f46298aa640(139939321390656)
       arg address: 0x7f46298a9d98
local var1 address: 0x7f46298a9da8
local var2 address: 0x7f46298a9db0
name=   main range=[0x7f46290ab000-0x7f46298aafff] size=8388608 guard size=4096
-------------------------------
[0]      Thread ID: 0x7f46298aa640(139939321390656)
       arg address: 0x7f46298a9cc8
local var1 address: 0x7f46298a9cd8
local var2 address: 0x7f46298a9ce0
name=   main range=[0x7f46290ab000-0x7f46298aafff] size=8388608 guard size=4096
-------------------------------
-----std::thread----------------
[1]      Thread ID: 0x7f46298aa640(139939321390656)
       arg address: 0x7f46298a9d78
local var1 address: 0x7f46298a9d88
local var2 address: 0x7f46298a9d90
name=   main range=[0x7f46290ab000-0x7f46298aafff] size=8388608 guard size=4096
-------------------------------
[0]      Thread ID: 0x7f46298aa640(139939321390656)
       arg address: 0x7f46298a9ca8
local var1 address: 0x7f46298a9cb8
local var2 address: 0x7f46298a9cc0
name=   main range=[0x7f46290ab000-0x7f46298aafff] size=8388608 guard size=4096
-------------------------------
[1]      Thread ID: 0x7f462a0ab640(139939329783360)
       arg address: 0x7f462a0aad78
local var1 address: 0x7f462a0aad88
local var2 address: 0x7f462a0aad90
name=   main range=[0x7f46298ac000-0x7f462a0abfff] size=8388608 guard size=4096
-------------------------------
[0]      Thread ID: 0x7f462a0ab640(139939329783360)
       arg address: 0x7f462a0aaca8
local var1 address: 0x7f462a0aacb8
local var2 address: 0x7f462a0aacc0
name=   main range=[0x7f46298ac000-0x7f462a0abfff] size=8388608 guard size=4096
-------------------------------
```
