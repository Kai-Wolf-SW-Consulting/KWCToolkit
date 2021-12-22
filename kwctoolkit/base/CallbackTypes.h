// Copyright (c) 2021, Kai Wolf - SW Consulting . All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS  file in the same directory.

#ifndef KWCTOOLKIT_BASE_CALLBACK_TYPES_H_
#define KWCTOOLKIT_BASE_CALLBACK_TYPES_H_

namespace kwc {

class Callback {
  public:
    virtual ~Callback() {}
    virtual bool isRepeatable() const { return false; }
    virtual void run() = 0;

  protected:
    Callback() {}
};

template <typename RetType>
class ResultCallback {
  public:
    virtual ~ResultCallback() {}
    virtual bool isRepeatable() const { return false; }
    virtual RetType run() = 0;

  protected:
    ResultCallback() {}
};

template <typename P1>
class Callback1 {
  public:
    virtual ~Callback1() {}
    virtual bool isRepeatable() const { return false; }
    virtual void run(P1) = 0;

  protected:
    Callback1() {}
};

template <typename RetType, typename P1>
class ResultCallback1 {
  public:
    virtual ~ResultCallback1() {}
    virtual bool isRepeatable() const { return false; }
    virtual RetType run(P1) = 0;

  protected:
    ResultCallback1() {}
};

template <typename P1, typename P2>
class Callback2 {
  public:
    virtual ~Callback2() {}
    virtual bool isRepeatable() const { return false; }
    virtual void run(P1, P2) = 0;

  protected:
    Callback2() {}
};

template <typename RetType, typename P1, typename P2>
class ResultCallback2 {
  public:
    virtual ~ResultCallback2() {}
    virtual bool isRepeatable() const { return false; }
    virtual RetType run(P1, P2) = 0;

  protected:
    ResultCallback2() {}
};

template <typename P1, typename P2, typename P3>
class Callback3 {
  public:
    virtual ~Callback3() {}
    virtual bool isRepeatable() const { return false; }
    virtual void run(P1, P2, P3) = 0;

  protected:
    Callback3() {}
};

template <typename RetType, typename P1, typename P2, typename P3>
class ResultCallback3 {
  public:
    virtual ~ResultCallback3() {}
    virtual bool isRepeatable() const { return false; }
    virtual RetType run(P1, P2, P3) = 0;

  protected:
    ResultCallback3() {}
};

}  // namespace kwc

#endif