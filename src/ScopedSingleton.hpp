#pragma once

#include <jsoncons/json.hpp>
#include <memory>

using jsoncons::json;
/**
 * StructuredLog returns a valid (never nullptr) pointer to a scoped-singleton json
 * @return
 */
std::shared_ptr<json> StructuredLog();

