# constexpr

`constexpr`or`const`な値はheader only libraryとして利用可能

`constexpr`なfunctionは`inline`の意味を含む

[c\+\+ \- Does constexpr imply inline? \- Stack Overflow]( https://stackoverflow.com/questions/14391272/does-constexpr-imply-inline )

## constexpr std::string?
[c\+\+11 \- Is it possible to use std::string in a constexpr? \- Stack Overflow]( https://stackoverflow.com/questions/27123306/is-it-possible-to-use-stdstring-in-a-constexpr )

```
// NG
// constexpr std:string constString = "constString";

// OK
constexpr char constString[] = "constString";
```

## enum std::string
We can't use...

----

## FYI
### how to clean build
```
ninja -t clean constexpr_app
ninja constexpr_app
```
