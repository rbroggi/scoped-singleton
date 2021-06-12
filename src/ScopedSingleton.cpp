#include "ScopedSingleton.hpp"

#include <iostream>

namespace {
    void remoteSend(const json& log) {
        std::cout << pretty_print(log) << std::endl;
    }
}

void hello() {
    std::cout << "Hello, World!" << std::endl;
}


std::shared_ptr<json> StructuredLog() {
    // static reference to a non-owning json
    static std::weak_ptr<json> _logNonOwningRef;
    auto aLog = _logNonOwningRef.lock();
    // If resource is not initialized, do init it and keep it in the local instance
    if (not aLog) {
        aLog = std::shared_ptr<json>(new json, [](json* logPtr) {
            if (logPtr) {
                // network call to send event to log server
                remoteSend(*logPtr);
            }
            delete logPtr;
        });
        _logNonOwningRef = aLog;
    }
    return aLog;
}

HTTPHandler ScopedLogHandler(HTTPHandler handler) {
    return [&handler](HTTPResponse& resp, const HTTPRequest& req) {
      auto anchor = StructuredLog();
      handler(resp, req);
      //after
    };
}
