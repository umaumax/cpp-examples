# defer

* ラムダ式で返り値を変数で書き換える形式にすることで，defer内でのエラーハンドリングでの返り値を更新することができる

## FYI
* [defer っぽいもの \- Qiita]( https://qiita.com/SaitoAtsushi/items/afb428d0834ca4dda1e5 )
* [is the a practical way to emulate GO language defer in C or C\+\+ destructors? \- Stack Overflow]( https://stackoverflow.com/questions/48117908/is-the-a-practical-way-to-emulate-go-language-defer-in-c-or-c-destructors )
* [c\+\+ \- Order of member constructor and destructor calls \- Stack Overflow]( https://stackoverflow.com/questions/2254263/order-of-member-constructor-and-destructor-calls )
  * destructorの呼ばれる順番はconstructorの逆(同一スコープならば)
    * 今回の実装のように`std::stack`を明示的に利用する必要性は特にない
  * `std::shared_ptr`利用時でも問題ない?
