#include "vee-eight.h"
#include "common.h"

const std::chrono::milliseconds TERMINATE_DELAY(100);

void terminate_loop(VeeEight& veeEight){
  while(!veeEight.shutdown()){
    if(veeEight.timeout().count() > 0 && veeEight.executing()){
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now() - veeEight.execution_started()
      );

      if(elapsed > veeEight.timeout())
        veeEight.Terminate();
    }
    std::this_thread::sleep_for(TERMINATE_DELAY);
  }
}

Nan::Persistent<v8::Function> VeeEight::constructor_;

bool VeeEight::shutdown() const{
  return shutdown_;
}

bool VeeEight::executing() const{
  return executing_;
}

std::chrono::milliseconds VeeEight::timeout() const{
  return timeout_;
}

std::chrono::system_clock::time_point VeeEight::execution_started() const{
  return execution_started_;
}

VeeEight::VeeEight() :
  shutdown_(false),
  executing_(false),
  timed_out_(false),
  timeout_(-1),
  terminate_thread_(&terminate_loop, std::ref(*this)) {

}

VeeEight::~VeeEight(){
  shutdown_ = true;
  terminate_thread_.join();
}

void VeeEight::Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

  tpl->SetClassName(Nan::New("VeeEight").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "timeout", Timeout);
  Nan::SetPrototypeMethod(tpl, "reset", Reset);
  Nan::SetPrototypeMethod(tpl, "execute", Execute);

  auto function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor_.Reset(function);

  Nan::Set(exports, Nan::New("VeeEight").ToLocalChecked(), function);
}

NAN_METHOD(VeeEight::New) {
  if (info.IsConstructCall()) {
    VeeEight *obj = new VeeEight();
    obj->Wrap(info.This());
    obj->Initialize();
    info.GetReturnValue().Set(info.This());

  } else {
    const int argc = 0;
    v8::Local<v8::Value> argv[argc] = { };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor_);
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

NAN_METHOD(VeeEight::Timeout) {
  NODE_ARG_INTEGER(0, "timeout");

  VeeEight* v_eight = ObjectWrap::Unwrap<VeeEight>(info.Holder());

  v_eight->timeout_ = std::chrono::duration<uint32_t, std::milli>(Nan::To<uint32_t>(info[0]).FromJust());
}

NAN_METHOD(VeeEight::Reset) {
  VeeEight* v_eight = ObjectWrap::Unwrap<VeeEight>(info.Holder());

  v_eight->Reset();
}

NAN_METHOD(VeeEight::Execute) {
  NODE_ARG_STRING(0, "code");

  Nan::Utf8String code(info[0]);

  VeeEight* v_eight = ObjectWrap::Unwrap<VeeEight>(info.Holder());

  v8::MaybeLocal<v8::Value> result = v_eight->Execute(*code);

  if(!result.IsEmpty())
    info.GetReturnValue().Set(result.ToLocalChecked());
}

void VeeEight::Initialize() {
  isolate_ = v8::Isolate::GetCurrent();
  global_context_.Reset(v8::Context::New(isolate_));

  auto context = Nan::New(global_context_);

  v8::Context::Scope context_scope(context);

  auto global = context->Global();

  Nan::SetPrivate(global, Nan::New("v_eight").ToLocalChecked(),
                            v8::External::New(isolate_, this));
  Nan::Set(global, Nan::New("global").ToLocalChecked(), context->Global());
}

void VeeEight::Reset() {
  global_context_.Reset(v8::Context::New(isolate_));
}

v8::MaybeLocal<v8::Value> VeeEight::Execute(const char *code) {
  // TODO 'always_reset' flag, invoke Reset() here if set

  auto context = Nan::New(global_context_);

  v8::Context::Scope context_scope(context);

  Nan::TryCatch tryCatch;

  v8::MaybeLocal<v8::Script> script = v8::Script::Compile(context, Nan::New(code).ToLocalChecked());

  if(tryCatch.HasCaught()){
    tryCatch.ReThrow();
    return v8::MaybeLocal<v8::Value>();
  }

  execution_started_ = std::chrono::system_clock::now();
  timed_out_ = false;
  executing_ = true;

  v8::MaybeLocal<v8::Value> maybe_result = script.ToLocalChecked()->Run(context);

  executing_ = false;

  if(timed_out_){
    isolate_->ThrowException(v8::String::NewFromUtf8(isolate_, "timed out"));
  }

  if(tryCatch.HasCaught()){
    tryCatch.ReThrow();
    return v8::MaybeLocal<v8::Value>();
  }

  return maybe_result;
}

// TODO 'safe_execute' method, same as execute but do not throw error

void VeeEight::Terminate() {
  timed_out_ = true;
  isolate_->TerminateExecution();
}
