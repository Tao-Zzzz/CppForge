一个c++的测试和轮子集合


## 智能指针
shared_ptr, 构造, 赋值, 析构, 拷贝要加, 移动不加, get, reset, 
unique_ptr, 构造, 赋值, 析构, 无拷贝, 无计数,  get, reset,
weak_ptr, 构造, 赋值, 析构, 拷贝要加weak (引入了controlBlock), get, reset
要考虑多线程加锁
## IO复用
### select
fd集合, select阻塞监听, 遍历fd集, 获取事件, 或者获取server连接, 然后处理
用一个数组存放这些fd
select每次要重新将fd写入fd集, 因为select会将没有反应的那些fd去掉
<font color="#ff0000">所以每次while循环的时候会将那些fd用FD_SET重新设置一遍</font>
### poll
一个pollfd数组, 写入fd和事件, 用掩码运算想要得到的事件
比起select每一次都要将fd集清空, poll就不需要
**因为poll函数会清空每个pollfd的revents**
### epoll
创建epoll实例, 然后注册事件, 用epoll_wait获取事件队列, 然后进行处理即可, 要将新连接设置为非阻塞模式
###  [[select,poll,epoll]]
