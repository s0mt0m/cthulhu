# cthulhu

## example usage

```cpp
// create a builder
cthu::builder::program b;

// add some stacks
auto s0 = b.add_stack();
auto s1 = b.add_stack();
auto s2 = b.add_stack();
auto s3 = b.add_stack();

// add some dictionaries
auto d0 = b.add_dict();
auto d1 = b.add_dict();

// add a value ‹2› to stack ‹s0›
s0.add( 2 );
// add an operation
s1.add( word_add( 1, 1, 2 ) );
// add a stack
s0.add( s1 );
// add a dictionary
s1.add( d0 );

s3.add( { 42, 3, 8  } );

// set stack at position 5 in dictionary d0 to s3
d1.set( 5, s3 );

// chain of ‹adds›
s0.add( 5 ).add( s1 ).add( 3 );

// add multiple values
s2.add( { 17, 9, 11 } );
s2.add( s0 ).add( { 4, 5, 0 } );

// both operations and values are represented
// using 32-bit unsigned integers, so you can
// add them with one ‹add›
s1.add( { word_mul( 2, 1, 2 ), 42, byte_sub( 0, 2, 1 ) } );

// create a program
// specify the initial configuration
// (up to 8 stacks, empty if unspecified)
auto p = b.build( { s0, s2 } );

// and execute it
exec( p ).start();
```
