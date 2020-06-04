const VEight = require('../lib/index').VEight

describe("#timeout", () => {
  var v8

  beforeEach(() => {
    v8 = new VEight()
    v8.initialize()
  })

  it("terminates long running code", () => {
    v8.timeout(100)

    function err(){ v8.execute("while(true){}") }
    expect(err).toThrow(new Error('timed out'))
  })
})
