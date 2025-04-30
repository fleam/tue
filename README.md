# 使用纯 C 开发 RESTful API

## 环境

### 系统 Ubuntu_2004.2021.825.0_x64 WSL

```
sudo apt-get update
sudo apt install build-essential cmake git net-tools 
```

### cJSON（20250430下载master接近v1.7.18）

```
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build
cd build
cmake ..
make
make install
```

### mongoose（20250430下载master接近7.17）

```
git clone https://github.com/cesanta/mongoose.git
```

### mysqlclient 8.0.4

```
sudo apt-get install libmysqlclient-dev
mysql_config --version
```

### wsl宿主机地址

```
ip route show | grep -i default | awk '{ print $3 }'
```

### 目录结构
```
/home/fleam/
├── cJSON/
│   └── ...
├── mongoose/
│   └── ...
├── tue/
│   └── ...
```

## todo

1. support debug
2. log