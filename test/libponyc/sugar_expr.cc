#include <gtest/gtest.h>
#include <platform.h>

#include "util.h"


// Tests for sugar that is applied during the expr pass

#define TEST_COMPILE(src) DO(test_compile(src, "expr"))
#define TEST_ERROR(src) DO(test_error(src, "expr"))
#define TEST_EQUIV(src, expect) DO(test_equiv(src, "expr", expect, "expr"))


class SugarExprTest : public PassTest
{};


/*
TEST_F(SugarExprTest, PartialWithTypeArgs)
{
  const char* short_form =
    "class Foo ref\n"
    "  fun f[A]() =>\n"
    "    this~f[A]()";

  TEST_ERROR(short_form);
}
*/


TEST_F(SugarExprTest, LambdaMinimal)
{
  const char* short_form =
    "class Foo ref\n"
    "  fun f() =>\n"
    "    lambda() => None end";

  const char* full_form =
    "class Foo ref\n"
    "  fun f() =>\n"
    "    object\n"
    "      fun apply() => None\n"
    "    end";

  TEST_EQUIV(short_form, full_form);
}


TEST_F(SugarExprTest, LambdaFull)
{
  const char* short_form =
    "trait A\n"
    "trait B\n"
    "trait C\n"
    "trait D\n"
    "trait D2 is D\n"

    "class Foo\n"
    "  fun f(c: C, d: D2) =>\n"
    "    lambda(a: A, b: B)(c, _c = c, _d: D = d): A => a end";

  const char* full_form =
    "trait A\n"
    "trait B\n"
    "trait C\n"
    "trait D\n"
    "trait D2 is D\n"

    "class Foo\n"
    "  fun f(c: C, d: D2) =>\n"
    "    object\n"
    "      let c: C = c\n"
    "      let _c: C = c\n"
    "      let _d: D = d\n"
    "      fun apply(a: A, b: B): A => a\n"
    "    end";

  TEST_EQUIV(short_form, full_form);
}


TEST_F(SugarExprTest, LambdaThrow)
{
  const char* short_form =
    "class Foo ref\n"
    "  fun f() =>\n"
    "    lambda() ? => error end";

  const char* full_form =
    "class Foo ref\n"
    "  fun f() =>\n"
    "    object\n"
    "      fun apply() ? => error\n"
    "    end";

  TEST_EQUIV(short_form, full_form);
}


TEST_F(SugarExprTest, LambdaWithTypeArgs)
{
  const char* short_form =
    "trait T\n"

    "class Foo ref\n"
    "  fun f() =>\n"
    "    lambda[A: T]() => None end";

  const char* full_form =
    "trait T\n"

    "class Foo\n"
    "  fun f() =>\n"
    "    object\n"
    "      fun apply[A: T]() => None\n"
    "    end";

  TEST_EQUIV(short_form, full_form);
}