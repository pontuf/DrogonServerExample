#include "TestCtrl.h"

void TestCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
  auto a = req->getParameter("a");
  auto b = req->getParameter("b");

  if (!req->session()->find("session-name") || !req->session()->find("session-pass"))
  {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(HttpStatusCode::k400BadRequest);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody("Сначала войди в свой аккаунт!");
    callback(resp);
    return;
  }

  std::string res = "";

  res.append("<form style='display:flex;'>A: <input type='text' name='a'>B: <input type='text' name='b'>");
  res.append("<input type='submit' formmethod='post' value='Готово'></form>");

  static const std::regex r("^-?\\d+((\\,|\\.)\\d+)?$");

  if (regex_match(a.data(), r) && regex_match(b.data(), r))
  {
    std::replace(a.begin(), a.end(), ',', '.');
    std::replace(b.begin(), b.end(), ',', '.');
    long double mul1 = std::stold(a);
    long double mul2 = std::stold(b);

    res.append(std::to_string(mul1 * mul2));
  }

  auto resp = HttpResponse::newHttpResponse();
  resp->setStatusCode(k200OK);
  resp->setContentTypeCode(CT_TEXT_HTML);

  resp->setBody(res);
  callback(resp);
}
