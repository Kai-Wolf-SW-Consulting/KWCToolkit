// Copyright (c) 2021, Kai Wolf - SW Consulting . All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS  file in the same directory.

#ifndef KWCTOOLKIT_BASE_CALLBACK_IMPL_H_
#define KWCTOOLKIT_BASE_CALLBACK_IMPL_H_

#include <type_traits>
#include <utility>

#include "kwctoolkit/base/callback_types.h"

namespace kwc {
namespace internal {

template <typename T>
struct ConstRef {
    using base_type = typename ::std::remove_reference<T>::type;
    using type = const base_type&;
};
}  // namespace internal
template <bool del,
          typename RetType,
          typename Class,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback00 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Method = RetType (Class::*)() const;
    inline ConstMemberResultCallback00(const Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (instance_->*method_)();
        } else {
            RetType result = (instance_->*method_)();
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
};
template <bool del, typename Class>
class ConstMemberResultCallback00<del,
                                  void,
                                  Class,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback {
  public:
    using base = Callback;
    using Method = void (Class::*)() const;
    inline ConstMemberResultCallback00(const Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (instance_->*method_)();
        } else {
            (instance_->*method_)();
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
};

template <typename Caller, typename Callee, typename RetType>
inline typename ConstMemberResultCallback00<true, RetType, Caller>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)() const) {
    return new ConstMemberResultCallback00<true, RetType, Caller>(instance, method);
}

template <typename Caller, typename Callee, typename RetType>
inline typename ConstMemberResultCallback00<false, RetType, Caller>::base* MakePermanentCallback(
    const Caller* instance,
    RetType (Callee::*method)() const) {
    return new ConstMemberResultCallback00<false, RetType, Caller>(instance, method);
}
template <bool del,
          typename RetType,
          typename Class,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback00 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Method = RetType (Class::*)();
    inline MemberResultCallback00(Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (instance_->*method_)();
        } else {
            RetType result = (instance_->*method_)();
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
};
template <bool del, typename Class>
class MemberResultCallback00<del,
                             void,
                             Class,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback {
  public:
    using base = Callback;
    using Method = void (Class::*)();
    inline MemberResultCallback00(Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (instance_->*method_)();
        } else {
            (instance_->*method_)();
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
};

template <typename Caller, typename Callee, typename RetType>
inline typename MemberResultCallback00<true, RetType, Caller>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)()) {
    return new MemberResultCallback00<true, RetType, Caller>(instance, method);
}

template <typename Caller, typename Callee, typename RetType>
inline typename MemberResultCallback00<false, RetType, Caller>::base* MakePermanentCallback(
    Caller* instance,
    RetType (Callee::*method)()) {
    return new MemberResultCallback00<false, RetType, Caller>(instance, method);
}
template <bool del, typename RetType>
class FunctionResultCallback00 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Function = RetType (*)();
    inline FunctionResultCallback00(Function function) : function_(function) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (*function_)();
        } else {
            RetType result = (*function_)();
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
};
template <bool del>
class FunctionResultCallback00<del, void> : public Callback {
  public:
    using base = Callback;
    using Function = void (*)();
    inline FunctionResultCallback00(Function function) : function_(function) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (*function_)();
        } else {
            (*function_)();
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
};

template <typename RetType>
inline typename FunctionResultCallback00<true, RetType>::base* MakeCallback(RetType (*function)()) {
    return new FunctionResultCallback00<true, RetType>(function);
}

template <typename RetType>
inline typename FunctionResultCallback00<false, RetType>::base* MakePermanentCallback(
    RetType (*function)()) {
    return new FunctionResultCallback00<false, RetType>(function);
}
template <bool del,
          typename RetType,
          typename Class,
          typename A1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback01 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Method = RetType (Class::*)(A1) const;
    inline ConstMemberResultCallback01(const Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (instance_->*method_)(a1);
        } else {
            RetType result = (instance_->*method_)(a1);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
};
template <bool del, typename Class, typename A1>
class ConstMemberResultCallback01<del,
                                  void,
                                  Class,
                                  A1,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Method = void (Class::*)(A1) const;
    inline ConstMemberResultCallback01(const Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (instance_->*method_)(a1);
        } else {
            (instance_->*method_)(a1);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
};

template <typename Caller, typename Callee, typename RetType, typename A1>
inline typename ConstMemberResultCallback01<true, RetType, Caller, A1>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(A1) const) {
    return new ConstMemberResultCallback01<true, RetType, Caller, A1>(instance, method);
}

template <typename Caller, typename Callee, typename RetType, typename A1>
inline typename ConstMemberResultCallback01<false, RetType, Caller, A1>::base*
MakePermanentCallback(const Caller* instance, RetType (Callee::*method)(A1) const) {
    return new ConstMemberResultCallback01<false, RetType, Caller, A1>(instance, method);
}
template <bool del,
          typename RetType,
          typename Class,
          typename A1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback01 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Method = RetType (Class::*)(A1);
    inline MemberResultCallback01(Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (instance_->*method_)(a1);
        } else {
            RetType result = (instance_->*method_)(a1);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
};
template <bool del, typename Class, typename A1>
class MemberResultCallback01<del,
                             void,
                             Class,
                             A1,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Method = void (Class::*)(A1);
    inline MemberResultCallback01(Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (instance_->*method_)(a1);
        } else {
            (instance_->*method_)(a1);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
};

template <typename Caller, typename Callee, typename RetType, typename A1>
inline typename MemberResultCallback01<true, RetType, Caller, A1>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(A1)) {
    return new MemberResultCallback01<true, RetType, Caller, A1>(instance, method);
}

template <typename Caller, typename Callee, typename RetType, typename A1>
inline typename MemberResultCallback01<false, RetType, Caller, A1>::base* MakePermanentCallback(
    Caller* instance,
    RetType (Callee::*method)(A1)) {
    return new MemberResultCallback01<false, RetType, Caller, A1>(instance, method);
}
template <bool del, typename RetType, typename A1>
class FunctionResultCallback01 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Function = RetType (*)(A1);
    inline FunctionResultCallback01(Function function) : function_(function) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (*function_)(a1);
        } else {
            RetType result = (*function_)(a1);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
};
template <bool del, typename A1>
class FunctionResultCallback01<del, void, A1> : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Function = void (*)(A1);
    inline FunctionResultCallback01(Function function) : function_(function) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (*function_)(a1);
        } else {
            (*function_)(a1);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
};

template <typename RetType, typename A1>
inline typename FunctionResultCallback01<true, RetType, A1>::base* MakeCallback(
    RetType (*function)(A1)) {
    return new FunctionResultCallback01<true, RetType, A1>(function);
}

template <typename RetType, typename A1>
inline typename FunctionResultCallback01<false, RetType, A1>::base* MakePermanentCallback(
    RetType (*function)(A1)) {
    return new FunctionResultCallback01<false, RetType, A1>(function);
}
template <bool del,
          typename RetType,
          typename Class,
          typename A1,
          typename A2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback02 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Method = RetType (Class::*)(A1, A2) const;
    inline ConstMemberResultCallback02(const Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (instance_->*method_)(a1, a2);
        } else {
            RetType result = (instance_->*method_)(a1, a2);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
};
template <bool del, typename Class, typename A1, typename A2>
class ConstMemberResultCallback02<del,
                                  void,
                                  Class,
                                  A1,
                                  A2,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Method = void (Class::*)(A1, A2) const;
    inline ConstMemberResultCallback02(const Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (instance_->*method_)(a1, a2);
        } else {
            (instance_->*method_)(a1, a2);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
};

template <typename Caller, typename Callee, typename RetType, typename A1, typename A2>
inline typename ConstMemberResultCallback02<true, RetType, Caller, A1, A2>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(A1, A2) const) {
    return new ConstMemberResultCallback02<true, RetType, Caller, A1, A2>(instance, method);
}

