const VEight = require('../lib/index').VEight

describe("#reset", () => {
  var v8

  beforeEach(() => {
    v8 = new VEight()
    v8.initialize()
  })

  it("resets javascript context", () => {
    expect(v8.execute("a = 1")).toEqual(1)
    expect(v8.execute("a")).toEqual(1)

    v8.reset()
    function err(){ v8.execute("a") }
    expect(err).toThrow(new Error('a is not defined'))
  })
})
