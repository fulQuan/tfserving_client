/*************************************************************************
	> File Name: tensorflow_client_util_type.h
	> Author: wanghaiqiang
	> Mail: wanghq0130@163.com 
	> Created Time: 四  5/24 17:11:43 2018
	> Brief: 
 ************************************************************************/

#ifndef TENSORFLOW_CLIENT_UTIL_TYPE_H_
#define TENSORFLOW_CLIENT_UTIL_TYPE_H_
#include <string>
#include <time.h>
#include <sys/time.h>

//double get_wall_time(){
//    struct timeval time;
//    if (gettimeofday(&time,NULL)){
//        //  Handle error
//        return 0;
//    }
//    return (double)time.tv_sec + (double)time.tv_usec * .000001;
//}
//double get_cpu_time(){
//    return (double)clock() / CLOCKS_PER_SEC;
//}

struct FeatureTuple {
  public:
      FeatureTuple() {
      }
      FeatureTuple(const std::string& featureName, const std::string& featureValueType,
              const std::string& featureValue) {
          this->featureName = featureName;
          this->featureValueType = featureValueType;
          this->featureValue = featureValue;
      }
      ~FeatureTuple() {
      }
      std::string featureName;
      std::string featureValueType; //0 means string featureValue, 1 means float featureValue
      std::string featureValue;
};
#endif

//typedef long signed int int64;
