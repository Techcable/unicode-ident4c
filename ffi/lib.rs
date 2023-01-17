pub mod raw {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

#[inline]
pub fn is_xid_start(ch: char) -> bool {
    unsafe { self::raw::ident4c_is_xid_start(ch as u32) }
}

#[inline]
pub fn is_xid_continue(ch: char) -> bool {
    unsafe { self::raw::ident4c_is_xid_continue(ch as u32) }
}
