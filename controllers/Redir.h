#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class Redir : public drogon::HttpSimpleController<Redir>
{
  public:
    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    // list path definitions here;
    PATH_ADD("/",Get,Post);
    PATH_LIST_END
};
