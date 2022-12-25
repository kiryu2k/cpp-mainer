pub struct IntsRust {
    a: i8,
    b: i16,
    c: i8,
}

#[repr(C)]
pub struct IntsC {
    a: i8,
    b: i16,
    c: i8,
}

#[no_mangle]
pub unsafe extern "C" fn fill_rust(obj: *mut IntsRust) {
    (*obj).a = 2;
    (*obj).b = 8;
    (*obj).c = 16;
}

#[no_mangle]
pub unsafe extern "C" fn fill_c(obj: *mut IntsC) {
    (*obj).a = 2;
    (*obj).b = 8;
    (*obj).c = 16;
}
