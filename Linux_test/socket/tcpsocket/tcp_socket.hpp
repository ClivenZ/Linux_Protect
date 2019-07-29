#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<cassert>
#include<sys/socket.h>
#include<error.h>
#include<bits/socket.h>
#include<sys/un.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string>
#include<arpa/inet.h>
#include<fcntl.h>

using namespace std;

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

#define CHECK_RET(exp){ if(!(exp))return false;}

class Tcp_Socket{
  public:
    Tcp_Socket():
      fd_(-1){  }
    Tcp_Socket(int fd):fd_(fd){ }
    //创建套接字
    bool Socket(){
      fd_=socket(AF_INET,SOCK_STREAM,0);
      if(fd_ < 0){
        perror("socket");
        return false;
      }
      cout << fd_ << endl;
      return true;
    }
    //命名套接字
    bool Bind(const string& ip,uint16_t port) const{
      sockaddr_in addr; 
      //IPv4 地址族
      addr.sin_family = AF_INET;
      //IPv4地址结构体
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      //端口号，网络字节序
      addr.sin_port = htons(port);

      int ret = bind(fd_,(sockaddr*)&addr,sizeof(addr));
      if(ret < 0){
        perror("bind");
        return false;
      }
      return true;
    }
    //监听socket
    bool Listen(int num) const{
      int ret = listen(fd_,num);
      if(ret < 0){
        perror("listen");
        return false;
      }
      return true;
    }
    //接收连接
    bool Accept(Tcp_Socket* peer,string* ip = NULL,uint16_t* port = NULL) const{
      sockaddr_in peer_addr;
      socklen_t len = sizeof(peer_addr);
      int new_sock = accept(fd_,(sockaddr*)&peer_addr,&len);
      if(new_sock < 0){
        perror("accept");
        return false;
      } 
      return true;
    }
    //发起连接
    bool Connect(const string &ip,uint16_t port) const{
      sockaddr_in addr;
      //IPv4
      addr.sin_family = AF_INET;
      //端口号
      addr.sin_port = htons(port);
      //IPv4结构体
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = connect(fd_,(sockaddr*)&addr,sizeof(addr));
      if(ret < 0){
        perror("connect");
        return false;
      }
      return true;
    }
    //发送数据
    bool Send(const string &buf) const{
      ssize_t write_size = send(fd_,buf.data(),buf.size(),0);
      if(write_size < 0){
        perror("send");
        return false;
      }
      return true;
    }
    //接收数据
    bool Recv(string* buf) const{
      buf->clear();
      char tmp[1024 * 10] = {0};
      //ssize_t revc(int sockfd,void* buf,size_t len,int flags);
      //返回收到的字节数
      ssize_t read_size = recv(fd_,tmp,sizeof(tmp),0);
      if(read_size < 0){
        perror("recv");
        return false;
      }
      if(read_size == 0){
        return false;
      }
      buf->assign(tmp,read_size);
      return true;
    }
    //关闭
    bool Close() const{
      close(fd_);
      cout << fd_ << endl;
      return true;
    }
    int GetFd() const{
      return fd_;
    }
  private:
    int fd_;
};