template <typename Caller, typename Callee, typename RetType, typename A1, typename A2>
inline typename ConstMemberResultCallback02<false, RetType, Caller, A1, A2>::base*
MakePermanentCallback(const Caller* instance, RetType (Callee::*method)(A1, A2) const) {
    return new ConstMemberResultCallback02<false, RetType, Caller, A1, A2>(instance, method);
}
template <bool del,
          typename RetType,
          typename Class,
          typename A1,
          typename A2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback02 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Method = RetType (Class::*)(A1, A2);
    inline MemberResultCallback02(Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (instance_->*method_)(a1, a2);
        } else {
            RetType result = (instance_->*method_)(a1, a2);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
};
template <bool del, typename Class, typename A1, typename A2>
class MemberResultCallback02<del,
                             void,
                             Class,
                             A1,
                             A2,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Method = void (Class::*)(A1, A2);
    inline MemberResultCallback02(Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (instance_->*method_)(a1, a2);
        } else {
            (instance_->*method_)(a1, a2);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
};

template <typename Caller, typename Callee, typename RetType, typename A1, typename A2>
inline typename MemberResultCallback02<true, RetType, Caller, A1, A2>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(A1, A2)) {
    return new MemberResultCallback02<true, RetType, Caller, A1, A2>(instance, method);
}

template <typename Caller, typename Callee, typename RetType, typename A1, typename A2>
inline typename MemberResultCallback02<false, RetType, Caller, A1, A2>::base* MakePermanentCallback(
    Caller* instance,
    RetType (Callee::*method)(A1, A2)) {
    return new MemberResultCallback02<false, RetType, Caller, A1, A2>(instance, method);
}
template <bool del, typename RetType, typename A1, typename A2>
class FunctionResultCallback02 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Function = RetType (*)(A1, A2);
    inline FunctionResultCallback02(Function function) : function_(function) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (*function_)(a1, a2);
        } else {
            RetType result = (*function_)(a1, a2);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
};
template <bool del, typename A1, typename A2>
class FunctionResultCallback02<del, void, A1, A2> : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Function = void (*)(A1, A2);
    inline FunctionResultCallback02(Function function) : function_(function) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (*function_)(a1, a2);
        } else {
            (*function_)(a1, a2);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
};

template <typename RetType, typename A1, typename A2>
inline typename FunctionResultCallback02<true, RetType, A1, A2>::base* MakeCallback(
    RetType (*function)(A1, A2)) {
    return new FunctionResultCallback02<true, RetType, A1, A2>(function);
}

template <typename RetType, typename A1, typename A2>
inline typename FunctionResultCallback02<false, RetType, A1, A2>::base* MakePermanentCallback(
    RetType (*function)(A1, A2)) {
    return new FunctionResultCallback02<false, RetType, A1, A2>(function);
}
template <bool del,
          typename RetType,
          typename Class,
          typename A1,
          typename A2,
          typename A3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback03 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Method = RetType (Class::*)(A1, A2, A3) const;
    inline ConstMemberResultCallback03(const Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (instance_->*method_)(a1, a2, a3);
        } else {
            RetType result = (instance_->*method_)(a1, a2, a3);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
};
template <bool del, typename Class, typename A1, typename A2, typename A3>
class ConstMemberResultCallback03<del,
                                  void,
                                  Class,
                                  A1,
                                  A2,
                                  A3,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Method = void (Class::*)(A1, A2, A3) const;
    inline ConstMemberResultCallback03(const Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (instance_->*method_)(a1, a2, a3);
        } else {
            (instance_->*method_)(a1, a2, a3);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
};

template <typename Caller, typename Callee, typename RetType, typename A1, typename A2, typename A3>
inline typename ConstMemberResultCallback03<true, RetType, Caller, A1, A2, A3>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(A1, A2, A3) const) {
    return new ConstMemberResultCallback03<true, RetType, Caller, A1, A2, A3>(instance, method);
}

template <typename Caller, typename Callee, typename RetType, typename A1, typename A2, typename A3>
inline typename ConstMemberResultCallback03<false, RetType, Caller, A1, A2, A3>::base*
MakePermanentCallback(const Caller* instance, RetType (Callee::*method)(A1, A2, A3) const) {
    return new ConstMemberResultCallback03<false, RetType, Caller, A1, A2, A3>(instance, method);
}
template <bool del,
          typename RetType,
          typename Class,
          typename A1,
          typename A2,
          typename A3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback03 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Method = RetType (Class::*)(A1, A2, A3);
    inline MemberResultCallback03(Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (instance_->*method_)(a1, a2, a3);
        } else {
            RetType result = (instance_->*method_)(a1, a2, a3);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
};
template <bool del, typename Class, typename A1, typename A2, typename A3>
class MemberResultCallback03<del,
                             void,
                             Class,
                             A1,
                             A2,
                             A3,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Method = void (Class::*)(A1, A2, A3);
    inline MemberResultCallback03(Class* instance, Method method)
        : instance_(instance), method_(method) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (instance_->*method_)(a1, a2, a3);
        } else {
            (instance_->*method_)(a1, a2, a3);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
};

template <typename Caller, typename Callee, typename RetType, typename A1, typename A2, typename A3>
inline typename MemberResultCallback03<true, RetType, Caller, A1, A2, A3>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(A1, A2, A3)) {
    return new MemberResultCallback03<true, RetType, Caller, A1, A2, A3>(instance, method);
}

template <typename Caller, typename Callee, typename RetType, typename A1, typename A2, typename A3>
inline typename MemberResultCallback03<false, RetType, Caller, A1, A2, A3>::base*
MakePermanentCallback(Caller* instance, RetType (Callee::*method)(A1, A2, A3)) {
    return new MemberResultCallback03<false, RetType, Caller, A1, A2, A3>(instance, method);
}
template <bool del, typename RetType, typename A1, typename A2, typename A3>
class FunctionResultCallback03 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Function = RetType (*)(A1, A2, A3);
    inline FunctionResultCallback03(Function function) : function_(function) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (*function_)(a1, a2, a3);
        } else {
            RetType result = (*function_)(a1, a2, a3);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
};
template <bool del, typename A1, typename A2, typename A3>
class FunctionResultCallback03<del, void, A1, A2, A3> : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Function = void (*)(A1, A2, A3);
    inline FunctionResultCallback03(Function function) : function_(function) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (*function_)(a1, a2, a3);
        } else {
            (*function_)(a1, a2, a3);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
};

template <typename RetType, typename A1, typename A2, typename A3>
inline typename FunctionResultCallback03<true, RetType, A1, A2, A3>::base* MakeCallback(
    RetType (*function)(A1, A2, A3)) {
    return new FunctionResultCallback03<true, RetType, A1, A2, A3>(function);
}

