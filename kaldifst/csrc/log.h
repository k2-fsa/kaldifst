// kaldifst/csrc/log.h
//
// Copyright (c)  2020  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_CSRC_LOG_H_
#define KALDIFST_CSRC_LOG_H_
#include <iostream>
#include <sstream>

namespace kaldifst {

enum class LogLevel {
  kInfo = 0,
  kWarn = 1,
  kError = 2,  // abort the program
};

class Logger {
 public:
  Logger(const char *filename, const char *func_name, uint32_t line_num,
         LogLevel level)
      : filename_(filename),
        func_name_(func_name),
        line_num_(line_num),
        level_(level) {
    os_ << filename << ":" << func_name << ":" << line_num << "\n";
    switch (level_) {
      case LogLevel::kInfo:
        os_ << "[I] ";
        break;
      case LogLevel::kWarn:
        os_ << "[W] ";
        break;
      case LogLevel::kError:
        os_ << "[E] ";
        break;
    }
  }

  template <typename T>
  Logger &operator<<(const T &val) {
    os_ << val;
    return *this;
  }

  ~Logger() {
    std::cerr << os_.str() << "\n";
    if (level_ == LogLevel::kError) abort();
  }

 private:
  std::ostringstream os_;
  const char *filename_;
  const char *func_name_;
  uint32_t line_num_;
  LogLevel level_;
};

class Voidifier {
 public:
  void operator&(const Logger &)const {}
};

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__) || \
    defined(__PRETTY_FUNCTION__)
// for clang and GCC
#define KALDIFST_FUNC __PRETTY_FUNCTION__
#else
// for other compilers
#define KALDIFST_FUNC __func__
#endif

#define KALDIFST_LOG \
  kaldifst::Logger(__FILE__, KALDIFST_FUNC, __LINE__, kaldifst::LogLevel::kInfo)

#define KALDIFST_WARN \
  kaldifst::Logger(__FILE__, KALDIFST_FUNC, __LINE__, kaldifst::LogLevel::kWarn)

#define KALDIFST_ERR                                  \
  kaldifst::Logger(__FILE__, KALDIFST_FUNC, __LINE__, \
                   kaldifst::LogLevel::kError)

#define KALDIFST_ASSERT(x)                                        \
  (x) ? (void)0                                                   \
      : kaldifst::Voidifier() & KALDIFST_ERR << "Check failed!\n" \
                                             << "x: " << #x

#define KALDIFST_PARANOID_ASSERT KALDIFST_ASSERT

#define KALDIFST_DISALLOW_COPY_AND_ASSIGN(Class) \
 public:                                         \
  Class(const Class &) = delete;                 \
  Class &operator=(const Class &) = delete;

using MatrixIndexT = int32_t;

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_LOG_H_
