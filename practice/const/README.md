# const memo

```
int a = 123;
{
  const int* b = &a;
  b++;
}
{
  int* const b = &a;
  *b           = 1;
}
```
