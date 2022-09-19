#include "Reg.h"

void Reg::asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto clientPtr = drogon::app().getDbClient("postgres");

    if (!req->session()->find("session-name") || !req->session()->find("session-pass"))
    {
        try
        {
            if (req->getParameter("name").empty() || req->getParameter("pass").empty())
            {
                std::string res = "";
                res.append("<form style='display:flex;'>Name: <input type='text' name='name'> \
                Password: <input type='text' name='pass'> \
                <input type='submit' formmethod='post' value='Register|Log in'></form>");
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k200OK);
                resp->setContentTypeCode(CT_TEXT_HTML);
                resp->setBody(res);
                callback(resp);
                return;
            }
            else
            {
                auto pass = std::to_string(std::hash<std::string>{}(std::string(req->getParameter("pass").data())));
                auto name = std::string(req->getParameter("name").data());

                std::cout << req->getCreationDate().toFormattedStringLocal(false) << " " << name << " " << pass << std::endl;

                auto check = clientPtr->execSqlSync("select * from users where user_name=$1", name);

                if (check.size() == 0)
                {
                    auto result = clientPtr->execSqlSync("insert into users(user_role, user_status, user_name, user_token) \
                     values (2, 1, $1, $2)", name, pass);
                }
                else
                {
                    bool matcher = false;

                    for (auto row : check)
                    {
                        if (pass == row["user_token"].as<std::string>())
                        {
                            matcher = true;
                            break;
                        }
                    }

                    if (!matcher)
                    {
                        auto resp = drogon::HttpResponse::newRedirectionResponse("/reg");
                        req->setParameter("name", "");
                        req->setParameter("pass", "");
                        callback(resp);
                        return;
                    }
                }

                req->session()->insert(std::string("session-name"), name);
                req->session()->insert(std::string("session-pass"), pass);
            }
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            std::cerr << "error:" << e.base().what() << std::endl;
        }
    }

    auto resp = drogon::HttpResponse::newRedirectionResponse("/");
    callback(resp);
}
