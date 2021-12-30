// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/transport/HttpTransaction.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "kwctoolkit/base/Status.h"
#include "kwctoolkit/serialization/DataReader.h"
#include "kwctoolkit/serialization/DataWriter.h"
#include "kwctoolkit/transport/HttpRequest.h"
#include "kwctoolkit/transport/HttpResponse.h"
#include "kwctoolkit/transport/SimpleHttpTransaction.h"

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