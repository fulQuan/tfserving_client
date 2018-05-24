## TensorFlowServing 客户端工程开发使用
--- 
### 简介
tensorflow用于离线模型训练, 在线预估时, 往往有两种方式, 一种是将模型存放成明文, 在服务端手动加载解析; 另一种是使用tensorflow_serving,tensorflow_serving 支持不同版本模型的加载, 但是在客户端调用的时候需要手写, 由于客户端包含了tensorflow+serving的很多依赖文件, 所有需要将客户端的调用依赖, 编译打包成.so, 在自己的工程中进行加载, 即可使用.

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

