#ifndef __WILL_SOCKET_H__
#define __WILL_SOCKET_H__

#include <memory>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "address.h"
#include "noncopyable.h"

namespace will {

class Socket : public std::enable_shared_from_this<Socket>, Noncopyable {
public:
    typedef std::shared_ptr<Socket> ptr;
    typedef std::weak_ptr<Socket> weak_ptr;

    enum Type {
        // TCP类型
        TCP = SOCK_STREAM,
        // UDP类型
        UDP = SOCK_DGRAM
    };

    enum Family {
        // IPv4 socket
        IPv4 = AF_INET,
        // IPv6 socket
        IPv6 = AF_INET6,
        // Unix socket
        UNIX = AF_UNIX,
    };

    static Socket::ptr CreateTCP(will::Address::ptr address);

    static Socket::ptr CreateUDP(will::Address::ptr address);

    static Socket::ptr CreateTCPSocket();

    static Socket::ptr CreateUDPSocket();

    static Socket::ptr CreateTCPSocket6();

    static Socket::ptr CreateUDPSocket6();

    static Socket::ptr CreateUnixTCPSocket();

    static Socket::ptr CreateUnixUDPSocket();

    Socket(int family, int type, int protocol = 0);

    virtual ~Socket();

    int64_t getSendTimeout();

    void setSendTimeout(int64_t v);

    int64_t getRecvTimeout();

    void setRecvTimeout(int64_t v);

    bool getOption(int level, int option, void *result, socklen_t *len);

    template <class T>
    bool getOption(int level, int option, T &result) {
        socklen_t length = sizeof(T);
        return getOption(level, option, &result, &length);
    }

    bool setOption(int level, int option, const void *result, socklen_t len);

    template <class T>
    bool setOption(int level, int option, const T &value) {
        return setOption(level, option, &value, sizeof(T));
    }

    // Socket必须 bind , listen  成功
    virtual Socket::ptr accept();

    virtual bool bind(const Address::ptr addr);

    virtual bool connect(const Address::ptr addr, uint64_t timeout_ms = -1);

    virtual bool reconnect(uint64_t timeout_ms = -1);

    virtual bool listen(int backlog = SOMAXCONN);

    virtual bool close();
    
    // retval >0 发送成功对应大小的数据
    //        =0 socket被关闭
    //        <0 socket出错
    virtual int send(const void *buffer, size_t length, int flags = 0);

    virtual int send(const iovec *buffers, size_t length, int flags = 0);

    virtual int sendTo(const void *buffer, size_t length, const Address::ptr to, int flags = 0);

    virtual int sendTo(const iovec *buffers, size_t length, const Address::ptr to, int flags = 0);

    virtual int recv(void *buffer, size_t length, int flags = 0);

    virtual int recv(iovec *buffers, size_t length, int flags = 0);

    virtual int recvFrom(void *buffer, size_t length, Address::ptr from, int flags = 0);

    virtual int recvFrom(iovec *buffers, size_t length, Address::ptr from, int flags = 0);

    Address::ptr getRemoteAddress();

    Address::ptr getLocalAddress();

    int getFamily() const { return m_family; }

    int getType() const { return m_type; }

    int getProtocol() const { return m_protocol; }

    bool isConnected() const { return m_isConnected; }

    bool isValid() const;

    int getError();

    virtual std::ostream &dump(std::ostream &os) const;

    virtual std::string toString() const;

    int getSocket() const { return m_sock; }

    bool cancelRead();

    bool cancelWrite();

    bool cancelAccept();

    bool cancelAll();

protected:

    void initSock();

    void newSock();

    virtual bool init(int sock);

protected:
    // socket句柄
    int m_sock;
    // 协议簇
    int m_family;
    // 类型
    int m_type;
    // 协议
    int m_protocol;
    // 是否连接
    bool m_isConnected;
    // 本地地址
    Address::ptr m_localAddress;
    // 远端地址
    Address::ptr m_remoteAddress;
};

std::ostream &operator<<(std::ostream &os, const Socket &sock);

} // namespace will

#endif
