#pragma once

#include <systemd/sd-journal.h>

#include <app.hpp>
#include <async_resp.hpp>
#include <error_messages.hpp>

#include <string_view>

namespace redfish
{

namespace query_param
{

void excuteQueryParamAll(crow::App& app, const crow::Request& req,
                         const std::shared_ptr<bmcweb::AsyncResp>& aResp)
{
    /* crow::Response& res */
    BMCWEB_LOG_DEBUG << "here All excuteQueryParam";
    int queryAccount = 0;
    boost::urls::url_view::params_type::iterator itOnly =
        req.urlParams.find("only");
    // boost::urls::url_view::params_type::iterator itSelect =
    //     req.urlParams.find("$select");
    // boost::urls::url_view::params_type::iterator itExpand =
    //     req.urlParams.find("$expand");
    boost::urls::url_view::params_type::iterator itSkip =
        req.urlParams.find("skip");
    // boost::urls::url_view::params_type::iterator itTop =
    //     req.urlParams.find("top");

    // std::vector<QueryParamType> paramVector;

    for (const auto param : req.urlParams)
    {
        std::cerr << "param->key()= " << param->key() << std::endl;
        std::cerr << "param->value()= " << param->value() << std::endl;
    }
    std::cerr << "req.urlParams.count(skip)？= " << req.urlParams.count("skip")
              << std::endl;
    BMCWEB_LOG_DEBUG << "req.urlParams[skip]？= " << req.urlParams["skip"];
    std::cerr << "req.urlParams.count(only)？= " << req.urlParams.count("only")
              << std::endl;
    BMCWEB_LOG_DEBUG << "req.urlParams[only]？= " << req.urlParams["only"];
    // std::string paramList;
    // for (const auto param : req.urlParams)
    // {
    //     paramList += param->key() + " " + param->value() + " ";
    // }
    // std::cerr << "paramList = " << paramList << std::endl;

    // if (itSelect != req.urlParams.end())
    // {
    //     std::cerr << "here select param. xiaochao" << std::endl;
    //     paramVector.push_back(QueryParamType::select);

    //     std::cerr << "paramVector size = " << paramVector.size() <<
    //     std::endl;
    // }
    // if (itExpand != req.urlParams.end())
    // {
    //     std::cerr << "here expand param. xiaochao " << std::endl;
    //     paramVector.push_back(QueryParamType::expand);

    //     std::cerr << "paramVector size = " << paramVector.size() <<
    //     std::endl;
    // }
    if (itSkip != req.urlParams.end())
    {
        std::cerr << "here skip param. xiaochao " << std::endl;
        // paramVector.push_back(QueryParamType::skip);
        queryAccount += 1;

        std::cerr << "queryAccount size = " << queryAccount << std::endl;
    }
    // if (itTop != req.urlParams.end())
    // {
    //     std::cerr << "here top param. xiaochao " << std::endl;
    //     paramVector.push_back(QueryParamType::top);
    //     queryAccount += 1;

    //     std::cerr << "paramVector size = " << paramVector.size() <<
    //     std::endl;
    // }
    if (itOnly != req.urlParams.end())
    {
        std::cerr << "here has only .xiaochao" << std::endl;
        // paramVector.push_back(QueryParamType::only);
        queryAccount += 1;

        if (queryAccount > 1)
        {
            std::cerr << "too many param. xiaochao " << std::endl;

            aResp->res.clear();
            messages::queryCombinationInvalid(aResp->res);
            return;
        }

        nlohmann::json::iterator itMemberCount =
            aResp->res.jsonValue.find("Members@odata.count");
        if (itMemberCount != aResp->res.jsonValue.end())
        {
            std::cerr << "here find membercounts .xiaochao" << std::endl;
            if (aResp->res.jsonValue["Members@odata.count"] == 1 &&
                aResp->res.jsonValue.find("Members") !=
                    aResp->res.jsonValue.end())
            {
                std::cerr << "find members .xiaochao" << std::endl;
                nlohmann::json& membersT = aResp->res.jsonValue["Members"];
                if (membersT.size() != 1)
                {
                    std::cerr << "Members' count isn't suit its description."
                              << std::endl;
                    return;
                }

                if (membersT[0].find("@odata.id") != membersT[0].end())
                {
                    std::string url = membersT[0]["@odata.id"];
                    // aResp->res.clear();
                    std::cerr << "do route. xiaochao " << std::endl;

                    // aResp->res.result(boost::beast::http::status::temporary_redirect);
                    // aResp->res.addHeader("Location", urlOnly);

                    // BMCWEB_ROUTE(app,
                    // "/redfish/v1/AccountService/Accounts/root")
                    //     .methods(boost::beast::http::verb::get)(
                    //         [urlOnly](const crow::Request&,
                    //                   crow::Response& /* res */) {
                    //             // aResp->res.jsonValue = {{"v1", urlOnly
                    //             "/redfish/v1/"}};
                    //             // aResp->res.end();
                    //             std::cout << "here rout xiaochao " << urlOnly
                    //                       << std::endl;
                    //         });

                    // , req, app
                    auto reqS = std::make_shared<crow::Request>(req);
                    // aResp->res.clear();
                    BMCWEB_LOG_DEBUG << "xiaochao -2";
                    reqS->url = url;

                    // BMCWEB_LOG_DEBUG << "aResp->res.bhandle= "
                    //                  << aResp->res.bhandle;

                    // aResp->res.counthandle = aResp->res.counthandle + 1;
                    aResp->res.clear();
                    BMCWEB_LOG_INFO << "-------> query.handle's aResp_count ="
                                    << aResp.use_count();
                    app.handle(*reqS, aResp);
                    BMCWEB_LOG_DEBUG << "xiaochao -3 " << reqS->url;
                }
            }
            else
            {
                std::cerr << "The member counts is "
                          << aResp->res.jsonValue["Members@odata.count"]
                          << " not meet the 'only' requirement." << std::endl;
            }
        }
        else
        {
            std::cerr << "can't find member count." << std::endl;
        }
    }
    return;
}

} // namespace query_param

} // namespace redfish