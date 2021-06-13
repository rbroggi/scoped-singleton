//
// Created by rbroggi on 6/12/21.
//

#pragma once

#include <functional>
#include <map>
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

std::string HTTPMethodStr(HTTPMethod method);


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

/**
 * HTTPHandlerFunc implements a decorator pattern around the HTTPHandler function
 *                 it takes an HTTPHandler in input and decorates it's call by performing
 *                 some pre/post actions on it
 */
using HTTPHandlerFunc = std::function<HTTPHandler(HTTPHandler)>;
