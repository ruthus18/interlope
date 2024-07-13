

// TODO: load through iterators
fn main() {
    println!("cargo::rustc-link-search=./interlope/vendor");
    println!("cargo::rustc-link-search=./target/debug/build");
    println!("cargo::rustc-link-lib=interlope");
    println!("cargo::rustc-link-lib=GL");
    println!("cargo::rustc-link-lib=GLEW");
    println!("cargo::rustc-link-lib=glfw");
    println!("cargo::rustc-link-lib=cglm");
}
