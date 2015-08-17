#include <walle/store/regex.h>
#include <walle/store/myconf.h>

extern "C" {
#include <regex.h>
}

namespace walle {
namespace store {

/**
 * Regex internal.
 */
struct RegexCore {
  ::regex_t rbuf;
  bool alive;
  bool nosub;
};


/**
 * Default constructor.
 */
Regex::Regex() : opq_(NULL) {
  assert(true);
  RegexCore* core = new RegexCore;
  core->alive = false;
  core->nosub = false;
  opq_ = (void*)core;
}


/**
 * Destructor.
 */
Regex::~Regex() {
  assert(true);
  RegexCore* core = (RegexCore*)opq_;
  if (core->alive) ::regfree(&core->rbuf);
  delete core;

}


/**
 * Compile a string of regular expression.
 */
bool Regex::compile(const std::string& regex, uint32_t opts) {
  assert(true);
  RegexCore* core = (RegexCore*)opq_;
  if (core->alive) {
    ::regfree(&core->rbuf);
    core->alive = false;
  }
  int32_t cflags = REG_EXTENDED;
  if (opts & IGNCASE) cflags |= REG_ICASE;
  if ((opts & MATCHONLY) || regex.empty()) {
    cflags |= REG_NOSUB;
    core->nosub = true;
  }
  if (::regcomp(&core->rbuf, regex.c_str(), cflags) != 0) return false;
  core->alive = true;
  return true;

}


/**
 * Check whether a string matches the regular expression.
 */
bool Regex::match(const std::string& str) {
  assert(true);
  RegexCore* core = (RegexCore*)opq_;
  if (!core->alive) return false;
  if (core->nosub) return ::regexec(&core->rbuf, str.c_str(), 0, NULL, 0) == 0;
  ::regmatch_t subs[1];
  return ::regexec(&core->rbuf, str.c_str(), 1, subs, 0) == 0;

}


/**
 * Check whether a string matches the regular expression.
 */
std::string Regex::replace(const std::string& str, const std::string& alt) {
  assert(true);
  RegexCore* core = (RegexCore*)opq_;
  if (!core->alive || core->nosub) return str;
  regmatch_t subs[256];
  if (::regexec(&core->rbuf, str.c_str(), sizeof(subs) / sizeof(*subs), subs, 0) != 0)
    return str;
  const char* sp = str.c_str();
  std::string xstr;
  bool first = true;
  while (sp[0] != '\0' && ::regexec(&core->rbuf, sp, 10, subs, first ? 0 : REG_NOTBOL) == 0) {
    first = false;
    if (subs[0].rm_so == -1) break;
    xstr.append(sp, subs[0].rm_so);
    for (const char* rp = alt.c_str(); *rp != '\0'; rp++) {
      if (*rp == '$') {
        if (rp[1] >= '0' && rp[1] <= '9') {
          int32_t num = rp[1] - '0';
          if (subs[num].rm_so != -1 && subs[num].rm_eo != -1)
            xstr.append(sp + subs[num].rm_so, subs[num].rm_eo - subs[num].rm_so);
          ++rp;
        } else if (rp[1] == '&') {
          xstr.append(sp + subs[0].rm_so, subs[0].rm_eo - subs[0].rm_so);
          ++rp;
        } else if (rp[1] != '\0') {
          xstr.append(++rp, 1);
        }
      } else {
        xstr.append(rp, 1);
      }
    }
    sp += subs[0].rm_eo;
    if (subs[0].rm_eo < 1) break;
  }
  xstr.append(sp);
  return xstr;

}


}
}                                        // common namespace

// END OF FILE
