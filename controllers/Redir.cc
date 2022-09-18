#include "Redir.h"

void Redir::asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    if (!req->session()->find("session-name") || !req->session()->find("session-pass"))
    {
        auto resp = drogon::HttpResponse::newRedirectionResponse("/reg");
        callback(resp);
        return;
    }

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody(req->session()->get<std::string>("session-name"));
    callback(resp);
}