// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/transport/http_transaction.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "kwctoolkit/base/status.h"
#include "kwctoolkit/serialization/data_reader.h"
#include "kwctoolkit/serialization/data_writer.h"
#include "kwctoolkit/transport/http_request.h"
#include "kwctoolkit/transport/http_response.h"
#include "kwctoolkit/transport/simple_http_transaction.h"

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
    auto request = transaction->createHttpRequest(HttpRequest::kGet);
    ASSERT_TRUE(request != nullptr);
}