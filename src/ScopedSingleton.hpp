#pragma once

#include <jsoncons/json.hpp>
#include <functional>
#include <map>
#include <memory>

void hello();

/**
 * HTTPStatus contains the status codes of an HTTP Response
 */
enum HTTPStatus {
    HTTPStatusOk = 200,
    HTTPBadRequest = 400,
    HTTPNotFound = 400,
    HTTPInternalServerError = 500,
};

/**
 * HTTPMethod contains the supported HTTP Methods
 */
enum class HTTPMethod {
    GET, POST, PUT, DELETE
};

/**
 * HTTPParameters contains the HTTP Parameters
 */
using HTTPParameters = std::map<std::string,std::string>;

/**
 * HTTPParameters contains the HTTP Parameters
 */
using HTTPParameters = std::map<std::string,std::string>;

/**
 * HTTPHeaders contains the HTTP Headers
 */
using HTTPHeaders = std::map<std::string,std::string>;

/**
 * HTTPRequest contains all HTTP Request parameters
 */
struct HTTPRequest {
    HTTPMethod method;
    HTTPParameters parameters;
    HTTPHeaders headers;
    std::string uri;
    std::string body;
};

/**
 * HTTPResponse contains all HTTP Response parameters
 */
struct HTTPResponse {
    HTTPHeaders headers;
    HTTPStatus status;
    std::string body;
};

/**
 * HTTPHandler resp to be enriched and a request
 * @param resp
 * @param req
 */
using HTTPHandler = std::function<void(HTTPResponse& resp, const HTTPRequest& req)>;

using jsoncons::json;
/**
 * StructuredLog returns a valid (never nullptr) pointer to a scoped-singleton json
 * @return
 */
std::shared_ptr<json> StructuredLog();

/**
 * @param handler is an inner handler
 * @return another HTTPHandler decorated with some middleware actions
 */
HTTPHandler ScopedLogHandler(HTTPHandler handler);
