//
// Created by rbroggi on 6/12/21.
//

#include "gtest/gtest.h"
#include <HTTP.h>
#include <ScopedSingleton.hpp>

using std::string_literals::operator""s;

namespace {

    HTTPRequest getDefaultRequest() {
        return HTTPRequest{
                .method = HTTPMethod::POST,
                .parameters = {{"domain"s, "internal"s}},
                .headers = {{"Accept"s, "application/json"s}},
                .uri = "https://scoped-singleton-app-demo/v1/users",
                .body = R"({"name": "John", "surname": "Taylor", "age": 30})"
        };
    };

    HTTPResponse getDefaultResponse() {
        return HTTPResponse{
                .headers = {{"Content-Type", "application/json"}},
                .status = HTTPStatusOk,
                .body = R"({"uid": "123e4567-e89b-12d3-a456-426614174000")"
        };
    }

    // anchorLog is a middleware HTTPHandlerFunc that scopes the StructuredLog around the
    // input handler. This will make sure that all the code in the handler will be logged
    // after the end of the transaction
    HTTPHandlerFunc anchorLog = [](HTTPHandler handler) -> HTTPHandler {
        return [handler](HTTPResponse& resp, const HTTPRequest& req) {
            auto ss = StructuredLog();
            handler(resp, req);
        };
    };


    // commonLogging is a middleware HTTPHandlerFunc that logs the common HTTP parameters of the transaction
    HTTPHandlerFunc commonLogging = [](HTTPHandler handler) -> HTTPHandler{
      return [handler](HTTPResponse& resp, const HTTPRequest& req) {
        // logging request attributes
        auto ss = StructuredLog();
        (*ss)["HTTPMethod"] = HTTPMethodStr(req.method);
        jsoncons::json params;
        for (auto& [k,v] : req.parameters) {
            params[k] = v;
        }
        (*ss)["HTTPParameters"] = params;
        jsoncons::json reqHeaders;
        for (auto& [k,v] : req.headers) {
            reqHeaders[k] = v;
        }
        (*ss)["HTTPRequestHeaders"] = reqHeaders;
        (*ss)["URI"] = req.uri;

        // calling handler function
        handler(resp, req);

        // logging response attributes
        jsoncons::json resHeaders;
        for (auto& [k,v] : resp.headers) {
            resHeaders[k] = v;
        }
        (*ss)["HTTPResponseHeaders"] = resHeaders;
        (*ss)["HTTPStatus"] = static_cast<int>(resp.status);
      };
    };

    HTTPHandler businessHandler = [](HTTPResponse& resp, const HTTPRequest& req) {
        std::cout << "REQ ---------------------------------------" << std::endl << std::endl;
        std::cout << "HTTP "s << HTTPMethodStr(req.method) << " "s << req.uri
                  << std::endl << std::endl
                  << req.body << std::endl << std::endl;
        std::cout << "ENDREQ -------------------------------------" << std::endl;
        resp = getDefaultResponse();
        std::cout << "RESP----------------------------------------" << std::endl << std::endl;
        std::cout << resp.status << std::endl << resp.body << std::endl << std::endl;
        std::cout << "RESPEND-------------------------------------" << std::endl;
    };
}

class HTTPTest : public ::testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

};

TEST_F(HTTPTest, DecorateBusinessHandler){
    // creating a handler that is enriched by several decorators
    auto handler = anchorLog(commonLogging(businessHandler));
    HTTPResponse resp;
    handler(resp, getDefaultRequest());
    ASSERT_EQ(resp.status, HTTPStatus::HTTPStatusOk);
}

TEST_F(HTTPTest, DifferentInstancesWouldBeCreatedInCaseOfNonAnchoredDisjointScopes){
    // anchor for the structured log enable testing sent events
    auto ss = StructuredLog();
    // creating a handler that is enriched by several decorators
    auto handler = commonLogging(businessHandler);
    HTTPResponse resp;
    handler(resp, getDefaultRequest());
    ASSERT_EQ(resp.status, HTTPStatus::HTTPStatusOk);

    // check that the log has all the data for monitoring
    ASSERT_TRUE(ss->contains("HTTPMethod"));
    ASSERT_TRUE(ss->contains("HTTPParameters"));
    ASSERT_TRUE(ss->contains("HTTPRequestHeaders"));
    ASSERT_TRUE(ss->contains("URI"));
    ASSERT_TRUE(ss->contains("HTTPResponseHeaders"));
    ASSERT_TRUE(ss->contains("HTTPStatus"));
}
