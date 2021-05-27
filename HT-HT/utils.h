inline long is_marked_ref(long i){
  return (long) (i & 0x1L);
}

inline long is_marked_ref2(long i){
  return (long) (i & 0x2L);
}

inline long unset_mark(long i){
  i &= ~0x1L;
  return i;
}

inline long unset_mark2(long i){
  i &= ~0x2L;
  return i;
}

inline long set_mark(long i){
  i |= 0x1L;
  return i;
}
inline long set_mark2(long i){
  i |= 0x2L;
  return i;
}

inline long get_unmarked_ref(long w){
  return w & ~0x1L;
}
inline long get_unmarked_ref2(long w){
  return w & ~0x2L;
}

inline long get_marked_ref(long w){
  return w | 0x1L;
}
inline long get_marked_ref2(long w){
  return w | 0x2L;
}