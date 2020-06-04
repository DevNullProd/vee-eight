const VEight = require('../lib/index').VEight

describe("#execute", () => {
  var v8

  beforeEach(() => {
    v8 = new VEight()
  })

  it("executes javascript code and returns the result", () => {
    expect(v8.execute("1")).toEqual(1)
    expect(v8.execute("'string'")).toEqual('string')
    expect(v8.execute("[1, 'a', 2.3]")).toEqual([1, 'a', 2.3])
    expect(v8.execute("a = {a : 1, b : { c : 3.4 }}")).toEqual({a : 1, b : { c : 3.4 }})
  })

  describe("syntax error", () => {
    it("throws a syntax error", () => {
      function err(){ v8.execute("{") }
      expect(err).toThrow(new SyntaxError('Unexpected end of input'))
    })
  })

  describe("javascript code throws error", () => {
    it("rethrows the error", () => {
      function err(){ v8.execute("s") }
      expect(err).toThrow(new Error('s is not defined'))
    })
  })
})