template <typename RetType, typename A1, typename A2, typename A3>
inline typename FunctionResultCallback03<false, RetType, A1, A2, A3>::base* MakePermanentCallback(
    RetType (*function)(A1, A2, A3)) {
    return new FunctionResultCallback03<false, RetType, A1, A2, A3>(function);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback10 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Method = RetType (Class::*)(P1) const;
    inline ConstMemberResultCallback10(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (instance_->*method_)(p1_);
        } else {
            RetType result = (instance_->*method_)(p1_);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename Class, typename P1>
class ConstMemberResultCallback10<del,
                                  void,
                                  Class,
                                  P1,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback {
  public:
    using base = Callback;
    using Method = void (Class::*)(P1) const;
    inline ConstMemberResultCallback10(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (instance_->*method_)(p1_);
        } else {
            (instance_->*method_)(p1_);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename Caller, typename Callee, typename RetType, typename P1>
inline typename ConstMemberResultCallback10<true, RetType, Caller, P1>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(P1) const,
    typename internal::ConstRef<P1>::type p1) {
    return new ConstMemberResultCallback10<true, RetType, Caller, P1>(instance, method, p1);
}

template <typename Caller, typename Callee, typename RetType, typename P1>
inline typename ConstMemberResultCallback10<false, RetType, Caller, P1>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1) const,
                      typename internal::ConstRef<P1>::type p1) {
    return new ConstMemberResultCallback10<false, RetType, Caller, P1>(instance, method, p1);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback10 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Method = RetType (Class::*)(P1);
    inline MemberResultCallback10(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (instance_->*method_)(p1_);
        } else {
            RetType result = (instance_->*method_)(p1_);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename Class, typename P1>
class MemberResultCallback10<del,
                             void,
                             Class,
                             P1,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback {
  public:
    using base = Callback;
    using Method = void (Class::*)(P1);
    inline MemberResultCallback10(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (instance_->*method_)(p1_);
        } else {
            (instance_->*method_)(p1_);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename Caller, typename Callee, typename RetType, typename P1>
inline typename MemberResultCallback10<true, RetType, Caller, P1>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1),
    typename internal::ConstRef<P1>::type p1) {
    return new MemberResultCallback10<true, RetType, Caller, P1>(instance, method, p1);
}

template <typename Caller, typename Callee, typename RetType, typename P1>
inline typename MemberResultCallback10<false, RetType, Caller, P1>::base* MakePermanentCallback(
    Caller* instance,
    RetType (Callee::*method)(P1),
    typename internal::ConstRef<P1>::type p1) {
    return new MemberResultCallback10<false, RetType, Caller, P1>(instance, method, p1);
}
template <bool del, typename RetType, typename P1>
class FunctionResultCallback10 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Function = RetType (*)(P1);
    inline FunctionResultCallback10(Function function, typename internal::ConstRef<P1>::type p1)
        : function_(function), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (*function_)(p1_);
        } else {
            RetType result = (*function_)(p1_);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename P1>
class FunctionResultCallback10<del, void, P1> : public Callback {
  public:
    using base = Callback;
    using Function = void (*)(P1);
    inline FunctionResultCallback10(Function function, typename internal::ConstRef<P1>::type p1)
        : function_(function), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (*function_)(p1_);
        } else {
            (*function_)(p1_);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename RetType, typename P1>
inline typename FunctionResultCallback10<true, RetType, P1>::base* MakeCallback(
    RetType (*function)(P1),
    typename internal::ConstRef<P1>::type p1) {
    return new FunctionResultCallback10<true, RetType, P1>(function, p1);
}

template <typename RetType, typename P1>
inline typename FunctionResultCallback10<false, RetType, P1>::base* MakePermanentCallback(
    RetType (*function)(P1),
    typename internal::ConstRef<P1>::type p1) {
    return new FunctionResultCallback10<false, RetType, P1>(function, p1);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename A1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback11 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Method = RetType (Class::*)(P1, A1) const;
    inline ConstMemberResultCallback11(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (instance_->*method_)(p1_, a1);
        } else {
            RetType result = (instance_->*method_)(p1_, a1);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename Class, typename P1, typename A1>
class ConstMemberResultCallback11<del,
                                  void,
                                  Class,
                                  P1,
                                  A1,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Method = void (Class::*)(P1, A1) const;
    inline ConstMemberResultCallback11(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (instance_->*method_)(p1_, a1);
        } else {
            (instance_->*method_)(p1_, a1);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename A1>
inline typename ConstMemberResultCallback11<true, RetType, Caller, P1, A1>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(P1, A1) const,
    typename internal::ConstRef<P1>::type p1) {
    return new ConstMemberResultCallback11<true, RetType, Caller, P1, A1>(instance, method, p1);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename A1>
inline typename ConstMemberResultCallback11<false, RetType, Caller, P1, A1>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, A1) const,
                      typename internal::ConstRef<P1>::type p1) {
    return new ConstMemberResultCallback11<false, RetType, Caller, P1, A1>(instance, method, p1);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename A1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback11 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Method = RetType (Class::*)(P1, A1);
    inline MemberResultCallback11(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (instance_->*method_)(p1_, a1);
        } else {
            RetType result = (instance_->*method_)(p1_, a1);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename Class, typename P1, typename A1>
class MemberResultCallback11<del,
                             void,
                             Class,
                             P1,
                             A1,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Method = void (Class::*)(P1, A1);
    inline MemberResultCallback11(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (instance_->*method_)(p1_, a1);
        } else {
            (instance_->*method_)(p1_, a1);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename A1>
inline typename MemberResultCallback11<true, RetType, Caller, P1, A1>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, A1),
    typename internal::ConstRef<P1>::type p1) {
    return new MemberResultCallback11<true, RetType, Caller, P1, A1>(instance, method, p1);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename A1>
inline typename MemberResultCallback11<false, RetType, Caller, P1, A1>::base* MakePermanentCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, A1),
    typename internal::ConstRef<P1>::type p1) {
    return new MemberResultCallback11<false, RetType, Caller, P1, A1>(instance, method, p1);
}
template <bool del, typename RetType, typename P1, typename A1>
class FunctionResultCallback11 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Function = RetType (*)(P1, A1);
    inline FunctionResultCallback11(Function function, typename internal::ConstRef<P1>::type p1)
        : function_(function), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (*function_)(p1_, a1);
        } else {
            RetType result = (*function_)(p1_, a1);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename P1, typename A1>
class FunctionResultCallback11<del, void, P1, A1> : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Function = void (*)(P1, A1);
    inline FunctionResultCallback11(Function function, typename internal::ConstRef<P1>::type p1)
        : function_(function), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (*function_)(p1_, a1);
        } else {
            (*function_)(p1_, a1);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename RetType, typename P1, typename A1>
inline typename FunctionResultCallback11<true, RetType, P1, A1>::base* MakeCallback(
    RetType (*function)(P1, A1),
    typename internal::ConstRef<P1>::type p1) {
    return new FunctionResultCallback11<true, RetType, P1, A1>(function, p1);
}

template <typename RetType, typename P1, typename A1>
inline typename FunctionResultCallback11<false, RetType, P1, A1>::base* MakePermanentCallback(
    RetType (*function)(P1, A1),
    typename internal::ConstRef<P1>::type p1) {
    return new FunctionResultCallback11<false, RetType, P1, A1>(function, p1);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename A1,
          typename A2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback12 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Method = RetType (Class::*)(P1, A1, A2) const;
    inline ConstMemberResultCallback12(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (instance_->*method_)(p1_, a1, a2);
        } else {
            RetType result = (instance_->*method_)(p1_, a1, a2);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename Class, typename P1, typename A1, typename A2>
class ConstMemberResultCallback12<del,
                                  void,
                                  Class,
                                  P1,
                                  A1,
                                  A2,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Method = void (Class::*)(P1, A1, A2) const;
    inline ConstMemberResultCallback12(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (instance_->*method_)(p1_, a1, a2);
        } else {
            (instance_->*method_)(p1_, a1, a2);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename A1, typename A2>
inline typename ConstMemberResultCallback12<true, RetType, Caller, P1, A1, A2>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(P1, A1, A2) const,
    typename internal::ConstRef<P1>::type p1) {
    return new ConstMemberResultCallback12<true, RetType, Caller, P1, A1, A2>(instance, method, p1);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename A1, typename A2>
inline typename ConstMemberResultCallback12<false, RetType, Caller, P1, A1, A2>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, A1, A2) const,
                      typename internal::ConstRef<P1>::type p1) {
    return new ConstMemberResultCallback12<false, RetType, Caller, P1, A1, A2>(instance, method,
                                                                               p1);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename A1,
          typename A2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback12 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Method = RetType (Class::*)(P1, A1, A2);
    inline MemberResultCallback12(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (instance_->*method_)(p1_, a1, a2);
        } else {
            RetType result = (instance_->*method_)(p1_, a1, a2);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename Class, typename P1, typename A1, typename A2>
class MemberResultCallback12<del,
                             void,
                             Class,
                             P1,
                             A1,
                             A2,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Method = void (Class::*)(P1, A1, A2);
    inline MemberResultCallback12(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(std::move(p1)) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (instance_->*method_)(p1_, a1, a2);
        } else {
            (instance_->*method_)(p1_, a1, a2);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename A1, typename A2>
inline typename MemberResultCallback12<true, RetType, Caller, P1, A1, A2>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, A1, A2),
    typename internal::ConstRef<P1>::type p1) {
    return new MemberResultCallback12<true, RetType, Caller, P1, A1, A2>(instance, method, p1);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename A1, typename A2>
inline typename MemberResultCallback12<false, RetType, Caller, P1, A1, A2>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, A1, A2),
                      typename internal::ConstRef<P1>::type p1) {
    return new MemberResultCallback12<false, RetType, Caller, P1, A1, A2>(instance, method, p1);
}
template <bool del, typename RetType, typename P1, typename A1, typename A2>
class FunctionResultCallback12 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Function = RetType (*)(P1, A1, A2);
    inline FunctionResultCallback12(Function function, typename internal::ConstRef<P1>::type p1)
        : function_(function), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (*function_)(p1_, a1, a2);
        } else {
            RetType result = (*function_)(p1_, a1, a2);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename P1, typename A1, typename A2>
class FunctionResultCallback12<del, void, P1, A1, A2> : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Function = void (*)(P1, A1, A2);
    inline FunctionResultCallback12(Function function, typename internal::ConstRef<P1>::type p1)
        : function_(function), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (*function_)(p1_, a1, a2);
        } else {
            (*function_)(p1_, a1, a2);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename RetType, typename P1, typename A1, typename A2>
inline typename FunctionResultCallback12<true, RetType, P1, A1, A2>::base* MakeCallback(
    RetType (*function)(P1, A1, A2),
    typename internal::ConstRef<P1>::type p1) {
    return new FunctionResultCallback12<true, RetType, P1, A1, A2>(function, p1);
}

template <typename RetType, typename P1, typename A1, typename A2>
inline typename FunctionResultCallback12<false, RetType, P1, A1, A2>::base* MakePermanentCallback(
    RetType (*function)(P1, A1, A2),
    typename internal::ConstRef<P1>::type p1) {
    return new FunctionResultCallback12<false, RetType, P1, A1, A2>(function, p1);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename A1,
          typename A2,
          typename A3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback13 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Method = RetType (Class::*)(P1, A1, A2, A3) const;
    inline ConstMemberResultCallback13(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (instance_->*method_)(p1_, a1, a2, a3);
        } else {
            RetType result = (instance_->*method_)(p1_, a1, a2, a3);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename Class, typename P1, typename A1, typename A2, typename A3>
class ConstMemberResultCallback13<del,
                                  void,
                                  Class,
                                  P1,
                                  A1,
                                  A2,
                                  A3,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Method = void (Class::*)(P1, A1, A2, A3) const;
    inline ConstMemberResultCallback13(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (instance_->*method_)(p1_, a1, a2, a3);
        } else {
            (instance_->*method_)(p1_, a1, a2, a3);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename A1,
          typename A2,
          typename A3>
inline typename ConstMemberResultCallback13<true, RetType, Caller, P1, A1, A2, A3>::base*
MakeCallback(const Caller* instance,
             RetType (Callee::*method)(P1, A1, A2, A3) const,
             typename internal::ConstRef<P1>::type p1) {
    return new ConstMemberResultCallback13<true, RetType, Caller, P1, A1, A2, A3>(instance, method,
                                                                                  p1);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename A1,
          typename A2,
          typename A3>
inline typename ConstMemberResultCallback13<false, RetType, Caller, P1, A1, A2, A3>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, A1, A2, A3) const,
                      typename internal::ConstRef<P1>::type p1) {
    return new ConstMemberResultCallback13<false, RetType, Caller, P1, A1, A2, A3>(instance, method,
                                                                                   p1);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename A1,
          typename A2,
          typename A3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback13 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Method = RetType (Class::*)(P1, A1, A2, A3);
    inline MemberResultCallback13(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (instance_->*method_)(p1_, a1, a2, a3);
        } else {
            RetType result = (instance_->*method_)(p1_, a1, a2, a3);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename Class, typename P1, typename A1, typename A2, typename A3>
class MemberResultCallback13<del,
                             void,
                             Class,
                             P1,
                             A1,
                             A2,
                             A3,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Method = void (Class::*)(P1, A1, A2, A3);
    inline MemberResultCallback13(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1)
        : instance_(instance), method_(method), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (instance_->*method_)(p1_, a1, a2, a3);
        } else {
            (instance_->*method_)(p1_, a1, a2, a3);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename A1,
          typename A2,
          typename A3>
inline typename MemberResultCallback13<true, RetType, Caller, P1, A1, A2, A3>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, A1, A2, A3),
    typename internal::ConstRef<P1>::type p1) {
    return new MemberResultCallback13<true, RetType, Caller, P1, A1, A2, A3>(instance, method, p1);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename A1,
          typename A2,
          typename A3>
inline typename MemberResultCallback13<false, RetType, Caller, P1, A1, A2, A3>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, A1, A2, A3),
                      typename internal::ConstRef<P1>::type p1) {
    return new MemberResultCallback13<false, RetType, Caller, P1, A1, A2, A3>(instance, method, p1);
}
template <bool del, typename RetType, typename P1, typename A1, typename A2, typename A3>
class FunctionResultCallback13 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Function = RetType (*)(P1, A1, A2, A3);
    inline FunctionResultCallback13(Function function, typename internal::ConstRef<P1>::type p1)
        : function_(function), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (*function_)(p1_, a1, a2, a3);
        } else {
            RetType result = (*function_)(p1_, a1, a2, a3);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
};
template <bool del, typename P1, typename A1, typename A2, typename A3>
class FunctionResultCallback13<del, void, P1, A1, A2, A3> : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Function = void (*)(P1, A1, A2, A3);
    inline FunctionResultCallback13(Function function, typename internal::ConstRef<P1>::type p1)
        : function_(function), p1_(p1) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (*function_)(p1_, a1, a2, a3);
        } else {
            (*function_)(p1_, a1, a2, a3);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
};

template <typename RetType, typename P1, typename A1, typename A2, typename A3>
inline typename FunctionResultCallback13<true, RetType, P1, A1, A2, A3>::base* MakeCallback(
    RetType (*function)(P1, A1, A2, A3),
    typename internal::ConstRef<P1>::type p1) {
    return new FunctionResultCallback13<true, RetType, P1, A1, A2, A3>(function, p1);
}

template <typename RetType, typename P1, typename A1, typename A2, typename A3>
inline typename FunctionResultCallback13<false, RetType, P1, A1, A2, A3>::base*
MakePermanentCallback(RetType (*function)(P1, A1, A2, A3),
                      typename internal::ConstRef<P1>::type p1) {
    return new FunctionResultCallback13<false, RetType, P1, A1, A2, A3>(function, p1);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback20 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Method = RetType (Class::*)(P1, P2) const;
    inline ConstMemberResultCallback20(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename Class, typename P1, typename P2>
class ConstMemberResultCallback20<del,
                                  void,
                                  Class,
                                  P1,
                                  P2,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback {
  public:
    using base = Callback;
    using Method = void (Class::*)(P1, P2) const;
    inline ConstMemberResultCallback20(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (instance_->*method_)(p1_, p2_);
        } else {
            (instance_->*method_)(p1_, p2_);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2>
inline typename ConstMemberResultCallback20<true, RetType, Caller, P1, P2>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(P1, P2) const,
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new ConstMemberResultCallback20<true, RetType, Caller, P1, P2>(instance, method, p1, p2);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2>
inline typename ConstMemberResultCallback20<false, RetType, Caller, P1, P2>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, P2) const,
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new ConstMemberResultCallback20<false, RetType, Caller, P1, P2>(instance, method, p1,
                                                                           p2);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback20 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Method = RetType (Class::*)(P1, P2);
    inline MemberResultCallback20(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename Class, typename P1, typename P2>
class MemberResultCallback20<del,
                             void,
                             Class,
                             P1,
                             P2,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback {
  public:
    using base = Callback;
    using Method = void (Class::*)(P1, P2);
    inline MemberResultCallback20(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (instance_->*method_)(p1_, p2_);
        } else {
            (instance_->*method_)(p1_, p2_);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2>
inline typename MemberResultCallback20<true, RetType, Caller, P1, P2>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, P2),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new MemberResultCallback20<true, RetType, Caller, P1, P2>(instance, method, p1, p2);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2>
inline typename MemberResultCallback20<false, RetType, Caller, P1, P2>::base* MakePermanentCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, P2),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new MemberResultCallback20<false, RetType, Caller, P1, P2>(instance, method, p1, p2);
}
template <bool del, typename RetType, typename P1, typename P2>
class FunctionResultCallback20 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Function = RetType (*)(P1, P2);
    inline FunctionResultCallback20(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2)
        : function_(function), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (*function_)(p1_, p2_);
        } else {
            RetType result = (*function_)(p1_, p2_);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename P1, typename P2>
class FunctionResultCallback20<del, void, P1, P2> : public Callback {
  public:
    using base = Callback;
    using Function = void (*)(P1, P2);
    inline FunctionResultCallback20(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2)
        : function_(function), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (*function_)(p1_, p2_);
        } else {
            (*function_)(p1_, p2_);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename RetType, typename P1, typename P2>
inline typename FunctionResultCallback20<true, RetType, P1, P2>::base* MakeCallback(
    RetType (*function)(P1, P2),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new FunctionResultCallback20<true, RetType, P1, P2>(function, p1, p2);
}

template <typename RetType, typename P1, typename P2>
inline typename FunctionResultCallback20<false, RetType, P1, P2>::base* MakePermanentCallback(
    RetType (*function)(P1, P2),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new FunctionResultCallback20<false, RetType, P1, P2>(function, p1, p2);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename A1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback21 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Method = RetType (Class::*)(P1, P2, A1) const;
    inline ConstMemberResultCallback21(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, a1);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, a1);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename Class, typename P1, typename P2, typename A1>
class ConstMemberResultCallback21<del,
                                  void,
                                  Class,
                                  P1,
                                  P2,
                                  A1,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Method = void (Class::*)(P1, P2, A1) const;
    inline ConstMemberResultCallback21(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, a1);
        } else {
            (instance_->*method_)(p1_, p2_, a1);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2, typename A1>
inline typename ConstMemberResultCallback21<true, RetType, Caller, P1, P2, A1>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(P1, P2, A1) const,
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new ConstMemberResultCallback21<true, RetType, Caller, P1, P2, A1>(instance, method, p1,
                                                                              p2);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2, typename A1>
inline typename ConstMemberResultCallback21<false, RetType, Caller, P1, P2, A1>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, P2, A1) const,
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new ConstMemberResultCallback21<false, RetType, Caller, P1, P2, A1>(instance, method, p1,
                                                                               p2);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename A1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback21 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Method = RetType (Class::*)(P1, P2, A1);
    inline MemberResultCallback21(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, a1);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, a1);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename Class, typename P1, typename P2, typename A1>
class MemberResultCallback21<del,
                             void,
                             Class,
                             P1,
                             P2,
                             A1,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Method = void (Class::*)(P1, P2, A1);
    inline MemberResultCallback21(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, a1);
        } else {
            (instance_->*method_)(p1_, p2_, a1);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2, typename A1>
inline typename MemberResultCallback21<true, RetType, Caller, P1, P2, A1>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, P2, A1),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new MemberResultCallback21<true, RetType, Caller, P1, P2, A1>(instance, method, p1, p2);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2, typename A1>
inline typename MemberResultCallback21<false, RetType, Caller, P1, P2, A1>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, P2, A1),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new MemberResultCallback21<false, RetType, Caller, P1, P2, A1>(instance, method, p1, p2);
}
template <bool del, typename RetType, typename P1, typename P2, typename A1>
class FunctionResultCallback21 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Function = RetType (*)(P1, P2, A1);
    inline FunctionResultCallback21(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2)
        : function_(function), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (*function_)(p1_, p2_, a1);
        } else {
            RetType result = (*function_)(p1_, p2_, a1);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename P1, typename P2, typename A1>
class FunctionResultCallback21<del, void, P1, P2, A1> : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Function = void (*)(P1, P2, A1);
    inline FunctionResultCallback21(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2)
        : function_(function), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (*function_)(p1_, p2_, a1);
        } else {
            (*function_)(p1_, p2_, a1);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename RetType, typename P1, typename P2, typename A1>
inline typename FunctionResultCallback21<true, RetType, P1, P2, A1>::base* MakeCallback(
    RetType (*function)(P1, P2, A1),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new FunctionResultCallback21<true, RetType, P1, P2, A1>(function, p1, p2);
}

template <typename RetType, typename P1, typename P2, typename A1>
inline typename FunctionResultCallback21<false, RetType, P1, P2, A1>::base* MakePermanentCallback(
    RetType (*function)(P1, P2, A1),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new FunctionResultCallback21<false, RetType, P1, P2, A1>(function, p1, p2);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback22 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Method = RetType (Class::*)(P1, P2, A1, A2) const;
    inline ConstMemberResultCallback22(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, a1, a2);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, a1, a2);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename Class, typename P1, typename P2, typename A1, typename A2>
class ConstMemberResultCallback22<del,
                                  void,
                                  Class,
                                  P1,
                                  P2,
                                  A1,
                                  A2,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Method = void (Class::*)(P1, P2, A1, A2) const;
    inline ConstMemberResultCallback22(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, a1, a2);
        } else {
            (instance_->*method_)(p1_, p2_, a1, a2);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2>
inline typename ConstMemberResultCallback22<true, RetType, Caller, P1, P2, A1, A2>::base*
MakeCallback(const Caller* instance,
             RetType (Callee::*method)(P1, P2, A1, A2) const,
             typename internal::ConstRef<P1>::type p1,
             typename internal::ConstRef<P2>::type p2) {
    return new ConstMemberResultCallback22<true, RetType, Caller, P1, P2, A1, A2>(instance, method,
                                                                                  p1, p2);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2>
inline typename ConstMemberResultCallback22<false, RetType, Caller, P1, P2, A1, A2>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, P2, A1, A2) const,
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new ConstMemberResultCallback22<false, RetType, Caller, P1, P2, A1, A2>(instance, method,
                                                                                   p1, p2);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback22 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Method = RetType (Class::*)(P1, P2, A1, A2);
    inline MemberResultCallback22(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, a1, a2);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, a1, a2);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename Class, typename P1, typename P2, typename A1, typename A2>
class MemberResultCallback22<del,
                             void,
                             Class,
                             P1,
                             P2,
                             A1,
                             A2,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Method = void (Class::*)(P1, P2, A1, A2);
    inline MemberResultCallback22(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, a1, a2);
        } else {
            (instance_->*method_)(p1_, p2_, a1, a2);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2>
inline typename MemberResultCallback22<true, RetType, Caller, P1, P2, A1, A2>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, P2, A1, A2),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new MemberResultCallback22<true, RetType, Caller, P1, P2, A1, A2>(instance, method, p1,
                                                                             p2);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2>
inline typename MemberResultCallback22<false, RetType, Caller, P1, P2, A1, A2>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, P2, A1, A2),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new MemberResultCallback22<false, RetType, Caller, P1, P2, A1, A2>(instance, method, p1,
                                                                              p2);
}
template <bool del, typename RetType, typename P1, typename P2, typename A1, typename A2>
class FunctionResultCallback22 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Function = RetType (*)(P1, P2, A1, A2);
    inline FunctionResultCallback22(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2)
        : function_(function), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (*function_)(p1_, p2_, a1, a2);
        } else {
            RetType result = (*function_)(p1_, p2_, a1, a2);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename P1, typename P2, typename A1, typename A2>
class FunctionResultCallback22<del, void, P1, P2, A1, A2> : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Function = void (*)(P1, P2, A1, A2);
    inline FunctionResultCallback22(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2)
        : function_(function), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (*function_)(p1_, p2_, a1, a2);
        } else {
            (*function_)(p1_, p2_, a1, a2);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename RetType, typename P1, typename P2, typename A1, typename A2>
inline typename FunctionResultCallback22<true, RetType, P1, P2, A1, A2>::base* MakeCallback(
    RetType (*function)(P1, P2, A1, A2),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new FunctionResultCallback22<true, RetType, P1, P2, A1, A2>(function, p1, p2);
}

template <typename RetType, typename P1, typename P2, typename A1, typename A2>
inline typename FunctionResultCallback22<false, RetType, P1, P2, A1, A2>::base*
MakePermanentCallback(RetType (*function)(P1, P2, A1, A2),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new FunctionResultCallback22<false, RetType, P1, P2, A1, A2>(function, p1, p2);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename A3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback23 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Method = RetType (Class::*)(P1, P2, A1, A2, A3) const;
    inline ConstMemberResultCallback23(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, a1, a2, a3);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, a1, a2, a3);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename Class, typename P1, typename P2, typename A1, typename A2, typename A3>
class ConstMemberResultCallback23<del,
                                  void,
                                  Class,
                                  P1,
                                  P2,
                                  A1,
                                  A2,
                                  A3,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Method = void (Class::*)(P1, P2, A1, A2, A3) const;
    inline ConstMemberResultCallback23(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, a1, a2, a3);
        } else {
            (instance_->*method_)(p1_, p2_, a1, a2, a3);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename A3>
inline typename ConstMemberResultCallback23<true, RetType, Caller, P1, P2, A1, A2, A3>::base*
MakeCallback(const Caller* instance,
             RetType (Callee::*method)(P1, P2, A1, A2, A3) const,
             typename internal::ConstRef<P1>::type p1,
             typename internal::ConstRef<P2>::type p2) {
    return new ConstMemberResultCallback23<true, RetType, Caller, P1, P2, A1, A2, A3>(
        instance, method, p1, p2);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename A3>
inline typename ConstMemberResultCallback23<false, RetType, Caller, P1, P2, A1, A2, A3>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, P2, A1, A2, A3) const,
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new ConstMemberResultCallback23<false, RetType, Caller, P1, P2, A1, A2, A3>(
        instance, method, p1, p2);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename A3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback23 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Method = RetType (Class::*)(P1, P2, A1, A2, A3);
    inline MemberResultCallback23(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, a1, a2, a3);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, a1, a2, a3);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename Class, typename P1, typename P2, typename A1, typename A2, typename A3>
