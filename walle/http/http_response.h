#ifndef WALLE_HTTP_RESPONSE_H_
#define WALLE_HTTP_RESPONSE_H_
#include <map>
#include <utility>
#include <string>
#include <iosfwd>
#include <stdint.h>
#include <vector>
#include <waLLE/algo/functional>

namespace walle {
namespace http {

class header_comparator;
class arg_comparator;


/**
 * Class representing an abstraction for an Http Response. It is used from classes using these apis to send information through http protocol.
**/
class HttpResponse
{
    public:

        HttpResponse();

        /**
         * Copy constructor
         * @param b The http_response object to copy attributes value from.
        **/
        HttpResponse(const HttpResponse& b):
            _content(b._content),
            _responseCode(b._responseCode),
            _autodelete(b._autodelete),
            _realm(b._realm),
            _opaque(b._opaque),
            _reloadNonce(b._reloadNonce),
            _fp(b._fp),
            _filename(b._filename),
            _headers(b._headers),
            _footers(b._footers),
            _cookies(b._cookies),
            _topics(b._topics),
            _keepaliveSecs(b._keepaliveSecs),
            _keepaliveMsg(b._keepaliveMsg),
            _sendTopic(b._sendTopic),
            _completed(b._completed)
        {
        }

        ~HttpResponse();
        /**
         * Method used to get the _content from the response.
         * @return the _content in string form
        **/
        const std::string getContent()
        {
            return this->_content;
        }

        void getContent(std::string& result)
        {
            result = this->_content;
        }

        /**
         * Method used to get a specified header defined for the response
         * @param key The header identification
         * @return a string representing the value assumed by the header
        **/
        const std::string getHeader(const std::string& key)
        {
            return this->_headers[key];
        }

        void getHeader(const std::string& key, std::string& result)
        {
            result = this->_headers[key];
        }

        /**
         * Method used to get a specified footer defined for the response
         * @param key The footer identification
         * @return a string representing the value assumed by the footer
        **/
        const std::string getFooter(const std::string& key)
        {
            return this->_footers[key];
        }

        void getFooter(const std::string& key, std::string& result)
        {
            result = this->_footers[key];
        }

        const std::string getCookie(const std::string& key)
        {
            return this->_cookies[key];
        }

        void getCookie(const std::string& key, std::string& result)
        {
            result = this->_cookies[key];
        }

        /**
         * Method used to get all _headers passed with the request.
         * @return a map<string,string> containing all _headers.
        **/
        size_t getHeaders(
                std::map<std::string, std::string, http::header_comparator>& result
        ) const;

        /**
         * Method used to get all _footers passed with the request.
         * @return a map<string,string> containing all _footers.
        **/
        size_t getFooters(
                std::map<std::string, std::string, http::header_comparator>& result
        ) const;

        size_t getCookies(
                std::map<std::string, std::string, http::header_comparator>& result
        ) const;

        /**
         * Method used to get the response code from the response
         * @return The response code
        **/
        int getResponseCode()
        {
            return this->_responseCode;
        }

        const std::string getRealm() const
        {
            return this->_realm;
        }

        void getRealm(std::string& result) const
        {
            result = this->_realm;
        }

        const std::string getOpaque() const
        {
            return this->_opaque;
        }

        void getOpaque(std::string& result) const
        {
            result = this->_opaque;
        }

        const bool needNonceReload() const
        {
            return this->_reloadNonce;
        }

        int getSwitchCallback() const
        {
            return 0;
        }

        bool isAutodelete() const
        {
            return _autodelete;
        }

        size_t getTopics(std::vector<std::string>& _topics) const
        {
            typedef std::vector<std::string>::const_iterator _topics_it;
            for(_topics_it it=this->_topics.begin();it != this->_topics.end();++it)
                _topics.push_back(*it);
            return _topics.size();
        }
    protected:
        std::string _content;
        int _responseCode;
        bool _autodelete;
        std::string _realm;
        std::string _opaque;
        bool _reloadNonce;
        int _fp;
        std::string _filename;
        std::map<std::string, std::string, http::header_comparator> _headers;
        std::map<std::string, std::string, http::header_comparator> _footers;
        std::map<std::string, std::string, http::header_comparator> _cookies;
        std::vector<std::string> _topics;
        int _keepaliveSecs;
        std::string _keepaliveMsg;
        std::string _sendTopic;
        details::cache_entry* ce;
        bool _completed;
    private:
        HttpResponse& operator=(const HttpResponse& b);

  };
}
}
#endif
