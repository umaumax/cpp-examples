# time

## gettimeofday
REQUIRED:
```
#include <sys/time.h>
```
```
int64_t now_usec(){
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
}
```