class MemberResultCallback23<del,
                             void,
                             Class,
                             P1,
                             P2,
                             A1,
                             A2,
                             A3,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Method = void (Class::*)(P1, P2, A1, A2, A3);
    inline MemberResultCallback23(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2)
        : instance_(instance), method_(method), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, a1, a2, a3);
        } else {
            (instance_->*method_)(p1_, p2_, a1, a2, a3);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename A3>
inline typename MemberResultCallback23<true, RetType, Caller, P1, P2, A1, A2, A3>::base*
MakeCallback(Caller* instance,
             RetType (Callee::*method)(P1, P2, A1, A2, A3),
             typename internal::ConstRef<P1>::type p1,
             typename internal::ConstRef<P2>::type p2) {
    return new MemberResultCallback23<true, RetType, Caller, P1, P2, A1, A2, A3>(instance, method,
                                                                                 p1, p2);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename A3>
inline typename MemberResultCallback23<false, RetType, Caller, P1, P2, A1, A2, A3>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, P2, A1, A2, A3),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new MemberResultCallback23<false, RetType, Caller, P1, P2, A1, A2, A3>(instance, method,
                                                                                  p1, p2);
}
template <bool del,
          typename RetType,
          typename P1,
          typename P2,
          typename A1,
          typename A2,
          typename A3>
