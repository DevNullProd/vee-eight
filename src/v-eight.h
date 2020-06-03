#ifndef __V_EIGHT_WRAP_H__
#define __V_EIGHT_WRAP_H__

#include <nan.h>

class VEight : public Nan::ObjectWrap {
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  explicit VEight();

  ~VEight();

  static NAN_METHOD(New);

  static NAN_METHOD(Initialize);

  static NAN_METHOD(Reset);

  static NAN_METHOD(Execute);

  void Initialize();

  void Reset();

  v8::MaybeLocal<v8::Value> Execute(const char *code);

  Nan::Global<v8::Context> vEightContext_;

  static Nan::Persistent<v8::Function> constructor;
};

#endif
