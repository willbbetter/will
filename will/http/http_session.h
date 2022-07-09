#ifndef __HTTP_SESSION_H__
#define __HTTP_SESSION_H__

#include "../socket_stream.h"
#include "http.h"
#include "../log.h"

namespace will {
namespace http{
class HttpSession : public SocketStream {
public:
    typedef std::shared_ptr<HttpSession> ptr;
    HttpSession(Socket::ptr sock, bool owner = true);
    HttpRequest::ptr recvRequest();
    int sendResponse(HttpResponse::ptr rsp);

};


}
}

#endif
