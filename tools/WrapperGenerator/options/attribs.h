// range 'expression to calculate the range'
#define _range_(x) __attribute((gccxml("range", #x)))

// defaults to handle without range
// generate 'true/false to say if a pointer wrapper should be generated' 'true/false to say if a handle wrapper should be generated' 'the range to use to validate pointers'
// must add range attribute later so that you can validate the memory. maybe add native_pointer that returns a native pointer directly
// don't support permutations of these either yet. If you have an argument of handle type it will generate a functions where all pointer arguments are handles and vice versa.
#define _use_type_(pointer, handle) __attribute((gccxml("use_pointer", #pointer), gccxml("use_handle", #handle)))
