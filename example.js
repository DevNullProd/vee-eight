const VeeEight = require('./lib/index').VeeEight
const v8 = new VeeEight()

// Primitive types:

console.log(v8.execute("1"))
// # => 1

console.log(v8.execute("'123'"))
// # => '123'

// Standard javascript functions:

console.log(v8.execute("parseInt('123')"))
// # => 123

// Arrays and objects:

console.log(v8.execute("[1,'a',3.5]"))
// # => [1, 'a', 3.5]

console.log(v8.execute("a = {b:2, c:3}"))
// # => {b: 2, c: 3}

// Errors:

try{
  v8.execute("s")
}catch(err){
  console.error(err)
  // # => ReferenceError: s is not defined
}

try{
  console.log(v8.execute("{"))
}catch(err){
  console.error(err)
  // # => SyntaxError: Unexpected end of input
}

// State is maintained:
console.log(v8.execute("variable = 'something'"))
// # => something

console.log(v8.execute("variable"))
// # => something

// Invoke 'reset' to restore initial context:
v8.reset()

try{
  v8.execute("variable")
}catch(err){
  console.error(err)
  // # => ReferenceError: variable is not defined
}

// Set timeout to terminate long running scripts:

v8.timeout(1000)

try{
  console.log(v8.execute("while(true){}"))
}catch(e){
  console.log(e)
  // # => timed out
}
