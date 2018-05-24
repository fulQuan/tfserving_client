## TensorFlowServing 客户端工程开发使用
--- 
### 简介
&ensp;&ensp;&ensp;&ensp;tensorflow用于离线模型训练, 在线预估时, 往往有两种方式, 一种是将模型存放成明文, 在服务端手动加载解析; 另一种是使用tensorflow_serving,tensorflow_serving 支持不同版本模型的加载, 但是在客户端调用的时候需要手写, 由于客户端包含了tensorflow+serving的很多依赖文件, 所有需要将客户端的调用依赖, 编译打包成.so, 在自己的工程中进行加载, 即可使用.

### 组成
``` bash 
#用于使用tensorflow_serving的callPredict
serving_client.h
seving_client.cc

# 构建client_server配置信息 ip:port, model_name, signature_name, 并调用serving_client.h 中的callPredict函数  
tensorflow_client_util.h
tensorflow_client_util.cc

# 包裹输入样本的数据结构
tensorflow_client_util_type.h  

#客户端demo, 构造样本,调用服务函数, 返回打分score
tfclient_main.cc
```
### 使用
``` bash 
# 使用打包好的libtensorflow_client_util.so 对客户端进行编译即可
#客户端的逻辑可自行定义
g++ tfclient_main.cc -I./ -L./ -ltensorflow_client_util  
```
### .so文件生成
&ensp;&ensp;&ensp;&ensp;由于客户端请求tensorflow_serving的Predict函数, 依赖于 tensorflow, tensorflow_serving, grpc等一系列依赖文件, 所有使用bazel 将所有依赖进行打包.
&ensp;&ensp;&ensp;&ensp; 安装好tensorflow以及tensorflow_serving后, 在 tensorflow_serving/example/BUILD目录下加入如下编译选项:
``` bash 
cc_library(
    name = "tensorflow_client_util_type",
    srcs = ["tensorflow_client_util_type.h"],
    visibility = ["//visibility:public"],
)

cc_library (
    name = "serving_client",
    srcs = ["serving_client.cc"],
    hdrs = ["serving_client.h"],
    deps = [
        "//tensorflow_serving/apis:prediction_service_proto",
        "@grpc//:grpc++_unsecure",
        "@org_tensorflow//tensorflow/core:framework",
        "@protobuf_archive//:protobuf_lite",
        ":tensorflow_client_util_type",
    ],
    visibility = ["//visibility:public"],
)
cc_library(
    name = "tensorflow_client_util",
    srcs = ["tensorflow_client_util.cc"],
    hdrs = ["tensorflow_client_util.h"],
    deps = [
        ":serving_client",
        ":tensorflow_client_util_type",
        "//tensorflow_serving/apis:prediction_service_proto",
        "@grpc//:grpc++_unsecure",
        "@org_tensorflow//tensorflow/core:framework",
        "@protobuf_archive//:protobuf_lite",
    ],
    visibility = ["//visibility:public"],
)



cc_binary(
    name = "tfclient_main",
    srcs = ["tfclient_main.cc"],
    deps = [
        ":tensorflow_client_util",
    ],
)
#使用bazel打包生成客户端, 可忽略, bazel编写客户端调试时使用
cc_binary(
    name = "tensorflow_client_util_test",
    srcs = ["tensorflow_client_util_test.cc"],
    deps = [
        ":tensorflow_client_util",
        ":tensorflow_client_util_type",
    ],
)
cc_library(
    name = "tensorflow_client_util_lib.so",
    srcs = ["tensorflow_client_util.cc"],
    hdrs = ["tensorflow_client_util.h"],
    linkstatic=1,
    deps = [
        ":serving_client",
        ":tensorflow_client_util_type",
        "@org_tensorflow//tensorflow/core:framework",

    ],
    visibility = ["//visibility:public"],
)

# 将依赖文件打包成动态库, 最终的目标文件,用于最终的服务端使用
cc_binary(
    name = "libtfutil.so",
    linkshared = 1,
    deps = [
        ":tensorflow_client_util",
        ":tensorflow_client_util_type",
    ],
)
```
使用 `bazel build //tensorflow_serving/example:libtfutil.so` 即可生成 **libtfutil.so** 从来用于客户端gcc编译时使用.
