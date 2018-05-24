/*************************************************************************
	> File Name: tensorflow_client_util.cc
	> Author: wanghaiqiang
	> Mail: wanghq0130@163.com 
	> Created Time: 四  5/24 17:10:54 2018
	> Brief: 
 ************************************************************************/

#include "tensorflow_client_util.h"
#include "serving_client.h"


TensorflowClientUtil::TensorflowClientUtil(
                       const std::string& server_port,
                       const std::string& model_name,
                       const std::string& model_signature_name)
          //             const int64& version)
{
        this->server_port = server_port;
        this->model_name = model_name;
        this->model_signature_name = model_signature_name;
        //this->version = version;
}


//template <long unsigned int DimsFirst, long unsigned int DimsSecond>
void TensorflowClientUtil::callPredict(const std::vector<std::vector<FeatureTuple>>& vector_features, std::vector<float> &result){
    ServingClient guide(
        grpc::CreateChannel(server_port, grpc::InsecureChannelCredentials()));
    guide.callPredict(model_name, model_signature_name,/* version,*/ vector_features, result);
    return;
}
