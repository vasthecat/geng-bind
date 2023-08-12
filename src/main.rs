use std::iter::Iterator;
use std::ptr;

#[allow(non_camel_case_types)]
enum geng_iterator {}

extern "C" {
    fn geng_iterator_init(iter: *const geng_iterator, n: i32);
    fn geng_iterator_next(iter: *const geng_iterator, g: *mut i32) -> bool;
    fn printgraph(g: *const i32, n: i32);
}

fn print_graph(g: Vec<i32>, n: usize) {
    unsafe {
        printgraph(g.as_ptr(), n as i32);
    }
}

struct GengIterator {
    pub size: usize,
}

impl GengIterator {
    fn new(n: usize) -> GengIterator {
        unsafe {
            geng_iterator_init(ptr::null(), n as i32);
        }
        GengIterator { size: n }
    }
}

impl Iterator for &GengIterator {
    type Item = Vec<i32>;

    fn next(&mut self) -> Option<Self::Item> {
        let mut g = vec![0; self.size];
        let res;
        unsafe { res = geng_iterator_next(ptr::null(), g.as_mut_ptr()) }
        if res {
            Some(g)
        } else {
            None
        }
    }
}

fn main() {
    let gi = GengIterator::new(12);

    let q = gi.skip(1000).next();
    println!("{:?}", q);
    print_graph(q.unwrap(), gi.size);
    // for i in &gi {
    //     print_graph(i, gi.size);
    // }
}
