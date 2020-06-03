#define NODE_ARG_STRING(num, name)                                                  \
  if (info.Length() < num + 1) {                                                    \
    return Nan::ThrowTypeError((std::string(name) + " must be given").c_str());     \
  }                                                                                 \
  if (!info[num]->IsString()) {                                                     \
    return Nan::ThrowTypeError((std::string(name) + " must be a string").c_str()); \
  }
