use std::iter::Iterator;
use std::ptr;

#[allow(non_camel_case_types)]
enum geng_iterator {}

extern "C" {
    fn geng_iterator_create(
        iter: *const *mut geng_iterator,
        graph_size: usize,
        batch_size: usize,
    );
    fn geng_iterator_next(iter: *const geng_iterator, g: *mut u32) -> bool;
    fn geng_iterator_destroy(iter: *const geng_iterator);
    fn printgraph(g: *const u32, n: usize);
}

fn print_graph(g: Vec<u32>, n: usize) {
    unsafe {
        printgraph(g.as_ptr(), n);
    }
}

struct GengIterator {
    pub size: usize,
    iter: Box<geng_iterator>,
}

impl GengIterator {
    fn new(n: usize) -> GengIterator {
        let iter = unsafe {
            let iter: *mut geng_iterator = ptr::null_mut();
            geng_iterator_create(&iter, n, 10000);
            Box::from_raw(iter)
        };
        GengIterator { size: n, iter }
    }
}

impl Iterator for &GengIterator {
    type Item = Vec<u32>;

    fn next(&mut self) -> Option<Self::Item> {
        let mut g = vec![0; self.size];
        let res;
        unsafe {
            let ptr: *const geng_iterator = &*self.iter;
            res = geng_iterator_next(ptr, g.as_mut_ptr())
        }
        if res {
            Some(g)
        } else {
            None
        }
    }
}

impl Drop for GengIterator {
    fn drop(&mut self) {
        unsafe {
            let ptr: *const geng_iterator = &*self.iter;
            geng_iterator_destroy(ptr);
        }
    }
}

fn main() {
    let gi = GengIterator::new(3);

    for i in &gi {
        print_graph(i, gi.size);
    }

    // let q = gi.take(2000000).collect::<Vec<_>>();
    // println!("{}", q.len());
    // println!("{:?}", q);
    // for i in q {
    //     print_graph(i, gi.size);
    // }
}
