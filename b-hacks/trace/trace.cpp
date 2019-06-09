#include <cxxabi.h>
#include <dlfcn.h>

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <typeinfo>

extern "C" {
void __cyg_profile_func_enter(void* func_address, void* call_site);
void __cyg_profile_func_exit(void* func_address, void* call_site);
}

// NOTE: function address to function name using dynamic symbol table
// Please add -rdynamic build option.
std::string addr2name(void* address) {
  Dl_info dli;
  if (0 != dladdr(address, &dli)) {
    if (dli.dli_sname) {
      std::string mangled_name = std::string(dli.dli_sname);
      int status;
      // FYI: [libstdc\+\+: abi Namespace Reference]( https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html )
      char* demangled_name_c_str =
          abi::__cxa_demangle(mangled_name.c_str(), 0, 0, &status);
      if (demangled_name_c_str) {
        std::string demangled_name(demangled_name_c_str);
        free(demangled_name_c_str);
        return demangled_name;
      }
      return mangled_name;
    }
  }
  return "";
}

void __cyg_profile_func_enter(void* func_address, void* call_site) {
  std::string caller_func_name_str = addr2name(call_site);
  std::string caller_func_name     = std::string(
      caller_func_name_str != "" ? caller_func_name_str : "unknown funciton");

  std::string func_name_str = addr2name(func_address);
  std::string func_name =
      std::string(func_name_str != "" ? func_name_str : "unknown funciton");

  std::cerr << "[simple tracer][enter]: " << caller_func_name_str << " -> "
            << func_name << "" << std::endl;
}

void __cyg_profile_func_exit(void* func_address, void* call_site) {
  std::string caller_func_name_str = addr2name(call_site);
  std::string caller_func_name     = std::string(
      caller_func_name_str != "" ? caller_func_name_str : "unknown funciton");

  std::string func_name_str = addr2name(func_address);
  std::string func_name =
      std::string(func_name_str != "" ? func_name_str : "unknown funciton");

  std::cerr << "[simple tracer][exit ]: " << func_name << " -> "
            << caller_func_name_str << std::endl;
}