class FunctionResultCallback23 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Function = RetType (*)(P1, P2, A1, A2, A3);
    inline FunctionResultCallback23(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2)
        : function_(function), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (*function_)(p1_, p2_, a1, a2, a3);
        } else {
            RetType result = (*function_)(p1_, p2_, a1, a2, a3);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};
template <bool del, typename P1, typename P2, typename A1, typename A2, typename A3>
class FunctionResultCallback23<del, void, P1, P2, A1, A2, A3> : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Function = void (*)(P1, P2, A1, A2, A3);
    inline FunctionResultCallback23(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2)
        : function_(function), p1_(p1), p2_(p2) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (*function_)(p1_, p2_, a1, a2, a3);
        } else {
            (*function_)(p1_, p2_, a1, a2, a3);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
};

template <typename RetType, typename P1, typename P2, typename A1, typename A2, typename A3>
inline typename FunctionResultCallback23<true, RetType, P1, P2, A1, A2, A3>::base* MakeCallback(
    RetType (*function)(P1, P2, A1, A2, A3),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2) {
    return new FunctionResultCallback23<true, RetType, P1, P2, A1, A2, A3>(function, p1, p2);
}

template <typename RetType, typename P1, typename P2, typename A1, typename A2, typename A3>
inline typename FunctionResultCallback23<false, RetType, P1, P2, A1, A2, A3>::base*
MakePermanentCallback(RetType (*function)(P1, P2, A1, A2, A3),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2) {
    return new FunctionResultCallback23<false, RetType, P1, P2, A1, A2, A3>(function, p1, p2);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback30 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Method = RetType (Class::*)(P1, P2, P3) const;
    inline ConstMemberResultCallback30(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2,
                                       typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, p3_);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, p3_);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename Class, typename P1, typename P2, typename P3>
