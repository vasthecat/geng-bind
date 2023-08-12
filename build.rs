use std::process::Command;

fn main() {
    Command::new("make")
        .arg("-C")
        .arg("nauty")
        .arg("libnauty.a")
        .status()
        .unwrap();
    cc::Build::new()
        .file("nauty/geng.c")
        .file("nauty/geng-iter.c")
        .flag("-O3")
        .flag("-Wno-unused-parameter")
        .flag("-Wno-sign-compare")
        .flag("-Wno-unused-variable")
        .flag("-Wno-unused-function")
        .define("_XOPEN_SOURCE", None)
        .define("MAXN", "WORDSIZE")
        .define("WORDSIZE", "32")
        .define("OUTPROC", "myoutproc")
        .define("GENG_MAIN", "geng_main")
        .compile("geng");

    println!("cargo:rerun-if-changed=./nauty/geng.c");
    println!("cargo:rerun-if-changed=./nauty/geng-iter.c");
    println!("cargo:rustc-link-search=./nauty");
    println!("cargo:rustc-link-lib=static=nauty");
}
