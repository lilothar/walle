#include <walle/sys/Url.h>
#include <walle/sys/StringPice.h>

using std::string;

namespace walle {
namespace util {
string fetchStr(string& inputStr, char delimiter, bool del = true)
{
	string result;

	string::size_type pos = inputStr.find(delimiter, 0);
	if (pos == string::npos)
	{
		result = inputStr;
		if (del)
			inputStr.clear();
	}
	else
	{
		result = inputStr.substr(0, pos);
		if (del)
			inputStr = inputStr.substr(pos + 1);
	}

	return result;
}

Url::Url(const string& url)
{
	setUrl(url);
}

Url::Url(const Url& src)
{
	(*this) = src;
}
void Url::clear()
{
	_protocol.clear();
	_host.clear();
	_port.clear();
	_path.clear();
	_fileName.clear();
	_bookmark.clear();
	_userName.clear();
	_password.clear();
	_params.clear();
}
Url& Url::operator = (const Url& rhs)
{
	if (this == &rhs) return *this;

	_protocol = rhs._protocol;
	_host = rhs._host;
	_port = rhs._port;
	_path = rhs._path;
	_fileName = rhs._fileName;
	_bookmark = rhs._bookmark;
	_userName = rhs._userName;
	_password = rhs._password;
	_params = rhs._params;

	return *this;
}


string Url::getUrl() const
{
	const char SEP_CHAR = '/';
	string result;

	if (!_protocol.empty())
		result = _protocol + "://";

	if (!_userName.empty())
	{
		result += _userName;
		if (!_password.empty())
			result = result + ":" + _password;
		result += "@";
	}

	result += _host;

	if (!_port.empty())
	{
		if (walle::sys::StringPice(_protocol)==walle::sys::StringPice("HTTP"))
		{
			if (_port != "80")
				result = result + ":" + _port;
		}
		else if (walle::sys::StringPice(_protocol)==walle::sys::StringPice("HTTPS"))
		{
			if (_port != "443")
				result = result + ":" + _port;
		}
		else if (walle::sys::StringPice(_protocol)==walle::sys::StringPice("FTP"))
		{
			if (_port != "21")
				result = result + ":" + _port;
		}
	}

	// path and filename
	string str = _path;
	if (!str.empty() && str[str.length()-1] != SEP_CHAR)
		str += SEP_CHAR;
	str += _fileName;

	if (!str.empty())
	{
		if (!result.empty() && str[0] == SEP_CHAR) str.erase(0, 1);
		if (!_host.empty() && result[result.length()-1] != SEP_CHAR)
			result += SEP_CHAR;
		result += str;
	}

	if (!_params.empty())
		result = result + "?" + _params;

	if (!_bookmark.empty())
		result = result + "#" + _bookmark;

	return result;
}

string Url::getUrl(uint32_t parts)
{
    Url url(*this);

    if (!(parts & URL_PROTOCOL)) url.setProtocol("");
    if (!(parts & URL_HOST)) url.setHost("");
    if (!(parts & URL_PORT)) url.setPort("");
    if (!(parts & URL_PATH)) url.setPath("");
    if (!(parts & URL_FILENAME)) url.setFileName("");
    if (!(parts & URL_BOOKMARK)) url.setBookmark("");
    if (!(parts & URL_USERNAME)) url.setUserName("");
    if (!(parts & URL_PASSWORD)) url.setPassword("");
    if (!(parts & URL_PARAMS)) url.setParams("");

    return url.getUrl();
}

void Url::setUrl(const string& value)
{
    clear();

    string url(value);
    if (url.empty()) return;

    // get the bookmark
    string::size_type pos = url.rfind('#');
    if (pos != string::npos)
    {
        _bookmark = url.substr(pos + 1);
        url.erase(pos);
    }

    // get the parameters
    pos = url.find('?');
    if (pos != string::npos)
    {
        _params = url.substr(pos + 1);
        url = url.substr(0, pos);
    }

    string buffer;
    pos = url.find("://");
    if (pos != string::npos)
    {
        _protocol = url.substr(0, pos);
        url.erase(0, pos + 3);
        // get the user name, password, host and the port number
        buffer = fetchStr(url, '/', true);
        // get username and password
        pos = buffer.find('@');
        if (pos != string::npos)
        {
            _password = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
            _userName = fetchStr(_password, ':');
            if (_userName.empty())
                _password.clear();
        }
        // get the host and the port number
        string::size_type p1, p2;
        if ((p1 = buffer.find('[')) != string::npos &&
            (p2 = buffer.find(']')) != string::npos &&
            p2 > p1)
        {
            // this is for IPv6 Hosts
            _host = fetchStr(buffer, ']');
            fetchStr(_host, '[');
            fetchStr(buffer, ':');
        }
        else
            _host = fetchStr(buffer, ':', true);
        _port = buffer;
        // get the path
        pos = url.rfind('/');
        if (pos != string::npos)
        {
            _path = "/" + url.substr(0, pos + 1);
            url.erase(0, pos + 1);
        }
        else
            _path = "/";
    }
    else
    {
        // get the path
        pos = url.rfind('/');
        if (pos != string::npos)
        {
            _path = url.substr(0, pos + 1);
            url.erase(0, pos + 1);
        }
    }

    // get the filename
    _fileName = url;
}
}
}
