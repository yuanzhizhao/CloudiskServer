# 编译器
CC = gcc
# 编译选项
CFLAGS = -Wall -g -I../server/

# 获取所有 .c 源文件
SRCS = $(wildcard *.c)
# 根据源文件生成对应的可执行文件列表
EXECS = $(SRCS:.c=)
OBJS = $(SRCS:.c=.o)

# LIB
EXCLUDE = ../server/main.o
LIBS_ALL = $(wildcard ../server/*.o)
LIBS = $(filter-out $(EXCLUDE), $(LIBS_ALL))

# 默认目标：生成所有可执行文件
all: $(EXECS)
$(EXECS): $(OBJS)

# 规则：根据 .c 文件生成可执行文件
$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%: %.o $(LIBS)
	$(CC) $< $(LIBS) -o $@


# 清理规则：删除所有生成的可执行文件
clean:
	rm -f $(EXECS) $(OBJS)
