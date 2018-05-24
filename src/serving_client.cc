/*************************************************************************
	> File Name: serving_client.cc
	> Author: wanghaiqiang
	> Mail: wanghq0130@163.com 
	> Created Time: 四  5/24 17:09:09 2018
	> Brief: 
 ************************************************************************/
/* Copyright 2017 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <fstream>
#include <iostream>
#include <array>

#include "grpc++/create_channel.h"
#include "grpc++/security/credentials.h"
#include "google/protobuf/map.h"
#include "google/protobuf/wrappers.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/example/example.pb.h"
#include "tensorflow/core/example/feature.pb.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/util/command_line_flags.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"
#include "tensorflow_serving/example/tensorflow_client_util_type.h"
#include "tensorflow_serving/example/serving_client.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::PredictRequest;
using tensorflow::serving::PredictResponse;
using tensorflow::serving::PredictionService;
using tensorflow::Example;

typedef google::protobuf::Map<tensorflow::string, tensorflow::TensorProto> OutMap;

ServingClient::ServingClient(std::shared_ptr<Channel> channel) {
    stub_ = PredictionService::NewStub(channel);
}
void ServingClient::callPredict(const tensorflow::string& model_name,
                                 const tensorflow::string& model_signature_name,
//                                 const int64 & version,
                                 const std::vector<std::vector<FeatureTuple>>& vector_features,
                                 std::vector<float> &result) {
    result.clear();
    Example example;
    unsigned int batchSize = vector_features.size();

    std::cout << "batch_size is : " << batchSize << std::endl;
    if(batchSize <= 0)
        return;
    PredictRequest predictRequest;
    PredictResponse response;
    ClientContext context;
    predictRequest.mutable_model_spec()->set_name(model_name);
    predictRequest.mutable_model_spec()->set_signature_name(
        model_signature_name);
    //predictRequet.mutable_model_spec()->set_allocated_version(model_name);
    //predictRequest.mutable_model_spec()->mutable_version()->set_value(version);

    google::protobuf::Map<tensorflow::string, tensorflow::TensorProto>& inputs =
        *predictRequest.mutable_inputs();

    // 可同时存放多个inputs
    tensorflow::TensorProto proto;


    //cpuTime.push_back(get_cpu_time());
    //wallTime.push_back(get_wall_time());
    tensorflow::string str_proto_example;
    proto.set_dtype(tensorflow::DataType::DT_STRING);
    for(const auto& features : vector_features) {
        for(const auto& feature : features) {
            if(feature.featureValueType == "0") {
                (*(example.mutable_features()->mutable_feature()))[feature.featureName].mutable_bytes_list()->add_value(feature.featureValue);
            } else if(feature.featureValueType == "1") {
                std::cout << "feature value type is : 1 " << std::endl;
                //(*(example.mutable_features()->mutable_feature()))[feature.featureName].mutable_float_list()->add_value(0, std::atof(feature.featureValue.c_str()));
                (*(example.mutable_features()->mutable_feature()))[feature.featureName].mutable_float_list()->add_value(std::atof(feature.featureValue.c_str()));
            } else {
                std::cout << "featureValueType must be 0 or 1: " << feature.featureName << " " << feature.featureValueType << " " <<feature.featureValue << std::endl;
                return;
            }
        }
        example.SerializeToString(&str_proto_example);
        //std::cout << "before add proto value" << std::endl;
        // 一条样本序列化后放到proto中
        proto.add_string_val(str_proto_example);
    }
    proto.mutable_tensor_shape()->add_dim()->set_size(batchSize);
    //inputs["examples"] = proto;
    inputs["inputs"] = proto;

    //cpuTime.push_back(get_cpu_time());
    //wallTime.push_back(get_wall_time());
    Status status = stub_->Predict(&context, predictRequest, &response);

    //delete[] image;

    if (status.ok()) {
        std::cout << "status ok" << std::endl;
    } else {
        std::cout << "status error: " << status.error_message() << std::endl;
    }
    if (status.ok()) {
      //std::cout << "call predict ok" << std::endl;
      //std::cout << "outputs size is " << response.outputs_size() << std::endl;
      OutMap& map_outputs = *response.mutable_outputs();
      for (auto& it : map_outputs) {
          std::cout << "output key : " << it.first << std::endl;
      }
      tensorflow::TensorProto& out_proto = (*response.mutable_outputs())["scores"];
      int size_float_score = out_proto.float_val_size();
      std::cout << "tensor output size: " << size_float_score << std::endl;
      //std::cout << "result array size is " << size_float_score<< std::endl;
      result.resize(size_float_score);
      for(int i = 0; i< size_float_score; i++)
      {
        result[i] = out_proto.float_val(i);
        std::cout << "score index : " << i << " --> " << result[i] << std::endl;
      }
      return;

    } else {
      return ;
    }
}
