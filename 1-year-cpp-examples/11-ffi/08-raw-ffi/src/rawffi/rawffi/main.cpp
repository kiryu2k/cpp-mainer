#include <ffi.h>

#include <dlfcn.h>
#include <gnu/lib-names.h>

#include <iostream>
#include <vector>

int main() {
  void* handle = dlopen(LIBM_SO, RTLD_LAZY);
  if (handle == nullptr) {
    std::cout << dlerror() << '\n';
    return 1;
  }

  void* pow_ptr = dlsym(handle, "pow");

  // Если сигнатура целевой функции известна на этапе компиляции, то можно
  // выполнить приведение указателя.
  using PowFn = double (*)(double, double);
  auto pow_fn = reinterpret_cast<PowFn>(pow_ptr);
  std::cout << "2^4 = " << pow_fn(2, 4) << '\n';

  // libffi позволяет вызвать функцию, не зная ее сигнатуры на этапе компиляции.
  std::vector arg_types{&ffi_type_double, &ffi_type_double};

  // cif == Call InterFace
  ffi_cif cif;
  if (const auto status = ffi_prep_cif(
          &cif,
          FFI_DEFAULT_ABI,
          arg_types.size(),
          &ffi_type_double,
          arg_types.data());
      status != FFI_OK) {
    // TODO: handle error
    std::cout << "ffi_prep_cif error\n";
    return 1;
  }

  double result = 0;
  std::vector args = {2.0, 4.0};
  std::vector<void*> arg_ptrs = {&args[0], &args[1]};
  using VoidFn = void (*)();
  ffi_call(&cif, reinterpret_cast<VoidFn>(pow_ptr), &result, arg_ptrs.data());

  std::cout << "2^4 = " << result << '\n';

  dlclose(handle);
}
