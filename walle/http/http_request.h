#ifndef WALLE_HTTP_REQUEST_H_
#define WALLE_HTTP_REQUEST_H_

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iosfwd>
#include <walle/http/http_utils.h>

namespace walle {
namespace http {

/**
 * Class representing an abstraction for an Http Request. It is used from classes using these apis to receive information through http protocol.
**/
class HttpRequest
{
    public:

        /**
         * Method used to get the _username eventually _passed through basic authentication.
         * @return string representation of the _username.
        **/
        const std::string getUser() const
        {
            return this->_user;
        }
        /**
         * Method used to get the _username eventually _passed through basic authentication.
         * @param result string that will be filled with the _username
        **/
        void getUser(std::string& result) const
        {
            result = this->_user;
        }
        /**
         * Method used to get the _username extracted from a digest authentication
         * @return the _username
        **/
        const std::string getDigestedUser() const
        {
            return this->_digestedUser;
        }
        /**
         * Method used to get the _username extracted from a digest authentication
         * @param result string that will be filled with the _username
        **/
        void getDigestedUser(std::string& result) const
        {
            result = this->_digestedUser;
        }
        /**
         * Method used to get the _password eventually _passed through basic authentication.
         * @return string representation of the _password.
        **/
        const std::string getPass() const
        {
            return this->_pass;
        }
        /**
         * Method used to get the _password eventually _passed through basic authentication.
         * @param result string that will be filled with the _password.
        **/
        void getPass(std::string& result) const
        {
            result = this->_pass;
        }
        /**
         * Method used to get the _path requested
         * @return string representing the _path requested.
        **/
        const std::string getPath() const
        {
            return this->_path;
        }
        /**
         * Method used to get the _path requested
         * @param result string that will be filled with the _path.
        **/
        void getPath(std::string& result) const
        {
            result = this->_path;
        }
        /**
         * Method used to get all pieces of the _path requested; considering an url splitted by '/'.
         * @return a vector of strings containing all pieces
        **/
        const std::vector<std::string> getPathPieces() const
        {
            return this->_postPath;
        }
        /**
         * Method used to get all pieces of the _path requested; considering an url splitted by '/'.
         * @param result vector of strings containing the _path
         * @return the size of the vector filled
        **/
        size_t getPathPieces(std::vector<std::string>& result) const
        {
            result = this->_postPath;
            return result.size();
        }
        /**
         * Method used to obtain the size of _path in terms of pieces; considering an url splitted by '/'.
         * @return an integer representing the number of pieces
        **/
        size_t getPathPiecesSize() const
        {
            return this->_postPath.size();
        }
        /**
         * Method used to obtain a specified piece of the _path; considering an url splitted by '/'.
         * @param index the index of the piece selected
         * @return the selected piece in form of string
        **/
        const std::string getPathPiece(int index) const
        {
            if(((int)(this->_postPath.size())) > index)
                return this->_postPath[index];
            return "";
        }
        /**
         * Method used to obtain a specified piece of the _path; considering an url splitted by '/'.
         * @param index the index of the piece selected
         * @param result a string that will be filled with the piece found
         * @return the length of the piece found
        **/
        size_t getPathPiece(int index, std::string& result) const
        {
            if(((int)(this->_postPath.size())) > index)
            {
                result = this->_postPath[index];
                return result.size();
            }
            else
            {
                result = "";
                return result.size();
            }
        }
        /**
         * Method used to get the METHOD used to make the request.
         * @return string representing the _method.
        **/
        const std::string getMethod() const
        {
            return this->_method;
        }
        /**
         * Method used to get the METHOD used to make the request.
         * @param result string that will be filled with the _method.
        **/
        void getMethod(std::string& result) const
        {
            result = this->_method;
        }
        /**
         * Method used to get all _headers _passed with the request.
         * @param result a map<string, string> > that will be filled with all _headers
         * @result the size of the map
        **/
        size_t getHeaders(std::map<std::string, std::string, http::header_comparator>& result) const;
        /**
         * Method used to get all _footers _passed with the request.
         * @param result a map<string, string> > that will be filled with all _footers
         * @result the size of the map
        **/
        size_t getFooters(std::map<std::string, std::string, http::header_comparator>& result) const;
        /**
         * Method used to get all _cookies _passed with the request.
         * @param result a map<string, string> > that will be filled with all _cookies
         * @result the size of the map
        **/
        size_t getCookies(std::map<std::string, std::string, http::header_comparator>& result) const;
        /**
         * Method used to get all _args _passed with the request.
         * @param result a map<string, string> > that will be filled with all _args
         * @result the size of the map
        **/
        size_t getArgs(std::map<std::string, std::string, http::arg_comparator>& result) const;
        /**
         * Method used to get a specific header _passed with the request.
         * @param key the specific header to get the value from
         * @return the value of the header.
        **/
        const std::string getHeader(const std::string& key) const
        {
            std::map<std::string, std::string>::const_iterator it =
                this->_headers.find(key);
            if(it != this->_headers.end())
                return it->second;
            else
                return "";
        }
        void getHeader(const std::string& key, std::string& result) const
        {
            std::map<std::string, std::string>::const_iterator it =
                this->_headers.find(key);
            if(it != this->_headers.end())
                result = it->second;
            else
                result = "";
        }
        const std::string getCookie(const std::string& key) const
        {
            std::map<std::string, std::string>::const_iterator it =
                this->_cookies.find(key);
            if(it != this->_cookies.end())
                return it->second;
            else
                return "";
        }
        void getCookie(const std::string& key, std::string& result) const
        {
            std::map<std::string, std::string>::const_iterator it =
                this->_cookies.find(key);
            if(it != this->_cookies.end())
                result = it->second;
            else
                result = "";
        }
        /**
         * Method used to get a specific footer _passed with the request.
         * @param key the specific footer to get the value from
         * @return the value of the footer.
        **/
        const std::string getFooter(const std::string& key) const
        {
            std::map<std::string, std::string>::const_iterator it =
                this->_footers.find(key);
            if(it != this->_footers.end())
                return it->second;
            else
                return "";
        }
        void getFooter(const std::string& key, std::string& result) const
        {
            std::map<std::string, std::string>::const_iterator it =
                this->_footers.find(key);
            if(it != this->_footers.end())
                result = it->second;
            else
                result = "";
        }
        /**
         * Method used to get a specific argument _passed with the request.
         * @param ket the specific argument to get the value from
         * @return the value of the arg.
        **/
        const std::string getArg(const std::string& key) const
        {
            std::map<std::string, std::string>::const_iterator it =
                this->_args.find(key);
            if(it != this->_args.end())
                return it->second;
            else
                return "";
        }
        void getArg(const std::string& key, std::string& result) const
        {
            std::map<std::string, std::string>::const_iterator it =
                this->_args.find(key);
            if(it != this->_args.end())
                result = it->second;
            else
                result = "";
        }
        /**
         * Method used to get the _content of the request.
         * @return the _content in string representation
        **/
        const std::string getContent() const
        {
            return this->_content;
        }
        void getContent(std::string& result) const
        {
            result = this->_content;
        }
        const std::string getQueryString() const
        {
            return this->_querystring;
        }
        void getQueryString(std::string& result) const
        {
            result = this->_querystring;
        }
        /**
         * Method used to get the _version of the request.
         * @return the _version in string representation
        **/
        const std::string getVersion() const
        {
            return this->_version;
        }
        void getVersion(std::string& result) const
        {
            result = this->_version;
        }
        /**
         * Method used to get the _requestor.
         * @return the _requestor
        **/
        const std::string getRequestor() const
        {
            return this->_requestor;
        }
        void getRequestor(std::string& result) const
        {
            result = this->_requestor;
        }
        /**
         * Method used to get the _requestor port used.
         * @return the _requestor port
        **/
        short getRequestorPort() const
        {
            return this->_requestor_port;
        }

