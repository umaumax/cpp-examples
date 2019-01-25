#include <cassert>
#include <cfenv>
#include <cstdio>
#include <limits>
#include <string>
#include <typeinfo>

int main() {
  {
    double x  = std::numeric_limits<double>::max();
    double xx = x * x;

    // NOTE: if you want to use dummy value (you can't use string because std::to_string can't accept not number)
#undef VAR_NAME
#define VAR_NAME(x) (#x "")
    !std::fetestexcept(FE_OVERFLOW) || !std::fprintf(stderr, "%s:%d RAISED FLOATING-POINT OVERFLOW: %s %s = %s\n", __FILE__, __LINE__, typeid(xx).name(), VAR_NAME(xx), std::to_string(xx).c_str());
    !std::fetestexcept(FE_UNDERFLOW) || !std::fprintf(stderr, "%s:%d RAISED FLOATING-POINT UNDERFLOW: %s %s = %s\n", __FILE__, __LINE__, typeid(xx).name(), VAR_NAME(xx), std::to_string(xx).c_str());
    //     (std::feclearexcept(FE_ALL_EXCEPT) == 0) || !std::fprintf(stderr, "%s:%d FE_ALL_EXCEPT FAILED\n", __FILE__, __LINE__);
    std::fprintf(stderr, "%s:%d FE_ALL_EXCEPT %s\n", __FILE__, __LINE__, (std::feclearexcept(FE_ALL_EXCEPT) == 0) ? "SUCCESS" : "FAILED");
  }

  {
    float f_overflow = std::numeric_limits<double>::max();

    // NOTE: if you want to use dummy value (you can't use string because std::to_string can't accept not number)
#undef VAR_NAME
#define VAR_NAME(x) (#x "")
    !std::fetestexcept(FE_OVERFLOW) || !std::fprintf(stderr, "%s:%d RAISED FLOATING-POINT OVERFLOW: %s %s = %s\n", __FILE__, __LINE__, typeid(f_overflow).name(), VAR_NAME(f_overflow), std::to_string(f_overflow).c_str());
    !std::fetestexcept(FE_UNDERFLOW) || !std::fprintf(stderr, "%s:%d RAISED FLOATING-POINT UNDERFLOW: %s %s = %s\n", __FILE__, __LINE__, typeid(f_overflow).name(), VAR_NAME(f_overflow), std::to_string(f_overflow).c_str());
    //     (std::feclearexcept(FE_ALL_EXCEPT) == 0) || !std::fprintf(stderr, "%s:%d FE_ALL_EXCEPT FAILED\n", __FILE__, __LINE__);
    std::fprintf(stderr, "%s:%d FE_ALL_EXCEPT %s\n", __FILE__, __LINE__, (std::feclearexcept(FE_ALL_EXCEPT) == 0) ? "SUCCESS" : "FAILED");
  }

  {
    float x = 1e-30f;
    float f_underflow = x * x;

    // NOTE: if you want to use dummy value (you can't use string because std::to_string can't accept not number)
#undef VAR_NAME
#define VAR_NAME(x) (#x "")
    !std::fetestexcept(FE_OVERFLOW) || !std::fprintf(stderr, "%s:%d RAISED FLOATING-POINT OVERFLOW: %s %s = %s\n", __FILE__, __LINE__, typeid(f_underflow).name(), VAR_NAME(f_underflow), std::to_string(f_underflow).c_str());
    !std::fetestexcept(FE_UNDERFLOW) || !std::fprintf(stderr, "%s:%d RAISED FLOATING-POINT UNDERFLOW: %s %s = %s\n", __FILE__, __LINE__, typeid(f_underflow).name(), VAR_NAME(f_underflow), std::to_string(f_underflow).c_str());
    //     (std::feclearexcept(FE_ALL_EXCEPT) == 0) || !std::fprintf(stderr, "%s:%d FE_ALL_EXCEPT FAILED\n", __FILE__, __LINE__);
    std::fprintf(stderr, "%s:%d FE_ALL_EXCEPT %s\n", __FILE__, __LINE__, (std::feclearexcept(FE_ALL_EXCEPT) == 0) ? "SUCCESS" : "FAILED");
  }

  return 0;
}
