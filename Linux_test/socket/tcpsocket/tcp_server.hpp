#pragma once
#include<functional>
#include"tcp_socket.hpp"

typedef std::function<void (const string& req,string* resp)> Handler;

class TcpServer{
public:
  TcpServer(const string& ip,uint16_t port)
  :ip_(ip),port_(port)  {}
  
  bool Start(Handler handler){
    //创建socket
    CHECK_RET(listen_sock_.Socket());
    //绑定端口号
    CHECK_RET(listen_sock_.Bind(ip_,port_));
    //进行监听
    CHECK_RET(listen_sock_.Listen(5));
    //进入事件循环
    for(;;){
      //Accept
      Tcp_Socket new_sock;
      string ip;
      uint16_t port = 0;
      if(!listen_sock_.Accept(&new_sock,&ip,&port)){
        continue;
      }
      cout << "Client:"  << ip.c_str() << port << "connect!" << endl;
      //读写循环
      for(;;){
        string req;
        //读取请求
        bool ret = new_sock.Recv(&req);
        if(!ret){
          cout << "Client" << ip.c_str() << port << "disconnect!" << endl;
          new_sock.Close();
          break;
       }
        //计算响应
        string resp;
        handler(req,&resp);
        //写回响应
        new_sock.Send(resp);
        cout << "[" << ip.c_str() << ":" << port << "req" << ":" << req.c_str() << ","  << "resp" << resp.c_str() << endl; 
      }
    }
    return true;
  }

private:
  Tcp_Socket listen_sock_;
  string ip_;
  uint64_t port_;
};

