// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "transport/http_transaction.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "base/status.h"
#include "serialization/data_reader.h"
#include "serialization/data_writer.h"
#include "transport/http_request.h"
#include "transport/http_response.h"
#include "transport/http_transaction.h"
#include "transport/simple_http_transaction.h"

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