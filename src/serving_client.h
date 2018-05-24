/*************************************************************************
	> File Name: serving_client.h
	> Author: wanghaiqiang
	> Mail: wanghq0130@163.com 
	> Created Time: 四  5/24 17:08:15 2018
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
#ifndef TENSORFLOW_SERVING_SERVING_CLIENT_H_
#define TENSORFLOW_SERVING_SERVING_CLIENT_H_

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


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::PredictRequest;
using tensorflow::serving::PredictResponse;
using tensorflow::serving::PredictionService;
using tensorflow::Example;

typedef google::protobuf::Map<tensorflow::string, tensorflow::TensorProto> OutMap;



class ServingClient {
    public:
        ServingClient(std::shared_ptr<Channel> channel);
        void callPredict(const tensorflow::string& model_name,
                const tensorflow::string& model_signature_name,
                const std::vector<std::vector<FeatureTuple>>& vector_features,
                std::vector<float> &result);
    private:
        std::unique_ptr<PredictionService::Stub> stub_;
};

#endif
