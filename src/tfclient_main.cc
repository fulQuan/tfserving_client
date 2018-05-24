/*************************************************************************
	> File Name: tfclient_main.cc
	> Author: wanghaiqiang
	> Mail: wanghq0130@163.com
	> Created Time: 四  5/17 11:19:28 2018
	> Brief:
 ************************************************************************/

#include <iostream>
#include <vector>
#include "tensorflow_client_util.h"
#include "tensorflow_client_util_type.h"


//std::vector<FeatureTuple> gSample() {
void gSample(std::vector<FeatureTuple>* sample) {
    std::string col_names[4] = {"account_id","adplan_id","scheduling_id", "xxxd"};
    for (auto& col_name: col_names) {
        FeatureTuple feat;
        feat.featureName = col_name;
        feat.featureValueType = "0";
        feat.featureValue = "2";
        sample->push_back(feat);
        //std::cout << feat << std::endl;
    }
    return;
}



int main() {

    std::vector<FeatureTuple>* sample = new std::vector<FeatureTuple>();
    gSample(sample);
    auto it = sample->begin();
    for (;it != sample->end(); ++it) {
        std::cout << it->featureName << std::endl;
    }
    std::vector<FeatureTuple>* sample2 = new std::vector<FeatureTuple>();
    gSample(sample2);
    //一条样本生成done
    std::vector<std::vector<FeatureTuple>> samples;
    samples.push_back(*sample);
    samples.push_back(*sample2);
    std::string sp = "localhost:9111";
    std::string name = "wide";
    std::string signame = "serving_default";
    TensorflowClientUtil* tf = new TensorflowClientUtil(sp, name, signame);
    std::vector<float> result;
    tf->callPredict(samples, result);
    return 0;
}
