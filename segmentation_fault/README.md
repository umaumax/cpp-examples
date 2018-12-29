# segmentation app

## linux
```
export LD_PRELOAD=/lib/x86_64-linux-gnu/libSegFault.so
export SEGFAULT_SIGNALS=all
./segmentation_fault_app
```

```
catchsegv ./segmentation_fault_app
```

## FYI
* add `-g` to compile option
