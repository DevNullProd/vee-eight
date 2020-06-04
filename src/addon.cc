#include "vee-eight.h"

NAN_MODULE_INIT(Init) {
  VeeEight::Init(target);
}

NODE_MODULE(VeeEight, Init)
