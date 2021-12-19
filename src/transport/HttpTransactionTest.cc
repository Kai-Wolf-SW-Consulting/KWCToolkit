// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "transport/HttpTransaction.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "base/Status.h"
#include "serialization/DataReader.h"
#include "serialization/DataWriter.h"
#include "transport/HttpRequest.h"
#include "transport/HttpResponse.h"
#include "transport/HttpTransaction.h"
#include "transport/SimpleHttpTransaction.h"

using kwc::serialization::CreateStringDataWriter;
using kwc::transport::HttpRequest;
using kwc::transport::HttpTransactionOptions;
using kwc::transport::SimpleHttpTransactionFactory;

TEST(HttpTransactionTest, InstantiateSimpleFactory) {
    std::unique_ptr<SimpleHttpTransactionFactory> factory(new SimpleHttpTransactionFactory());
    ASSERT_TRUE(factory != nullptr);
}

TEST(HttpTransactionTest, InstantiateSimpleTransaction) {
    std::unique_ptr<SimpleHttpTransactionFactory> factory(new SimpleHttpTransactionFactory());
    HttpTransactionOptions options;
    auto transaction = factory->createTransaction(options);
    ASSERT_TRUE(transaction != nullptr);
}

TEST(HttpTransactionTest, InstantiateSimpleRequest) {
    std::unique_ptr<SimpleHttpTransactionFactory> factory(new SimpleHttpTransactionFactory());

    HttpTransactionOptions options;
    auto transaction = factory->createTransaction(options);
    auto request = transaction->createHttpRequest(HttpRequest::GET);
    ASSERT_TRUE(request != nullptr);
}