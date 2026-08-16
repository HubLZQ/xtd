
#include <memory>
#include <vector>
#include <random>
#include <algorithm>
#include <random>

#include <xtd/Xtd.h>

int main()
{
    auto xd = xtd::X_PI;
    auto s1 = xtd::ToI32("123");
    auto s2 = xtd::ToF32("123.34134");
    auto s3 = xtd::text::StrToUtf16("Hello, World!说的分手费", -1);
    auto s4 = xtd::text::StrToUtf32("Hello, World!说的分手费", -1);
    auto s5 = xtd::text::StrToUtf8("Hello, World!说的分手费", -1);
    return 0; 
}
