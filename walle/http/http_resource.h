#ifndef WALLE_HTTP_RESOURCE_H_
#define WALLE_HTTP_RESOURCE_H_
#include <map>
#include <string>

namespace walle {
namespace http {
	
class HttpRequest;
class HttpResponse;

namespace details
{

void empty_render(const HttpRequest& r, HttpResponse** res);

}




class HttpResource
{
    public:
		/**
         * Constructor of the class
        **/
        HttpResource()
        {
            resource_init();
        }
        /**
         * Class destructor
        **/
        virtual ~HttpResource()
        {
        }

        /**
         * Method used to answer to a generic request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void render(const HttpRequest& r, HttpResponse** res)
        {
            details::empty_render(r, res);
        }
        /**
         * Method used to answer to a GET request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void renderGET(const HttpRequest& req, HttpResponse** res)
        {
            render(req, res);
        }
        /**
         * Method used to answer to a POST request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void renderPOST(const HttpRequest& req, HttpResponse** res)
        {
            render(req, res);
        }
        /**
         * Method used to answer to a PUT request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void renderPUT(const HttpRequest& req, HttpResponse** res)
        {
            render(req, res);
        }
        /**
         * Method used to answer to a HEAD request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void renderHEAD(const HttpRequest& req, HttpResponse** res)
        {
            render(req, res);
        }
        /**
         * Method used to answer to a DELETE request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void renderDELETE(const HttpRequest& req, HttpResponse** res)
        {
            render(req, res);
        }
        /**
         * Method used to answer to a TRACE request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void renderTRACE(const HttpRequest& req, HttpResponse** res)
        {
            render(req, res);
        }
        /**
         * Method used to answer to a OPTIONS request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void renderOPTIONS(const HttpRequest& req, HttpResponse** res)
        {
            render(req, res);
        }
        /**
         * Method used to answer to a CONNECT request
         * @param req Request passed through http
         * @return A HttpResponse object
        **/
        virtual void renderCONNECT(const HttpRequest& req, HttpResponse** res)
        {
            render(req, res);
        }
        /**
         * Method used to set if a specific method is allowed or not on this request
         * @param method method to set permission on
         * @param allowed boolean indicating if the method is allowed or not
        **/
        void setAllowing(const std::string& method, bool allowed)
        {
            if(this->_allowedMethods.count(method))
            {
                this->_allowedMethods[method] = allowed;
            }
        }
        /**
         * Method used to implicitly allow all methods
        **/
        void allowAll()
        {
            std::map<std::string,bool>::iterator it;
            for ( it=this->_allowedMethods.begin() ; it != this->_allowedMethods.end(); ++it )
                this->_allowedMethods[(*it).first] = true;
        }
        /**
         * Method used to implicitly disallow all methods
        **/
        void disallowAll()
        {
            std::map<std::string,bool>::iterator it;
            for ( it=this->_allowedMethods.begin() ; it != this->_allowedMethods.end(); ++it )
                this->_allowedMethods[(*it).first] = false;
        }
        /**
         * Method used to discover if an http method is allowed or not for this resource
         * @param method Method to discover allowings
         * @return true if the method is allowed
        **/
        bool isAllowed(const std::string& method)
        {
            if(this->_allowedMethods.count(method)) {
                return this->_allowedMethods[method];
            } else {
                return false;
            }
        }
    protected:
		
		/**
		 * Class representing a callable http resource.
		**/
		void resource_init();
        /**
         * Copy constructor
        **/
        HttpResource(const HttpResource& b) : _allowedMethods(b._allowedMethods) { }

        HttpResource& operator = (const HttpResource& b)
        {
            _allowedMethods = b._allowedMethods;
            return (*this);
        }

    private:
        std::map<std::string, bool> _allowedMethods;
};

}
}
#endif
