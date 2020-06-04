# v-eight

Safely execute arbitrary untrusted JavaScript from nodejs. This module implements a lightweight isolated JavaScript environment that can be used to run any code without being able to escape the sandbox. The V8 context is initialized and executed entirely from C++ so it's impossible for the JS stack frames to lead back to the nodejs environment. It's usable from a nodejs process, but the JS environment is pure V8.

See **example.js** for usage

## Installation

```
npm install v-eight
```

Once installed, import it into your module via:

```javascript
const VEight = require('v-eight').VEight
```

## API

Initialize a new **VEight** instance:

```javascript
const v8 = new VEight()
```

Use **execute(code)** to run the specified code block:

```javascript
v8.execute("1+2")
// #=> 3
```

State will be preserved between function calls:

```javascript
v8.execute("var = 42")
v8.execute("var")
// # => 42
```

To clear the context invoke **reset()**:

```javascript
v8.reset()
v8.execute("var")
// # => ReferenceError: s is not defined
```

To limit long running operations, set a timeout with: **timeout(milliseconds)**:

```javascript
v8.timeout(1000) // 1 second timeout
v8.execute("while(true){}")
// # => timed out
```

## License

v-eight is licensed under **The MIT License**