class ConstMemberResultCallback30<del,
                                  void,
                                  Class,
                                  P1,
                                  P2,
                                  P3,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback {
  public:
    using base = Callback;
    using Method = void (Class::*)(P1, P2, P3) const;
    inline ConstMemberResultCallback30(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2,
                                       typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, p3_);
        } else {
            (instance_->*method_)(p1_, p2_, p3_);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2, typename P3>
inline typename ConstMemberResultCallback30<true, RetType, Caller, P1, P2, P3>::base* MakeCallback(
    const Caller* instance,
    RetType (Callee::*method)(P1, P2, P3) const,
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2,
    typename internal::ConstRef<P3>::type p3) {
    return new ConstMemberResultCallback30<true, RetType, Caller, P1, P2, P3>(instance, method, p1,
                                                                              p2, p3);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2, typename P3>
inline typename ConstMemberResultCallback30<false, RetType, Caller, P1, P2, P3>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, P2, P3) const,
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new ConstMemberResultCallback30<false, RetType, Caller, P1, P2, P3>(instance, method, p1,
                                                                               p2, p3);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback30 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Method = RetType (Class::*)(P1, P2, P3);
    inline MemberResultCallback30(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2,
                                  typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, p3_);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, p3_);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename Class, typename P1, typename P2, typename P3>
class MemberResultCallback30<del,
                             void,
                             Class,
                             P1,
                             P2,
                             P3,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback {
  public:
    using base = Callback;
    using Method = void (Class::*)(P1, P2, P3);
    inline MemberResultCallback30(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2,
                                  typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, p3_);
        } else {
            (instance_->*method_)(p1_, p2_, p3_);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2, typename P3>
inline typename MemberResultCallback30<true, RetType, Caller, P1, P2, P3>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, P2, P3),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2,
    typename internal::ConstRef<P3>::type p3) {
    return new MemberResultCallback30<true, RetType, Caller, P1, P2, P3>(instance, method, p1, p2,
                                                                         p3);
}

