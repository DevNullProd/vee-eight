#include "v-eight.h"
#include "common.h"

Nan::Persistent<v8::Function> VEight::constructor;

VEight::VEight(){}
VEight::~VEight(){}

void VEight::Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

  tpl->SetClassName(Nan::New("VEight").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "initialize", Initialize);
  Nan::SetPrototypeMethod(tpl, "reset", Reset);
  Nan::SetPrototypeMethod(tpl, "execute", Execute);

  auto function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);

  Nan::Set(exports, Nan::New("VEight").ToLocalChecked(), function);
}

NAN_METHOD(VEight::New) {
  if (info.IsConstructCall()) {
    VEight *obj = new VEight();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());

  } else {
    const int argc = 0;
    v8::Local<v8::Value> argv[argc] = { };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

NAN_METHOD(VEight::Initialize) {
  VEight* v_eight = ObjectWrap::Unwrap<VEight>(info.Holder());

  v_eight->Initialize();

  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(VEight::Reset) {
  VEight* v_eight = ObjectWrap::Unwrap<VEight>(info.Holder());

  v_eight->Reset();
}

NAN_METHOD(VEight::Execute) {
  NODE_ARG_STRING(0, "code");

  Nan::Utf8String code(info[0]);

  VEight* v_eight = ObjectWrap::Unwrap<VEight>(info.Holder());

  v8::MaybeLocal<v8::Value> result = v_eight->Execute(*code);

  if(!result.IsEmpty())
    info.GetReturnValue().Set(result.ToLocalChecked());
}

void VEight::Initialize() {
  vEightContext_.Reset(v8::Context::New(v8::Isolate::GetCurrent()));

  auto context = Nan::New(vEightContext_);

  v8::Context::Scope context_scope(context);

  auto global = context->Global();

  Nan::SetPrivate(global, Nan::New("v_eight").ToLocalChecked(),
            v8::External::New(v8::Isolate::GetCurrent(), this));
  Nan::Set(global, Nan::New("global").ToLocalChecked(), context->Global());
}

void VEight::Reset() {
  vEightContext_.Reset(v8::Context::New(v8::Isolate::GetCurrent()));
}

v8::MaybeLocal<v8::Value> VEight::Execute(const char *code) {
  // TODO 'always_reset' flag, invoke Reset() here if set

  auto context = Nan::New(vEightContext_);

  v8::Context::Scope context_scope(context);

  Nan::TryCatch tryCatch;

  v8::MaybeLocal<v8::Script> script = v8::Script::Compile(context, Nan::New(code).ToLocalChecked());

  if(tryCatch.HasCaught()){
    tryCatch.ReThrow();
    return v8::MaybeLocal<v8::Value>();
  }

  v8::MaybeLocal<v8::Value> maybe_result = script.ToLocalChecked()->Run(context);

  if(tryCatch.HasCaught()){
    tryCatch.ReThrow();
    return v8::MaybeLocal<v8::Value>();
  }

  return maybe_result;

// ... timeout
}
