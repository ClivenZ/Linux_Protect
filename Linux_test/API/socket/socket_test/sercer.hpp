#pragma once
#include "socket.hpp"

#include<functional>
typedef std::function<void (const std::string&,std::string* resp)> Handler;

class UdpServer{
  public:
    UdpServer(){
      assert(sock_.Socket());
    }
    ~UdpServer()
    {
      sock_.Close();
    }
    bool Start(const std::string& ip,uint16_t port,Handler handler)
    {
      bool ret = sock_.Bind(ip,port);
      if(!ret){
        return false;
      }
      for(;;){
        std::string req;
        std::string remote_ip;
        uint16_t remote_port = 0;
        bool ret = sock_.RecvFrom(&req,&remote_ip,&remote_port);
        
        if(!ret){
          continue;
        }
        std::string resp;
        handler(req,&resp);
        sock_.SendTo(resp,remote_ip,remote_port);
        printf("[%s:%d] req: %s,resp: %s\n",remote_ip.c_str(),remote_port,
            req.c_str(),resp.c_str());
      }
      sock_.Close();
      return true;
    }
  private:
    UdpSocket sock_;
};
