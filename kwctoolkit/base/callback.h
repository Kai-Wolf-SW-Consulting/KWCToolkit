// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_CALLBACK_H_
#define KWCTOOLKIT_BASE_CALLBACK_H_

// Callback Generic callback implementation
//
// The following callback implementation provides a generic
// interface for all classes which require a callback from another class.
//
// Function Signature
// The following callbacks with up to 3 arguments are provided:
//
//     Callback               --> provides "void run()"
//     Callback1<P1>          --> provides "void run(P1)"
//     Callback2<P1, P2>      --> provides "void run(P1, P2)"
//     Callback3<P1, P2, P3>  --> provides "void run(P1, P2, P3)"
//
// Furthermore 'ResultCallback' classes provide a generic interface for all
// callbacks which additionally return a value:
//
//     ResultCallback<Ret>               --> provides "Ret run()"
//     ResultCallback1<Ret, P1>          --> provides "Ret run(P1)"
//     ResultCallback2<Ret, P1, P2>      --> provides "Ret run(P1, P2)"
//     ResultCallback3<Ret, P1, P2, P3>  --> provides "Ret run(P1, P2, P3)"
//
// In order to instantiate one of the callbacks described above a convenient
// factory MakeCallback is provided which may be called with an object pointer,
// a pointer to a free function or a pointer to a member function with the
// appropriate signature.
//
// Return types
// The returned callback objects from 'MakeCallback' are self-deleting, i.e.
// they delete themselves after they've been invoked once. In case a given
// callback object has to be called repeatedly or several times, one should use
// 'MakePermanentCallback'. Note that in the latter case, one has to take care
// of the correct deletion of the callback in order to avoid memory leaks.
//
// There are two types of arguments which may be passed to the callback:
//   - "pre-bound arguments" -> supplied when the callback is created
//   - "call-time arguments" -> supplied when the callback is invoked
//
// These two types correspond to "early binding" and "late binding". In case
// an argument whose value is known at the creation time of the callback may
// be "pre-bound" to the callback. Note that for creating a callback with 3
// pre-bound arguments and 1 call-time argument for example has the same effect
// as creating a callback with 2 pre-bound arguments and 2 call-time arguments
// or 0 pre-bound arguments and 4 call-time arguments.
// Example:
//
//     struct Foo {
//         void bar(int a, double b);
//     };
//
//     Foo* foo_ = new Foo;                      // [pre-bound] + [call-time]
//     MakeCallback(foo_, &Foo::bar)             --> run(23, 1.7); // 0 + 2
//     == MakeCallback(foo_, &Foo::bar, 10)      --> run(1.7) ;    // 1 + 1
//     == MakeCallback(foo_, &Foo::bar, 10, 1.7) --> run();        // 2 + 0
//     == foo_->bar(23, 1.7);

#include "kwctoolkit/base/callback_impl.h"

namespace kwc {
class Callback;

template <typename Instance>
void DeletePointer(Instance* instance) {
    delete instance;
}

template <typename Instance>
Callback* DeletePointerCallback(Instance* instance) {
    return MakeCallback(&DeletePointer<Instance>, instance);
}

}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_CALLBACK_H_
