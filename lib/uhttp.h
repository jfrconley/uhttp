#include <string>
#include <vector>
#include "napi.h"
#include "uws/uWS.h"
#include "router/HttpRouter.hpp"

#ifndef UHTTP
#define UHTTP

class Server : public Napi::ObjectWrap<Server> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Server(const Napi::CallbackInfo& info);
    ~Server();

//    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);
private:

//    static napi_value New(napi_env env, napi_callback_info info);
    void Listen(const Napi::CallbackInfo& info);
//    static napi_value On(napi_env env, napi_callback_info info);
    static Napi::FunctionReference constructor;
    Napi::FunctionReference onrequest_;
    uWS::Group<uWS::SERVER> *group_;
};

class Request : public Napi::ObjectWrap<Request> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);
    Request(const Napi::CallbackInfo& info);
private:
    static Napi::FunctionReference constructor;
    Napi::Value On(const Napi::CallbackInfo& info);
    Napi::Value GetMethod(const Napi::CallbackInfo& info);
    Napi::Value GetUrl(const Napi::CallbackInfo& info);
    uWS::HttpMethod method = uWS::METHOD_GET;
    char* url;
    Napi::FunctionReference onend_;
    Napi::FunctionReference ondata_;
};

#endif