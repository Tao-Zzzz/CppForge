#ifndef _IO_THREAD_H__
#define _IO_THREAD_H__

#include <queue>
#include <memory>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <string>
#include <unordered_map>

enum class TaskType
{
    RegisterConn,
    SendData,
    Shutdown
};

class IOTask
{
public:
    IOTask(int fd, TaskType type, std::string data = "", int msgtype = 0) : _fd(fd),
                                                                            _type(type), _data(data), _msgtype(msgtype) {}
    ~IOTask() = default;
    TaskType _type;
    int _fd;
    // 以下字段仅发送时生效
    std::string _data;
    int _msgtype;
};

class NoneCopy
{
protected:
    NoneCopy() = default;

private:
    NoneCopy(const NoneCopy &cp) = delete;
    NoneCopy &operator=(const NoneCopy &) = delete;
};

class Session;
class IOThread : public NoneCopy
{
public:
    IOThread(int index);
    ~IOThread();
    int set_nonblocking(int fd);
    void enqueue_new_con(int fd);
    void catche_new_con(int fd);
    void start();
    void wakeup();
    void stop();
    void join();
    void enqueue_task(std::shared_ptr<IOTask> task);
    void enqueue_send_data(int fd, const std::string &msg, int msgtype);
    void loop();

private:
    bool deal_enque_tasks();
    bool add_fd(int fd, int events);
    bool mod_fd(int fd, int events);
    bool del_fd(int fd);
    void clear_fd(int fd);
    int read_head_data(std::shared_ptr<Session> sess);
    int read_body_data(std::shared_ptr<Session> sess);
    void handle_epollout(std::shared_ptr<Session> sess);
    int _event_fd;
    int _epoll_fd;
    std::mutex _task_mtx;
    std::queue<std::shared_ptr<IOTask>> _tasks;
    struct epoll_event *_event_addr;
    int _event_count;
    std::thread _thread;
    std::atomic<bool> _stop;
    int _index;
    std::unordered_map<int, std::shared_ptr<Session>> _sessions;
    bool _expanded_once;
};

#endif