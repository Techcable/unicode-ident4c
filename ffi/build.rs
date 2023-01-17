use std::path::PathBuf;

fn main() {
    let out_dir = PathBuf::from(std::env::var_os("OUT_DIR").unwrap());
    let root = PathBuf::from(std::env::var_os("CARGO_MANIFEST_DIR").unwrap());

    println!("cargo:rustc-link-lib=ident4c");

    let build_dir = root.join("build");
    assert!(build_dir.is_dir(), "Missing build dir");

    println!("cargo:rustc-link-search=native={}", build_dir.display());

    let bindings = bindgen::builder()
        .header(root.join("include/ident4c.h").to_str().unwrap())
        .allowlist_function("ident4c_.*")
        .allowlist_var("_?IDENT4C_.*")
        // We have out-of-line declarations for these too
        .generate_inline_functions(true)
        .generate().unwrap();

    bindings.write_to_file(out_dir.join("bindings.rs")).unwrap();

}
