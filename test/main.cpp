#ifdef TESTING
#define RUSTY_SYNTAX_EXT
#include "mod.h"

/*
template<typename ... Args>
String format(const str fmt, Args&& ... args) {
    let_mut _v = std::vector<String>{ rstd::string::ToString<Args>::to_string(std::forward<Args>(args))... };
    let_mut vals = Vec<String>(std::move(_v));
    let_mut res = fmt.to_string();
    while (let first = vals.remove(0).if_let()) {
        res = res.replacen(STR("{}"), first->as_str(), 1);
    }
    return res;
}
*/


struct Foo {
    i32 x;
};

impl_Debug_all(    Foo, { f << "Foo { " << self.x << " }"; })
impl_PartialEq_all(Foo, { return self.x == rhs.x; })


fn main() -> int {
    let a = -10_i32;
    let b = a.pow(3);
    let c = ((f32)b);
    println("c = " << c);

    let f = Foo{ .x = 0 };
    let g = Foo{ .x = 5 };
    if (f == g) {
        println("f == g");
    } else if (f != g) {
        println("f != g");
    }

    let s1 = STR("hello").to_string();
    let s2 = (*s1).slice(1, 3);
    println(s2);

    let_mut s3 = format("{}", 5);
    println(s3);
    WRITE(s3, " extra text {} baba", 5);
    println(s3);

    let_mut x = Vec<String>::make();
    x.push(String::from("hello"));
    print("x = ")
    println(x);
}

#endif
