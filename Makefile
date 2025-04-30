# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Werror -I../mongoose -I/usr/local/include/cjson
LDFLAGS = -L/home/fleam/cJSON/build -lcjson -Wl,-rpath=/home/fleam/cJSON/build -lmysqlclient

# 源文件和目标文件
SRCS = mongoose_server.c \
       mongoose_event.c \
       handler/time_handler.c \
       handler/directory_handler.c \
       handler/unsupported_handler.c \
       handler/cpi_handler.c \
       lib/mysql_connection_pool.c \
       lib/mysql_config.c \
       ../mongoose/mongoose.c
OBJS = $(SRCS:.c=.o)

# 可执行文件名称
TARGET = server

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# 编译源文件生成目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)