		/**
         * Method used to set an header value by key.
         * @param key The name identifying the header
         * @param value The value assumed by the header
        **/
        void setHeader(const std::string& key, const std::string& value)
        {
            this->_headers[key] = value;
        }
        /**
         * Method used to set a footer value by key.
         * @param key The name identifying the footer
         * @param value The value assumed by the footer
        **/
        void setFooter(const std::string& key, const std::string& value)
        {
            this->_footers[key] = value;
        }
        /**
         * Method used to set a cookie value by key.
         * @param key The name identifying the cookie
         * @param value The value assumed by the cookie
        **/
        void setCookie(const std::string& key, const std::string& value)
        {
            this->_cookies[key] = value;
        }
        /**
         * Method used to set an argument value by key.
         * @param key The name identifying the argument
         * @param value The value assumed by the argument
        **/
        void setArg(const std::string& key, const std::string& value)
        {
            this->_args[key] = value;
        }
        /**
         * Method used to set an argument value by key.
         * @param key The name identifying the argument
         * @param value The value assumed by the argument
         * @param size The size in number of char of the value parameter.
        **/
        void setArg(const char* key, const char* value, size_t size)
        {
            this->_args[key] = std::string(value, size);
        }
        /**
         * Method used to set the _content of the request
         * @param _content The _content to set.
        **/
        void setContent(const std::string& _content)
        {
            this->_content = _content;
        }
        /**
         * Method used to append _content to the request preserving the previous inserted _content
         * @param _content The _content to append.
         * @param size The size of the data to append.
        **/
        void growContent(const char* _content, size_t size)
        {
            this->_content.append(_content, size);
        }
        /**
         * Method used to set the _path requested.
         * @param _path The _path searched by the request.
        **/
        void setPath(const std::string& _path)
        {
            this->_path = _path;
            std::vector<std::string> complete__path;
            http::http_utils::tokenize_url(this->_path, complete__path);
            for(unsigned int i = 0; i < complete__path.size(); i++)
            {
                this->_postPath.push_back(complete__path[i]);
            }
        }
        /**
         * Method used to set the request METHOD
         * @param _method The _method to set for the request
        **/
        void setMethod(const std::string& _method);
        /**
         * Method used to set the request http _version (ie http 1.1)
         * @param _version The _version to set in form of string
        **/
        void setVersion(const std::string& _version)
        {
            this->_version = _version;
        }
        /**
         * Method used to set the _requestor
         * @param _requestor The _requestor to set
        **/
        void setRequestor(const std::string& _requestor)
        {
            this->_requestor = _requestor;
        }
        /**
         * Method used to set the _requestor port
         * @param _requestor The _requestor port to set
        **/
        void setRequestorPort(short _requestor_port)
        {
            this->_requestor_port = _requestor_port;
        }
        /**
         * Method used to remove an header previously inserted
         * @param key The key identifying the header to remove.
        **/
        void removeHeader(const std::string& key)
        {
            this->_headers.erase(key);
        }
        /**
         * Method used to set all _headers of the request.
         * @param _headers The _headers key-value map to set for the request.
        **/
        void setHeaders(const std::map<std::string, std::string>& _headers)
        {
            std::map<std::string, std::string>::const_iterator it;
            for(it = _headers.begin(); it != _headers.end(); ++it)
                this->_headers[it->first] = it->second;
        }
        /**
         * Method used to set all _footers of the request.
         * @param _footers The _footers key-value map to set for the request.
        **/
        void setFooters(const std::map<std::string, std::string>& _footers)
        {
            std::map<std::string, std::string>::const_iterator it;
            for(it = _footers.begin(); it != _footers.end(); ++it)
                this->_footers[it->first] = it->second;
        }
        /**
         * Method used to set all _cookies of the request.
         * @param _cookies The _cookies key-value map to set for the request.
        **/
        void setCookies(const std::map<std::string, std::string>& _cookies)
        {
            std::map<std::string, std::string>::const_iterator it;
            for(it = _cookies.begin(); it != _cookies.end(); ++it)
                this->_cookies[it->first] = it->second;
        }
        /**
         * Method used to set all arguments of the request.
         * @param _args The _args key-value map to set for the request.
        **/
        void setArgs(const std::map<std::string, std::string>& _args)
        {
            std::map<std::string, std::string>::const_iterator it;
            for(it = _args.begin(); it != _args.end(); ++it)
                this->_args[it->first] = it->second;
        }
        /**
         * Method used to set the _username of the request.
         * @param _user The _username to set.
        **/
        void setUser(const std::string& _user)
        {
            this->_user = _user;
        }
        void setDigestedUser(const std::string& _digestedUser)
        {
            this->_digestedUser = _digestedUser;
        }
        /**
         * Method used to set the _password of the request.
         * @param _pass The _password to set.
        **/
        void setPass(const std::string& _pass)
        {
            this->_pass = _pass;
        }
		friend std::ostream &operator<< (std::ostream &os, const HttpRequest &r);

        /**
         * Default constructor of the class. It is a specific responsibility of apis to initialize this type of objects.
        **/
       HttpRequest():
            _content("")
        {
        }
        /**
         * Copy constructor.
         * @param b http_request b to copy attributes from.
        **/
        HttpRequest(const HttpRequest& b):
            _user(b._user),
            _pass(b._pass),
            _path(b._path),
            _digestedUser(b._digestedUser),
            _method(b._method),
            _postPath(b._postPath),
            _headers(b._headers),
            _footers(b._footers),
            _cookies(b._cookies),
            _args(b._args),
            _querystring(b._querystring),
            _content(b._content),
            _version(b._version),
            _requestor(b._requestor)
        {
        }
private:
        std::string _user;
        std::string _pass;
        std::string _path;
        std::string _digestedUser;
        std::string _method;
        std::vector<std::string> _postPath;
        std::map<std::string, std::string, http::header_comparator> _headers;
        std::map<std::string, std::string, http::header_comparator> _footers;
        std::map<std::string, std::string, http::header_comparator> _cookies;
        std::map<std::string, std::string, http::arg_comparator> _args;
        std::string _querystring;
        std::string _content;
        std::string _version;
        std::string _requestor;

        short _requestor_port;
        
        

};


}
}
#endif
