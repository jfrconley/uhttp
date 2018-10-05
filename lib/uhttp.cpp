#include "uhttp.h"
//
//
//static napi_ref on_fatal_exception;
//
//std::string testResponse = "Test response";
//
//std::string strMethod(HttpMethod method) {
//    switch (method) {
//        case METHOD_GET:
//            return "GET";
//        case METHOD_POST:
//            return "POST";
//        case METHOD_DELETE:
//            return "DELETE";
//        case METHOD_PUT:
//            return "PUT";
//        case METHOD_PATCH:
//            return "PATCH";
//        default:
//            return "GET";
//    }
//}
//
//RequestHandler::RequestHandler(uhttp_server *server) : self(server) {}
//void RequestHandler::convertRequest(uWS::HttpRequest *req, napi_value *out) const {
//    Header reqUrl = req->getUrl();
//    std::string reqMethod = strMethod(req->getMethod());
//    size_t headerLen = strlen(req->headers->key);
//    napi_value jsUrl;
//    napi_value jsMethod;
//    napi_value jsHeader;
//
//    napi_create_object(self->env, out);
//    napi_create_string_utf8(self->env, reqUrl.value, reqUrl.valueLength, &jsUrl);
//    napi_set_named_property(self->env, *out, "url", jsUrl);
//    napi_create_string_utf8(self->env, reqMethod.data(), reqMethod.length(), &jsMethod);
//    napi_set_named_property(self->env, *out, "method", jsMethod);
//    napi_create_buffer(self->env, headerLen, (void**) &(req->headers->key), &jsHeader);
//    napi_set_named_property(self->env, *out, "headerBuffer", jsHeader);
//}
//
//void RequestHandler::operator()(uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length,
//                                size_t remainingBytes) const {
//    UHTTP_CALLBACK(self->on_request,
//    napi_value obj;
//    convertRequest(&req, &obj);
//    NAPI_MAKE_CALLBACK_FATAL(1, &obj, nullptr)
//    )
//    res->end(testResponse.data(), testResponse.length());
//}
//
//
//NAPI_METHOD(init) {
//    NAPI_ARGV(3)
//    NAPI_ARGV_BUFFER_CAST(uhttp_server *, self, 0)
//    int result = 0;
//
//    Group<SERVER> *group = hub.createGroup<SERVER>();
//    RequestHandler *handler = new RequestHandler(self);
//    group->onHttpRequest(*handler);
//    self->group = group;
//    self->env = env;
//    self->handler = handler;
//    napi_create_reference(env, argv[1], 1, &(self->ctx));
//    napi_create_reference(env, argv[2], 1, &(self->on_request));
//    NAPI_RETURN_INT32(result)
//}
//
//NAPI_METHOD(listen) {
//    NAPI_ARGV(4)
//    NAPI_ARGV_BUFFER_CAST(uhttp_server *, self, 0)
//    NAPI_ARGV_INT32(port, 1)
//    NAPI_ARGV_UTF8(ip, 17, 2)
//
//    int result = hub.listen(ip, port, nullptr, 0, self->group);
//    if (result) {
//        printf("Started server\n");
//    }
//    NAPI_RETURN_INT32(result)
//}
//
//NAPI_METHOD(route) {
//    NAPI_ARGV(1)
//    NAPI_ARGV_BUFFER_CAST(uhttp_server *, self, 0)
////    NAPI_ARGV_UINT32(method_length, 1)
////    NAPI_ARGV_UTF8(method, method_length, 2)
////    NAPI_ARGV_UINT32(url_length, 3)
////    NAPI_ARGV_UTF8(url, url_length, 4)
////
////    int result = 1;
////    napi_ref jsHandler;
////    auto * router = self->router;
////
////    napi_create_reference(self->env, argv[5], 1, &jsHandler);
//
//    auto thing = *(new RouterHandler(self, nullptr));
////    router->add(method, url, *handler);
//
//    return nullptr;
//}
//
//NAPI_METHOD(close) {
//    NAPI_ARGV(3)
//    NAPI_ARGV_BUFFER_CAST(uhttp_server *, self, 0)
//
//    Group<SERVER> *group = self->group;
//    group->close();
//
//    napi_delete_reference(env, self->on_request);
//    napi_delete_reference(env, self->ctx);
////    self->routerHandlers->clear();
////    delete self->routerHandlers;
//    delete self->handler;
//    delete group;
////    delete self->router;
//    return nullptr;
//}
//
//NAPI_METHOD(uhttp_on_fatal_exception) {
//    NAPI_ARGV(1)
//
//    napi_create_reference(env, argv[0], 1, &on_fatal_exception);
//
//    return nullptr;
//}
//
//NAPI_INIT() {
//    NAPI_EXPORT_FUNCTION(init)
//    NAPI_EXPORT_FUNCTION(listen)
//    NAPI_EXPORT_FUNCTION(close)
//    NAPI_EXPORT_FUNCTION(route)
//    NAPI_EXPORT_FUNCTION(uhttp_on_fatal_exception)
//    NAPI_EXPORT_SIZEOF(uhttp_server)
//}

using namespace uWS;
Hub hub(0, true);

std::string TestResponse = "yay!";

Napi::FunctionReference Server::constructor;

Napi::Object Server::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Server", {
        InstanceMethod("listen", &Server::Listen)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Server", func);
    return exports;
}

Server::Server(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Server>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    size_t length = info.Length();

    if (length<=0 || !info[0].IsFunction()) {
        Napi::TypeError::New(env, "Request callback expected").ThrowAsJavaScriptException();
    }

    Napi::Function onrequest = info[0].As<Napi::Function>();
    onrequest_ = Napi::Persistent(onrequest);
    Napi::FunctionReference *onrequestptr = &onrequest_;
    group_ = hub.createGroup<SERVER>();
    group_->onHttpRequest([onrequestptr, env](HttpResponse *res, HttpRequest req, char *data, size_t length, size_t remainingBytes) {
//        Napi::HandleScope scope(env);
        onrequestptr->Call(env.Global(), {Napi::String::New(env, "potato")});
        res->end(TestResponse.data(), TestResponse.length());
    });
}

Server::~Server() {
    group_->close();
    delete group_;
};

void Server::Listen(const Napi::CallbackInfo &info) {
    size_t length = info.Length();
    Napi::Env env = info.Env();
    std::string address = "0.0.0.0";
    int port;
    Napi::Function cb;

    if (length<=0) {
        Napi::TypeError::New(env, "Not enough arguments").ThrowAsJavaScriptException();
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "port must be a number").ThrowAsJavaScriptException();
    }

    port = info[0].As<Napi::Number>().Int32Value();

    if (length>=2 && info[1].IsString()) {
        address = info[1].As<Napi::String>().Utf8Value();
    }

    hub.listen(address.data(), port, nullptr, 0, group_);
}

Napi::FunctionReference Request::constructor;

Napi::Object Request::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);
    Napi::Function func = DefineClass(env, "Request", {
        InstanceAccessor("method", &Request::GetMethod, nullptr),
        InstanceAccessor("url", &Request::GetUrl, nullptr),
        InstanceMethod("on", &Request::On)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Request", func);
    return exports;
}

Request::Request(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Request>(info), url(nullptr) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);


}


Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports = Server::Init(env, exports);
    return Request::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)