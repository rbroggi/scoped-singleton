//
// Created by rbroggi on 6/12/21.
//
#include "gtest/gtest.h"
#include <ScopedSingleton.hpp>

class ScopedSingletonTest : public ::testing::Test {

protected:
    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(ScopedSingletonTest, ScopeDefineObject){
    auto ss = StructuredLog();
    (*ss)["first"]="second";
}
