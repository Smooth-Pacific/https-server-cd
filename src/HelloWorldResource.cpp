#include <HelloWorldResource.h>

const std::shared_ptr<httpserver::http_response> hello_world_resource::render(const httpserver::http_request& req){
    if(req.get_digested_user() == ""){
            return std::shared_ptr<httpserver::digest_auth_fail_response>(
                new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE
            ));
        }
        else{
            bool reload_nonce = false;

            if(!req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)){
                return std::shared_ptr<httpserver::digest_auth_fail_response>(
                    new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, reload_nonce
                ));
            }
        }
    // get requestor IP address; does nothing for now but I will probably log accesses
    std::string ip_addr = req.http_request::get_requestor();

    return std::shared_ptr<httpserver::string_response>(new httpserver::string_response(
        ip_addr, 200, "text/plain"
    ));
}