template <typename Caller, typename Callee, typename RetType, typename P1, typename P2, typename P3>
inline typename MemberResultCallback30<false, RetType, Caller, P1, P2, P3>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, P2, P3),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new MemberResultCallback30<false, RetType, Caller, P1, P2, P3>(instance, method, p1, p2,
                                                                          p3);
}
template <bool del, typename RetType, typename P1, typename P2, typename P3>
class FunctionResultCallback30 : public ResultCallback<RetType> {
  public:
    using base = ResultCallback<RetType>;
    using Function = RetType (*)(P1, P2, P3);
    inline FunctionResultCallback30(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2,
                                    typename internal::ConstRef<P3>::type p3)
        : function_(function), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run() override {
        if (!del) {
            return (*function_)(p1_, p2_, p3_);
        } else {
            RetType result = (*function_)(p1_, p2_, p3_);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename P1, typename P2, typename P3>
class FunctionResultCallback30<del, void, P1, P2, P3> : public Callback {
  public:
    using base = Callback;
    using Function = void (*)(P1, P2, P3);
    inline FunctionResultCallback30(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2,
                                    typename internal::ConstRef<P3>::type p3)
        : function_(function), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run() override {
        if (!del) {
            (*function_)(p1_, p2_, p3_);
        } else {
            (*function_)(p1_, p2_, p3_);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename RetType, typename P1, typename P2, typename P3>
inline typename FunctionResultCallback30<true, RetType, P1, P2, P3>::base* MakeCallback(
    RetType (*function)(P1, P2, P3),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2,
    typename internal::ConstRef<P3>::type p3) {
    return new FunctionResultCallback30<true, RetType, P1, P2, P3>(function, p1, p2, p3);
}

template <typename RetType, typename P1, typename P2, typename P3>
inline typename FunctionResultCallback30<false, RetType, P1, P2, P3>::base* MakePermanentCallback(
    RetType (*function)(P1, P2, P3),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2,
    typename internal::ConstRef<P3>::type p3) {
    return new FunctionResultCallback30<false, RetType, P1, P2, P3>(function, p1, p2, p3);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback31 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Method = RetType (Class::*)(P1, P2, P3, A1) const;
    inline ConstMemberResultCallback31(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2,
                                       typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, p3_, a1);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, p3_, a1);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename Class, typename P1, typename P2, typename P3, typename A1>
class ConstMemberResultCallback31<del,
                                  void,
                                  Class,
                                  P1,
                                  P2,
                                  P3,
                                  A1,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Method = void (Class::*)(P1, P2, P3, A1) const;
    inline ConstMemberResultCallback31(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2,
                                       typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, p3_, a1);
        } else {
            (instance_->*method_)(p1_, p2_, p3_, a1);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1>
inline typename ConstMemberResultCallback31<true, RetType, Caller, P1, P2, P3, A1>::base*
MakeCallback(const Caller* instance,
             RetType (Callee::*method)(P1, P2, P3, A1) const,
             typename internal::ConstRef<P1>::type p1,
             typename internal::ConstRef<P2>::type p2,
             typename internal::ConstRef<P3>::type p3) {
    return new ConstMemberResultCallback31<true, RetType, Caller, P1, P2, P3, A1>(instance, method,
                                                                                  p1, p2, p3);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1>
inline typename ConstMemberResultCallback31<false, RetType, Caller, P1, P2, P3, A1>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, P2, P3, A1) const,
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new ConstMemberResultCallback31<false, RetType, Caller, P1, P2, P3, A1>(instance, method,
                                                                                   p1, p2, p3);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback31 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Method = RetType (Class::*)(P1, P2, P3, A1);
    inline MemberResultCallback31(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2,
                                  typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, p3_, a1);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, p3_, a1);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename Class, typename P1, typename P2, typename P3, typename A1>
class MemberResultCallback31<del,
                             void,
                             Class,
                             P1,
                             P2,
                             P3,
                             A1,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Method = void (Class::*)(P1, P2, P3, A1);
    inline MemberResultCallback31(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2,
                                  typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, p3_, a1);
        } else {
            (instance_->*method_)(p1_, p2_, p3_, a1);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1>
inline typename MemberResultCallback31<true, RetType, Caller, P1, P2, P3, A1>::base* MakeCallback(
    Caller* instance,
    RetType (Callee::*method)(P1, P2, P3, A1),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2,
    typename internal::ConstRef<P3>::type p3) {
    return new MemberResultCallback31<true, RetType, Caller, P1, P2, P3, A1>(instance, method, p1,
                                                                             p2, p3);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1>
inline typename MemberResultCallback31<false, RetType, Caller, P1, P2, P3, A1>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, P2, P3, A1),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new MemberResultCallback31<false, RetType, Caller, P1, P2, P3, A1>(instance, method, p1,
                                                                              p2, p3);
}
template <bool del, typename RetType, typename P1, typename P2, typename P3, typename A1>
class FunctionResultCallback31 : public ResultCallback1<RetType, A1> {
  public:
    using base = ResultCallback1<RetType, A1>;
    using Function = RetType (*)(P1, P2, P3, A1);
    inline FunctionResultCallback31(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2,
                                    typename internal::ConstRef<P3>::type p3)
        : function_(function), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1) override {
        if (!del) {
            return (*function_)(p1_, p2_, p3_, a1);
        } else {
            RetType result = (*function_)(p1_, p2_, p3_, a1);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename P1, typename P2, typename P3, typename A1>
class FunctionResultCallback31<del, void, P1, P2, P3, A1> : public Callback1<A1> {
  public:
    using base = Callback1<A1>;
    using Function = void (*)(P1, P2, P3, A1);
    inline FunctionResultCallback31(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2,
                                    typename internal::ConstRef<P3>::type p3)
        : function_(function), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1) override {
        if (!del) {
            (*function_)(p1_, p2_, p3_, a1);
        } else {
            (*function_)(p1_, p2_, p3_, a1);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename RetType, typename P1, typename P2, typename P3, typename A1>
inline typename FunctionResultCallback31<true, RetType, P1, P2, P3, A1>::base* MakeCallback(
    RetType (*function)(P1, P2, P3, A1),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2,
    typename internal::ConstRef<P3>::type p3) {
    return new FunctionResultCallback31<true, RetType, P1, P2, P3, A1>(function, p1, p2, p3);
}

template <typename RetType, typename P1, typename P2, typename P3, typename A1>
inline typename FunctionResultCallback31<false, RetType, P1, P2, P3, A1>::base*
MakePermanentCallback(RetType (*function)(P1, P2, P3, A1),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new FunctionResultCallback31<false, RetType, P1, P2, P3, A1>(function, p1, p2, p3);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback32 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Method = RetType (Class::*)(P1, P2, P3, A1, A2) const;
    inline ConstMemberResultCallback32(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2,
                                       typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, p3_, a1, a2);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, p3_, a1, a2);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename Class, typename P1, typename P2, typename P3, typename A1, typename A2>
class ConstMemberResultCallback32<del,
                                  void,
                                  Class,
                                  P1,
                                  P2,
                                  P3,
                                  A1,
                                  A2,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Method = void (Class::*)(P1, P2, P3, A1, A2) const;
    inline ConstMemberResultCallback32(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2,
                                       typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, p3_, a1, a2);
        } else {
            (instance_->*method_)(p1_, p2_, p3_, a1, a2);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2>
inline typename ConstMemberResultCallback32<true, RetType, Caller, P1, P2, P3, A1, A2>::base*
MakeCallback(const Caller* instance,
             RetType (Callee::*method)(P1, P2, P3, A1, A2) const,
             typename internal::ConstRef<P1>::type p1,
             typename internal::ConstRef<P2>::type p2,
             typename internal::ConstRef<P3>::type p3) {
    return new ConstMemberResultCallback32<true, RetType, Caller, P1, P2, P3, A1, A2>(
        instance, method, p1, p2, p3);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2>
inline typename ConstMemberResultCallback32<false, RetType, Caller, P1, P2, P3, A1, A2>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, P2, P3, A1, A2) const,
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new ConstMemberResultCallback32<false, RetType, Caller, P1, P2, P3, A1, A2>(
        instance, method, p1, p2, p3);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback32 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Method = RetType (Class::*)(P1, P2, P3, A1, A2);
    inline MemberResultCallback32(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2,
                                  typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, p3_, a1, a2);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, p3_, a1, a2);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename Class, typename P1, typename P2, typename P3, typename A1, typename A2>
class MemberResultCallback32<del,
                             void,
                             Class,
                             P1,
                             P2,
                             P3,
                             A1,
                             A2,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Method = void (Class::*)(P1, P2, P3, A1, A2);
    inline MemberResultCallback32(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2,
                                  typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, p3_, a1, a2);
        } else {
            (instance_->*method_)(p1_, p2_, p3_, a1, a2);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2>
inline typename MemberResultCallback32<true, RetType, Caller, P1, P2, P3, A1, A2>::base*
MakeCallback(Caller* instance,
             RetType (Callee::*method)(P1, P2, P3, A1, A2),
             typename internal::ConstRef<P1>::type p1,
             typename internal::ConstRef<P2>::type p2,
             typename internal::ConstRef<P3>::type p3) {
    return new MemberResultCallback32<true, RetType, Caller, P1, P2, P3, A1, A2>(instance, method,
                                                                                 p1, p2, p3);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2>
inline typename MemberResultCallback32<false, RetType, Caller, P1, P2, P3, A1, A2>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, P2, P3, A1, A2),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new MemberResultCallback32<false, RetType, Caller, P1, P2, P3, A1, A2>(instance, method,
                                                                                  p1, p2, p3);
}
template <bool del,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2>
class FunctionResultCallback32 : public ResultCallback2<RetType, A1, A2> {
  public:
    using base = ResultCallback2<RetType, A1, A2>;
    using Function = RetType (*)(P1, P2, P3, A1, A2);
    inline FunctionResultCallback32(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2,
                                    typename internal::ConstRef<P3>::type p3)
        : function_(function), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2) override {
        if (!del) {
            return (*function_)(p1_, p2_, p3_, a1, a2);
        } else {
            RetType result = (*function_)(p1_, p2_, p3_, a1, a2);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename P1, typename P2, typename P3, typename A1, typename A2>
class FunctionResultCallback32<del, void, P1, P2, P3, A1, A2> : public Callback2<A1, A2> {
  public:
    using base = Callback2<A1, A2>;
    using Function = void (*)(P1, P2, P3, A1, A2);
    inline FunctionResultCallback32(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2,
                                    typename internal::ConstRef<P3>::type p3)
        : function_(function), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2) override {
        if (!del) {
            (*function_)(p1_, p2_, p3_, a1, a2);
        } else {
            (*function_)(p1_, p2_, p3_, a1, a2);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename RetType, typename P1, typename P2, typename P3, typename A1, typename A2>
inline typename FunctionResultCallback32<true, RetType, P1, P2, P3, A1, A2>::base* MakeCallback(
    RetType (*function)(P1, P2, P3, A1, A2),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2,
    typename internal::ConstRef<P3>::type p3) {
    return new FunctionResultCallback32<true, RetType, P1, P2, P3, A1, A2>(function, p1, p2, p3);
}

template <typename RetType, typename P1, typename P2, typename P3, typename A1, typename A2>
inline typename FunctionResultCallback32<false, RetType, P1, P2, P3, A1, A2>::base*
MakePermanentCallback(RetType (*function)(P1, P2, P3, A1, A2),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new FunctionResultCallback32<false, RetType, P1, P2, P3, A1, A2>(function, p1, p2, p3);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class ConstMemberResultCallback33 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Method = RetType (Class::*)(P1, P2, P3, A1, A2, A3) const;
    inline ConstMemberResultCallback33(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2,
                                       typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, p3_, a1, a2, a3);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, p3_, a1, a2, a3);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
class ConstMemberResultCallback33<del,
                                  void,
                                  Class,
                                  P1,
                                  P2,
                                  P3,
                                  A1,
                                  A2,
                                  A3,
                                  typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Method = void (Class::*)(P1, P2, P3, A1, A2, A3) const;
    inline ConstMemberResultCallback33(const Class* instance,
                                       Method method,
                                       typename internal::ConstRef<P1>::type p1,
                                       typename internal::ConstRef<P2>::type p2,
                                       typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, p3_, a1, a2, a3);
        } else {
            (instance_->*method_)(p1_, p2_, p3_, a1, a2, a3);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    const Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
inline typename ConstMemberResultCallback33<true, RetType, Caller, P1, P2, P3, A1, A2, A3>::base*
MakeCallback(const Caller* instance,
             RetType (Callee::*method)(P1, P2, P3, A1, A2, A3) const,
             typename internal::ConstRef<P1>::type p1,
             typename internal::ConstRef<P2>::type p2,
             typename internal::ConstRef<P3>::type p3) {
    return new ConstMemberResultCallback33<true, RetType, Caller, P1, P2, P3, A1, A2, A3>(
        instance, method, p1, p2, p3);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
inline typename ConstMemberResultCallback33<false, RetType, Caller, P1, P2, P3, A1, A2, A3>::base*
MakePermanentCallback(const Caller* instance,
                      RetType (Callee::*method)(P1, P2, P3, A1, A2, A3) const,
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new ConstMemberResultCallback33<false, RetType, Caller, P1, P2, P3, A1, A2, A3>(
        instance, method, p1, p2, p3);
}
template <bool del,
          typename RetType,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3,
          typename OnlyIf = typename ::std::enable_if<::std::is_compound<Class>::value>::type>
class MemberResultCallback33 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Method = RetType (Class::*)(P1, P2, P3, A1, A2, A3);
    inline MemberResultCallback33(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2,
                                  typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (instance_->*method_)(p1_, p2_, p3_, a1, a2, a3);
        } else {
            RetType result = (instance_->*method_)(p1_, p2_, p3_, a1, a2, a3);
            method_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del,
          typename Class,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
class MemberResultCallback33<del,
                             void,
                             Class,
                             P1,
                             P2,
                             P3,
                             A1,
                             A2,
                             A3,
                             typename ::std::enable_if<::std::is_compound<Class>::value>::type>
    : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Method = void (Class::*)(P1, P2, P3, A1, A2, A3);
    inline MemberResultCallback33(Class* instance,
                                  Method method,
                                  typename internal::ConstRef<P1>::type p1,
                                  typename internal::ConstRef<P2>::type p2,
                                  typename internal::ConstRef<P3>::type p3)
        : instance_(instance), method_(method), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (instance_->*method_)(p1_, p2_, p3_, a1, a2, a3);
        } else {
            (instance_->*method_)(p1_, p2_, p3_, a1, a2, a3);
            method_ = nullptr;
            delete this;
        }
    }

  private:
    Class* instance_;
    Method method_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
inline typename MemberResultCallback33<true, RetType, Caller, P1, P2, P3, A1, A2, A3>::base*
MakeCallback(Caller* instance,
             RetType (Callee::*method)(P1, P2, P3, A1, A2, A3),
             typename internal::ConstRef<P1>::type p1,
             typename internal::ConstRef<P2>::type p2,
             typename internal::ConstRef<P3>::type p3) {
    return new MemberResultCallback33<true, RetType, Caller, P1, P2, P3, A1, A2, A3>(
        instance, method, p1, p2, p3);
}

template <typename Caller,
          typename Callee,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
inline typename MemberResultCallback33<false, RetType, Caller, P1, P2, P3, A1, A2, A3>::base*
MakePermanentCallback(Caller* instance,
                      RetType (Callee::*method)(P1, P2, P3, A1, A2, A3),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new MemberResultCallback33<false, RetType, Caller, P1, P2, P3, A1, A2, A3>(
        instance, method, p1, p2, p3);
}
template <bool del,
          typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
class FunctionResultCallback33 : public ResultCallback3<RetType, A1, A2, A3> {
  public:
    using base = ResultCallback3<RetType, A1, A2, A3>;
    using Function = RetType (*)(P1, P2, P3, A1, A2, A3);
    inline FunctionResultCallback33(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2,
                                    typename internal::ConstRef<P3>::type p3)
        : function_(function), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    RetType run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            return (*function_)(p1_, p2_, p3_, a1, a2, a3);
        } else {
            RetType result = (*function_)(p1_, p2_, p3_, a1, a2, a3);
            function_ = nullptr;
            delete this;
            return result;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};
template <bool del, typename P1, typename P2, typename P3, typename A1, typename A2, typename A3>
class FunctionResultCallback33<del, void, P1, P2, P3, A1, A2, A3> : public Callback3<A1, A2, A3> {
  public:
    using base = Callback3<A1, A2, A3>;
    using Function = void (*)(P1, P2, P3, A1, A2, A3);
    inline FunctionResultCallback33(Function function,
                                    typename internal::ConstRef<P1>::type p1,
                                    typename internal::ConstRef<P2>::type p2,
                                    typename internal::ConstRef<P3>::type p3)
        : function_(function), p1_(p1), p2_(p2), p3_(p3) {}
    bool isRepeatable() const override { return !del; }
    void run(A1 a1, A2 a2, A3 a3) override {
        if (!del) {
            (*function_)(p1_, p2_, p3_, a1, a2, a3);
        } else {
            (*function_)(p1_, p2_, p3_, a1, a2, a3);
            function_ = nullptr;
            delete this;
        }
    }

  private:
    Function function_;
    typename ::std::remove_reference<P1>::type p1_;
    typename ::std::remove_reference<P2>::type p2_;
    typename ::std::remove_reference<P3>::type p3_;
};

template <typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
inline typename FunctionResultCallback33<true, RetType, P1, P2, P3, A1, A2, A3>::base* MakeCallback(
    RetType (*function)(P1, P2, P3, A1, A2, A3),
    typename internal::ConstRef<P1>::type p1,
    typename internal::ConstRef<P2>::type p2,
    typename internal::ConstRef<P3>::type p3) {
    return new FunctionResultCallback33<true, RetType, P1, P2, P3, A1, A2, A3>(function, p1, p2,
                                                                               p3);
}

template <typename RetType,
          typename P1,
          typename P2,
          typename P3,
          typename A1,
          typename A2,
          typename A3>
inline typename FunctionResultCallback33<false, RetType, P1, P2, P3, A1, A2, A3>::base*
MakePermanentCallback(RetType (*function)(P1, P2, P3, A1, A2, A3),
                      typename internal::ConstRef<P1>::type p1,
                      typename internal::ConstRef<P2>::type p2,
                      typename internal::ConstRef<P3>::type p3) {
    return new FunctionResultCallback33<false, RetType, P1, P2, P3, A1, A2, A3>(function, p1, p2,
                                                                                p3);
}
}  // namespace kwc

#endif