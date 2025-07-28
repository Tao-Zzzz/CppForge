#ifndef __SESSION_H__
#define __SESSION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <queue>
#include <memory>
#include <mutex>
#include "global.h"

// 接受状态
enum RecvStage
{
    NO_RECV = 0, // 未接受
    HEAD_RECVING,
    BODY_RECVING,
};

// 发送状态
enum SendStage
{
    NO_SEND = 0,
    SENDING = 1
};

// 头部buffer
class HeadBuf
{
public:
    HeadBuf(size_t head_len);
    ~HeadBuf();
    // 缓存接受的头部信息
    char *_buf;
    // 头部总长度
    size_t _head_len;
    // 头部已经接受的偏移量
    size_t _offset;
};

// 数据buffer,用来存储接受或者发送的数据
class DataBuf
{
public:
    DataBuf(uint16_t type, size_t data_len);
    DataBuf(uint16_t type, std::string data, size_t data_len);
    ~DataBuf();
    // 数据类型id, 比如1001表示登录，1002表示聊天等。
    uint16_t _type;
    // 接受或发送缓存
    char *_buf;
    // 数据总长度
    size_t _data_len;
    // 已接受或发送的偏移量
    size_t _offset;
};

class IOThread;
class Session
{
public:
    friend class IOThread;
    Session(int fd, IOThread *pthread);
    ~Session();
    void Send(std::string data, int msg_type);

private:
    void enqueue_data(std::shared_ptr<DataBuf> data);
    //-1表示失败，0表示成功，1表示需要继续监听
    int send_data(std::shared_ptr<DataBuf> data);
    friend class IOThread;

private:
    // 文件描述符
    int _fd;
    // 存储接受的数据包体内容
    std::shared_ptr<DataBuf> _data_buf;
    // 存储未接受完整的头部
    std::shared_ptr<HeadBuf> _head_buf;

    // 接受阶段,0未开始后接受，1接受头部，2接受包体
    enum RecvStage _recv_stage;

    // 发送状态
    enum SendStage _send_stage;

    // 目前不用了，统一放在epoll所在线程发送，不存在线程安全问题
    std::mutex _send_mtx;
    // 发送队列
    std::queue<std::shared_ptr<DataBuf>> _send_que;
    IOThread *_p_ownerthread;
};

#endif