/*************************************************************************
	> File Name: tensorflow_client_util.h
	> Author: wanghaiqiang
	> Mail: wanghq0130@163.com 
	> Created Time: 四  5/24 17:10:07 2018
	> Brief: 
 ************************************************************************/
#ifndef TENSORFLOW_CLIENT_UTIL_H_
#define TENSORFLOW_CLIENT_UTIL_H_

#include <string>
#include <array>
#include <vector>
#include "tensorflow_client_util_type.h"

class TensorflowClientUtil{
public:
  TensorflowClientUtil(const std::string& server_port,
                       const std::string& model_name,
                       const std::string& model_signature_name);
//                       const int64& version);

  void callPredict(const std::vector<std::vector<FeatureTuple>>& vector_features, std::vector<float> &result);
  //template <long unsigned int DimsFirst, long unsigned int DimsSecond>
  //void callPredict(const std::array<std::array<FeatureTuple,DimsSecond>,DimsFirst>& feature_tuple_array, std::vector<float> &result);
//  {
//    ServingClient guide(
//        grpc::CreateChannel(server_port, grpc::InsecureChannelCredentials()));
//    //std::cout << "calling predict using file: " << image_file << "  ..."
//    //          << std::endl;
//    guide.callPredict(model_name, model_signature_name, feature_tuple_array, result);
//    return;
//  }

private:
   std::string server_port;
   std::string model_name;
   std::string model_signature_name;
   //int64 version;

};
#endif

