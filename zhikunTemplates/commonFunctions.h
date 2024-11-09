#ifndef _COMMONFUNCTIONS_H_
#define _COMMONFUNCTIONS_H_
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::duration;
using std::chrono::system_clock;
namespace commonFunctions{
    // 获取当前时间的字符串格式
    std::string getTimeString(std::chrono::system_clock::time_point time) {
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(time);
        std::tm now_tm = *std::localtime(&now_time_t);
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y_%m_%d_%H.%M.%S");
        return oss.str();
    }
}
#endif