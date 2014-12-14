#include "simpleLogger.h"

int main() {
  LOG_TRACE << "this is a trace message";
  LOG_DEBUG << "this is a debug message";
  LOG_WARNING << "this is a warning message";
  LOG_ERROR << "this is an error message";
  LOG_FATAL << "this is a fatal error message";
  return 0;
}