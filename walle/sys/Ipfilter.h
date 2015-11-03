#ifndef WALLE_SYS_IPFILTER_H_
#define WALLE_SYS_IPFILTER_H_

#include <string>
#include <set>

// filter ip address
namespace walle {
namespace util {
    
class IpFilter{


public:

	IpFilter():_denyall(false),
                _allowall(false),
                _empty(true)

    {

	}
	
	bool empty(){
		return _empty;
	}
	
	void addAllow(const std::string &ip_prefix){
		if(ip_prefix == "all" || ip_prefix == "*"){
			_allowall = true;
		}else{
			// '@' and '=' is greater than any char in ip
			std::string prefix = ip_prefix + (isFullIp(ip_prefix)? "=" : "@");
			_allow.insert(prefix);
		}
		_empty = false;
	}
	
	void addDeny(const std::string &ip_prefix){
		if(ip_prefix == "all" || ip_prefix == "*"){
			_denyall = true;
		}else{
			// '@' and '=' is greater than any char in ip
			std::string prefix = ip_prefix + (isFullIp(ip_prefix)? "=" : "@");
			_deny.insert(prefix);
		}
		_empty = false;
	}
	
	bool checkPass(const std::string &ip){
		if(_empty){
			return true;
		}
        if(_denyall){
			return false;
		}
		if(_allowall){
			return true;
		}
		// check specified allow/deny
		if(checkHit(_allow, ip)){
			return true;
		}
		if(checkHit(_deny, ip)){
			return false;
		}
	
		return false;
	}
private:
	bool _denyall;
	bool _allowall;
	bool _empty;
	std::set<std::string> _deny;
	std::set<std::string> _allow;
	
	bool checkHit(const std::set<std::string> &m, const std::string &ip)
    {
		if(m.empty()){
			return false;
		}
		std::set<std::string>::const_iterator it;
		it = m.upper_bound(ip);
		if(it == m.end()){
			return false;
		}
		const std::string &prefix = *it;

		size_t len = prefix.size() - 1;
		if(prefix[len] == '='){
			return prefix.compare(0, len, ip) == 0;
		}else if(ip.size() > len){
			return ip.compare(0, len, prefix, 0, len) == 0;
		}
		return false;
	}
	
	bool isFullIp(const std::string &ip_prefix){
		int n = 0;
		for(int i=0; i<(int)ip_prefix.size(); i++){
			if(ip_prefix[i] == '.'){
				n ++;
			}
		}
		return n == 3;
	}
};

}
}
#endif
