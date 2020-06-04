#ifndef __V_EIGHT_WRAP_H__
#define __V_EIGHT_WRAP_H__

#include <thread>
#include <chrono>

#include <nan.h>

class VEight : public Nan::ObjectWrap {
public:
  static void Init(v8::Local<v8::Object> exports);

  bool shutdown() const;

  bool executing() const;

  std::chrono::milliseconds timeout() const;

  std::chrono::system_clock::time_point execution_started() const;

  void Terminate();

private:
  explicit VEight();

  ~VEight();

  static NAN_METHOD(New);

  static NAN_METHOD(Initialize);

  static NAN_METHOD(Timeout);

  static NAN_METHOD(Reset);

  static NAN_METHOD(Execute);

  void Initialize();

  void Reset();

  v8::MaybeLocal<v8::Value> Execute(const char *code);

  Nan::Global<v8::Context> global_context_;

  v8::Isolate* isolate_;

  static Nan::Persistent<v8::Function> constructor_;

  bool shutdown_;

  bool executing_;

  bool timed_out_;

  std::chrono::milliseconds timeout_;

  std::chrono::system_clock::time_point execution_started_;

  std::thread terminate_thread_;
};

#endif
