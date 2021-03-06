//
// Created by danylo.kolinko on 6/3/19.
//

#ifndef CONCURRENT_MAKE_REQUESTS_H
#define CONCURRENT_MAKE_REQUESTS_H


#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "sys_spec.h"

namespace http = boost::beast::http;
static const std::string website = "yeliseev.pythonanywhere.com";


std::string get(std::string target, std::map<std::string, std::string> params);
std::vector<std::string> get_all_fields(std::string &input_json, std::string field);
std::string put_ip(std::string &login, std::string &ip, std::string &public_key, std::string& linux_dist, std::string& compiler, int major, int minor);



#endif //CONCURRENT_MAKE_REQUESTS_H
