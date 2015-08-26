README - Phase C Requirements

1. Don’t allow your templates to be instantiated unless the arguments are the correct
type. E.g., your operator<< should get used if the argument is an Expression class,
but shouldn’t get used if I want to output some random Foo object. Your templates should
not interfere with me if I want to write a
template <typename T1,typename T2> T1 operator+(T1 x, T2 y);

2. Solve this project using as few operator functions as possible. I’d like to say,
“solve the project using the shortest solution you can come up with”, but it’s not
really lines of code that matter, it’s the number of functions and the number of
template classes that you want to keep under control. Note: We will be manually
reading your code, so please also keep to a clean format.

3. All Phase A and Phase B requirements apply.