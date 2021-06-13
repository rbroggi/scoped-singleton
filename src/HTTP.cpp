//
// Created by rbroggi on 6/12/21.
//

#include "HTTP.h"
#include "ScopedSingleton.hpp"

using std::string_literals::operator""s;

std::string HTTPMethodStr(HTTPMethod method) {
    switch(method) {
        case HTTPMethod::GET:
            return "GET"s;
        case HTTPMethod::POST:
            return "POST"s;
        case HTTPMethod::PUT:
            return "PUT"s;
        case HTTPMethod::DELETE:
            return "DELETE"s;
    };
}
