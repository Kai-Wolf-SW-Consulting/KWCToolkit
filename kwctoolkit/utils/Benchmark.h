// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_UTILS_BENCHMARK_H_
#define KWCTOOLKIT_UTILS_BENCHMARK_H_

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "kwctoolkit/base/IntegralTypes.h"

namespace kwc {
namespace utils {
class Benchmark;

using BenchmarkList = std::vector<class Benchmark*>;

enum class ContextState { Idle, Running, AreaBench };

template <typename Clock>
class BasicContext {
  public:
    BasicContext(std::chrono::nanoseconds duration)
        : state_(ContextState::Idle), duration_(duration) {}

    // Keeps the benchmark running for the duration_ given in the constructor.
    // Return true as long as there is time left.
    bool running() {
        if (state_ == ContextState::AreaBench) {
            if (run_time_ > duration_) {
                return false;
            }
            iterations_++;
            return true;
        }
        if (state_ == ContextState::Idle) {
            state_ = ContextState::Running;
            iterations_ = 1;
            start_ = Clock::now();
            return true;
        }
        auto now = Clock::now();
        run_time_ = now - start_;
        if (run_time_ >= duration_) {
            return false;
        }

        iterations_++;
        return true;
    }

  protected:
    int64 timePerIteration(int64 overhead = 0) const {
        auto per_it = run_time_.count() / iterations_;

        if (state_ != ContextState::AreaBench) {
            per_it -= overhead;
        }

        return per_it;
    }

    int64 iterations() const { return iterations_; }

    void beginArea() {
        if (state_ != ContextState::AreaBench) {
            // reset everything set by running()
            iterations_ = 1;
            run_time_ = std::chrono::nanoseconds::zero();
            state_ = ContextState::AreaBench;
        }
        start_ = Clock::now();
    }

    void endArea() {
        auto now = Clock::now();
        run_time_ += (now - start_);
    }

  private:
    ContextState state_;
    int64 iterations_;
    typename Clock::time_point start_;
    std::chrono::nanoseconds duration_;
    std::chrono::nanoseconds run_time_;

    friend class BenchmarkArea;
    friend class Benchmark;
};

using Context = BasicContext<std::chrono::high_resolution_clock>;

class BenchmarkArea {
  public:
    BenchmarkArea(Context& context) : context_(context) { context.beginArea(); }

    ~BenchmarkArea() { context_.endArea(); }

  private:
    Context& context_;
};

enum class Color { White = 0, Green = 32, Yellow = 33, Cyan = 36 };

class ConsoleModifier {
  public:
    ConsoleModifier(Color c) : code_(c) {}
    friend std::ostream& operator<<(std::ostream& os, const ConsoleModifier& cm) {
        return os << "\033[" << static_cast<int>(cm.code_) << "m";
    }

  private:
    Color code_;
};

class Benchmark {
  private:
    static std::string filterArgument(int argc, const char* argv[]) {
        // iterate over all arguments and find --benchmark_filter=
        std::regex filter_regex{"--benchmark_filter=(.*)"};
        std::smatch base_match;

        for (int i = 0; i < argc; i++) {
            std::string argument{argv[i]};
            if (std::regex_match(argument, base_match, filter_regex)) {
                if (base_match.size() == 2) {
                    std::ssub_match sub_match = base_match[1];
                    std::string filter = sub_match.str();
                    // replace all occurrences of * with .*
                    filter = std::regex_replace(filter, std::regex("\\*"), ".*");

                    return filter;
                }
            }
        }

        return std::string(".*");
    }

  public:
    static BenchmarkList& list() {
        // Currently elements in list are never destroyed, but as
        // the lifetime of list is the lifetime of the process, we
        // don't bother using unique_ptr
        static BenchmarkList list;
        return list;
    }

    // empty benchmark to measure the overhead of context_.running()
    static void emptyBenchMark(Context& context) {
        while (context.running())
            ;
    }

