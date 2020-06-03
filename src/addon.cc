#include "v-eight.h"

NAN_MODULE_INIT(Init) {
  VEight::Init(target);
}

NODE_MODULE(VEight, Init)
