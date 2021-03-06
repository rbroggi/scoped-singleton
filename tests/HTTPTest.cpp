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
    HTTPHandlerFunc commonLogging = [](HTTPHandler handler) -> HTTPHandler {
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

    void printReqResp(const HTTPResponse& resp, const HTTPRequest& req) {
        std::cout << "REQ ---------------------------------------" << std::endl << std::endl;
        std::cout << "HTTP "s << HTTPMethodStr(req.method) << " "s << req.uri
                  << std::endl << std::endl
                  << req.body << std::endl << std::endl;
        std::cout << "ENDREQ -------------------------------------" << std::endl;
        std::cout << "RESP----------------------------------------" << std::endl << std::endl;
        std::cout << resp.status << std::endl << resp.body << std::endl << std::endl;
        std::cout << "ENDRESP-------------------------------------" << std::endl;
    }

    HTTPHandler businessHandler = [](HTTPResponse& resp, const HTTPRequest& req) {
        resp = getDefaultResponse();
        printReqResp(resp, req);
    };
}

class HTTPTest : public ::testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

};

// make utest TSTARGS="--gtest_filter=HTTPTest.DecorateBusinessHandler"
TEST_F(HTTPTest, DecorateBusinessHandler){
    // creating a handler that is enriched by several decorators
    auto handler = anchorLog(commonLogging(businessHandler));
    HTTPResponse resp;
    handler(resp, getDefaultRequest());
    ASSERT_EQ(resp.status, HTTPStatus::HTTPStatusOk);
}

// make utest TSTARGS="--gtest_filter=HTTPTest.DifferentInstancesWouldBeCreatedInCaseOfNonAnchoredDisjointScopes"
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

// make utest TSTARGS="--gtest_filter=HTTPTest.ExceptionsDontBlockEventSending"
TEST_F(HTTPTest, ExceptionsDontBlockEventSending) {
    HTTPHandler throwingHandler = [](HTTPResponse& resp, const HTTPRequest& req) {
        throw 1;
    };
    // simulate handler called
    auto handler = commonLogging(throwingHandler);
    HTTPResponse resp;
    ASSERT_ANY_THROW(handler(resp, getDefaultRequest()));
}

// make utest TSTARGS="--gtest_filter=HTTPTest.WatchYourScopes"
TEST_F(HTTPTest, WatchYourScopes) {
    // this handler simply log the "name" and the "age" json attributes to the StructuredLog
    HTTPHandler businessTransaction = [](HTTPResponse& resp, const HTTPRequest& req) {
      auto aJsonBody = jsoncons::json::parse(req.body);
      (*StructuredLog())["name"] = aJsonBody["name"].as<std::string>();
      (*StructuredLog())["age"] = aJsonBody["age"].as<int>();
    };


    // anchor log here to simulate an instance that outlives the transaction scopes
    auto ss = StructuredLog();
    // first transaction
    {
        auto req = getDefaultRequest();
        req.body = R"({"name": "John", "surname": "Taylor", "age": 30})";
        HTTPResponse resp;
        businessTransaction(resp, req);
    }

    // second transaction
    {
        auto req = getDefaultRequest();
        req.body = R"({"name": "Jack", "surname": "Taylor", "age": 20})";
        HTTPResponse resp;
        businessTransaction(resp, req);
    }

    ASSERT_TRUE(ss->contains("name"));
    ASSERT_TRUE(ss->contains("age"));
        ASSERT_EQ((*ss)["name"].as<std::string>(), "Jack");
        ASSERT_EQ((*ss)["age"].as<int>(), 20);
    }

// make utest TSTARGS="--gtest_filter=HTTPExample.DistinctTransactions"
TEST(HTTPExample, DistinctTransactions) {
    // this handler simply log the "name" and the "age" json attributes to the StructuredLog
    HTTPHandler businessHandler = [](HTTPResponse& resp, const HTTPRequest& req) {
      auto aJsonBody = jsoncons::json::parse(req.body);
      (*StructuredLog())["name"] = aJsonBody["name"].as<std::string>();
      (*StructuredLog())["age"] = aJsonBody["age"].as<int>();
      // set resp
      resp.status = HTTPStatusOk;
      resp.headers = {{"Content-Type", "application/json"}};
      resp.body =  R"({"uid": "123e4567-e89b-12d3-a456-426614174000")";
    };


    // first transaction
    std::cout << "Start First transaction" << std::endl;
    {
        auto ss = StructuredLog();
        auto req = HTTPRequest{
                .method = HTTPMethod::POST,
                .parameters = {{"domain"s, "internal"s}},
                .headers = {{"Accept"s, "application/json"s}},
                .uri = "https://scoped-singleton-app-demo/v1/users",
                .body = R"({"name": "John", "surname": "Taylor", "age": 30})"
        };
        HTTPResponse resp;
        businessHandler(resp, req);
    }
    std::cout << "End First transaction" << std::endl << std::endl;

    // second transaction
    std::cout << "Start Second transaction" << std::endl;
    {
        auto ss = StructuredLog();
        auto req = HTTPRequest{
                .method = HTTPMethod::POST,
                .parameters = {{"domain"s, "internal"s}},
                .headers = {{"Accept"s, "application/json"s}},
                .uri = "https://scoped-singleton-app-demo/v1/users",
                .body = R"({"name": "Jack", "surname": "Taylor", "age": 20})"
        };
        HTTPResponse resp;
        businessHandler(resp, req);
    }
    std::cout << "End Second transaction" << std::endl << std::endl;
}

// make utest TSTARGS="--gtest_filter=HTTPExample.DistinctTransactionsWithCommonLogging"
TEST(HTTPExample, DistinctTransactionsWithCommonLogging) {
    // this handler simply log the "name" and the "age" json attributes to the StructuredLog
    HTTPHandler businessTransaction = [](HTTPResponse& resp, const HTTPRequest& req) {
      auto aJsonBody = jsoncons::json::parse(req.body);
      (*StructuredLog())["name"] = aJsonBody["name"].as<std::string>();
      (*StructuredLog())["age"] = aJsonBody["age"].as<int>();
      // set resp
      resp.status = HTTPStatusOk;
      resp.headers = {{"Content-Type", "application/json"}};
      resp.body =  R"({"uid": "123e4567-e89b-12d3-a456-426614174000")";
    };

    auto handler = commonLogging(businessTransaction);


    // first transaction
    std::cout << "Start First transaction" << std::endl;
    {
        auto req = HTTPRequest{
                .method = HTTPMethod::POST,
                .parameters = {{"domain"s, "internal"s}},
                .headers = {{"Accept"s, "application/json"s}},
                .uri = "https://scoped-singleton-app-demo/v1/users",
                .body = R"({"name": "John", "surname": "Taylor", "age": 30})"
        };
        HTTPResponse resp;
        handler(resp, req);
    }
    std::cout << "End First transaction" << std::endl << std::endl;

    // second transaction
    std::cout << "Start Second transaction" << std::endl;
    {
        auto req = HTTPRequest{
                .method = HTTPMethod::POST,
                .parameters = {{"domain"s, "internal"s}},
                .headers = {{"Accept"s, "application/json"s}},
                .uri = "https://scoped-singleton-app-demo/v1/users",
                .body = R"({"name": "Jack", "surname": "Taylor", "age": 20})"
        };
        HTTPResponse resp;
        handler(resp, req);
    }
    std::cout << "End Second transaction" << std::endl << std::endl;
}