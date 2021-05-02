#pragma once
#include <string>
#include <restbed>
void message_handler(const std::shared_ptr<restbed::Session> session);
void add_user_handlers(restbed::Service& service);