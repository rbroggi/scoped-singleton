//
// Created by rbroggi on 6/12/21.
//
#include "gtest/gtest.h"
#include <ScopedSingleton.hpp>

namespace {
    void methodCallEnrichesWithKeyAndValue(std::string_view k, std::string_view v) {
        (*StructuredLog())[k] = v;
    }
    void methodCallEnrichesWithKeyAndValueAndLog(std::string_view k, std::string_view v) {
        (*StructuredLog())[k] = v;
        std::cout << "MethodScope\n";
    }
}

using std::string_literals::operator""s;

// Non-anchored disjoint scopes
TEST(ScopedSingletonTest, DifferentInstancesWouldBeCreatedInCaseOfNonAnchoredDisjointScopes){
    // scope 1
    {
        auto ss = StructuredLog();
        ASSERT_NE(nullptr, ss);
        ASSERT_TRUE(ss->empty());
        (*ss)["key1"] = "value1";
        ASSERT_TRUE(ss->contains("key1"));
        ASSERT_EQ((*ss)["key1"].as<std::string>(), "value1");
    }

    // scope 2 - disjoint from scope 1
    {
        auto ss = StructuredLog();
        ASSERT_NE(nullptr, ss);
        ASSERT_TRUE(ss->empty());
        (*ss)["key2"] = "value2";
        ASSERT_TRUE(ss->contains("key2"));
        ASSERT_EQ((*ss)["key2"].as<std::string>(), "value2");
    }
}

// Anchored disjoint scopes
TEST(ScopedSingletonTest, AnchoredDisjointScopes){
    // scope 0
    auto ss = StructuredLog();
    ASSERT_NE(nullptr, ss);
    ASSERT_TRUE(ss->empty());
    // scope 1
    {
        (*ss)["key1"] = "value1";
    }

    ASSERT_TRUE(ss->contains("key1"));
    ASSERT_EQ((*ss)["key1"].as<std::string>(), "value1");

    // scope 2 - even if a new reference is issued, the longest living instance guarantees singleton
    {
        auto ss2 = StructuredLog();
        ASSERT_NE(nullptr, ss2);
        // It contains the log in the other disjointed scope
        ASSERT_TRUE(ss->contains("key1"));
        ASSERT_EQ((*ss)["key1"].as<std::string>(), "value1");
        (*ss2)["key2"] = "value2";
    }
    // even though modified using different variable, the anchor also had it's value updated
    ASSERT_TRUE(ss->contains("key2"));
    ASSERT_EQ((*ss)["key2"].as<std::string>(), "value2");
}

// method call share instance with anchor
TEST(ScopedSingletonTest, MethodCallShareInstance){
    auto ss = StructuredLog();
    ASSERT_TRUE(ss->empty());
    methodCallEnrichesWithKeyAndValue("key", "value");
    ASSERT_FALSE(ss->empty());
    ASSERT_TRUE(ss->contains("key"));
    ASSERT_EQ((*ss)["key"].as<std::string>(), "value");
}

TEST(ScopedSingletonExample, DifferentInstancesWouldBeCreatedInCaseOfNonAnchoredDisjointScopes){
    // scope 1
    {
        auto ss = StructuredLog();
        (*ss)["key1"] = "value1";
        std::cout << "Scope1\n";
    }

    // scope 2 - disjoint from scope 1
    {
        auto ss = StructuredLog();
        (*ss)["key2"] = "value2";
        std::cout << "Scope2\n";
    }
}

TEST(ScopedSingletonExample, AnchoredDisjointScopes){
    // scope 0
    auto ss = StructuredLog();
    std::cout << "Scope0\n";
    // scope 1
    {
        (*ss)["key1"] = "value1";
        std::cout << "Scope1\n";
    }

    // scope 2 - even if a new reference is issued, the longest living instance guarantees singleton
    {
        auto ss2 = StructuredLog();
        (*ss2)["key2"] = "value2";
        std::cout << "Scope2\n";
    }
}

TEST(ScopedSingletonExample, MethodCallShareInstance){
    auto ss = StructuredLog();
    std::cout << "Scope0\n";
    methodCallEnrichesWithKeyAndValueAndLog("key", "value");
}