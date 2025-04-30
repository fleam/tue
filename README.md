# 使用纯 C 开发 RESTful API

## cJSON（20250430接近v1.7.18）

```
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build
cd build
cmake ..
make
make install
```

## mongoose（接近7.17）

```
git clone https://github.com/cesanta/mongoose.git
```

## mysqlclient 8.0.4

```
sudo apt-get update
sudo apt-get install libmysqlclient-dev
mysql_config --version
```

## wsl宿主机地址

```
ip route show | grep -i default | awk '{ print $3 }'
```
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