    static void runAllBenchmarks(int argc = 0, const char* argv[] = nullptr) {
        auto max_length = maxNameLength();
        auto nano_length = std::string("Time").length() + 10;
        auto it_length = std::string("iterations").length();

        auto filter = filterArgument(argc, argv);

        std::regex filter_regex;
        try {
            filter_regex = std::regex(filter);
        } catch (std::regex_error) {
            std::cout << "Invalid filter: " << filter << std::endl;
            exit(1);
        }

        Context overhead_context(std::chrono::seconds(1));
        emptyBenchMark(overhead_context);

        std::cout << std::left << std::setw(max_length) << "Name"
                  << "               "
                  << "Time  iterations" << std::endl;
        std::string dashes;
        dashes.insert(0, max_length + 3 + nano_length + 4 + it_length, '-');
        std::cout << dashes << std::endl;

        for (auto benchmark : list()) {
            std::smatch base_match;
            if (!std::regex_match(benchmark->name(), base_match, filter_regex))
                continue;

            Context context(std::chrono::seconds(1));
            benchmark->setUp();
            benchmark->runBenchmark(context);
            benchmark->tearDown();
            std::ostringstream ost;
            int64 time_per_iter = context.timePerIteration(overhead_context.timePerIteration());
            time_per_iter = std::max<int64>(time_per_iter, 0ll);
            ost << time_per_iter;
            auto nano_str = ost.str();

            std::cout << ConsoleModifier(Color::Green);
            std::cout << std::left << std::setw(max_length);
            std::cout << benchmark->name();
            std::cout << "  ";

            std::cout << ConsoleModifier(Color::Yellow);
            std::cout << std::right << std::setw(nano_length) << nano_str << " ns";

            std::ostringstream itstrstr;
            itstrstr << context.iterations();
            auto it_str = itstrstr.str();

            std::cout << "  ";
            std::cout << ConsoleModifier(Color::Cyan);
            std::cout << std::right << std::setw(it_length) << it_str << std::endl;
            std::cout << "\033[0m";
        }
    }

    static size_t maxNameLength() {
        size_t length = 0;
        for (const auto& benchmark : list()) {
            length = benchmark->name().length() > length ? benchmark->name().length() : length;
        }
        return length;
    }

    const std::string& name() const { return name_; }

    std::string& name() { return name_; }

  protected:
    virtual void runBenchmark(Context& context){};
    virtual void setUp() {}
    virtual void tearDown() {}

  protected:
    std::string name_;
};

#define _BM_CONCATX(A, B) A##B
#define _BM_CONCAT(A, B) _BM_CONCATX(A, B)
#define _BM_STRX(X) #X
#define _BM_STR(X) _BM_STRX(X)

#define BENCHMARK(NAME)                                    \
    class NAME : public kwc::Benchmark {                   \
      public:                                              \
        static class _init {                               \
          public:                                          \
            _init() {                                      \
                kwc::Benchmark* bench = new NAME();        \
                bench->Name() = #NAME;                     \
                NAME::List().push_back(bench);             \
            }                                              \
        } _initializer;                                    \
                                                           \
      protected:                                           \
        void RunBenchmark(kwc::Context& context) override; \
    };                                                     \
    NAME::_init NAME::_initializer;                        \
                                                           \
    void NAME::RunBenchmark(kwc::Context& context)

#define BENCHMARK_F(FIXTURE, NAME)                                            \
    class _BM_CONCAT(FIXTURE, NAME) : public FIXTURE {                        \
      public:                                                                 \
        static class _init {                                                  \
          public:                                                             \
            _init() {                                                         \
                kwc::Benchmark* bench = new _BM_CONCAT(FIXTURE, NAME)();      \
                bench->Name() = _BM_STR(FIXTURE) "." _BM_STR(NAME);           \
                _BM_CONCAT(FIXTURE, NAME)::List().push_back(bench);           \
            }                                                                 \
        } _initializer;                                                       \
                                                                              \
      protected:                                                              \
        void RunBenchmark(kwc::Context& context) override;                    \
    };                                                                        \
    _BM_CONCAT(FIXTURE, NAME)::_init _BM_CONCAT(FIXTURE, NAME)::_initializer; \
                                                                              \
    void _BM_CONCAT(FIXTURE, NAME)::RunBenchmark(kwc::Context& context)

}  // namespace utils
}  // namespace kwc

#define BENCHMARK_MAIN() \
    int main(int argc, const char* argv[]) { kwc::Benchmark::RunAllBenchmarks(argc, argv); }

#endif  // KWCTOOLKIT_UTILS_BENCHMARK_H_
