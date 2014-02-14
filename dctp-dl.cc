// Download client for HTTP Live Streaming content from dctp
//
// by Georg Sauthoff <mail@georg.so>, 2012, GPLv3+

#include "ccurl/curl.hh"

#include <fstream>
#include <stdexcept>
#include <cassert>


#include <iostream>

#include <boost/regex.hpp>

using namespace boost;

using namespace std;

#include <unistd.h>

class dctp {

  private:
    curl::global g;
    vector<char> v;
    curl::callback::vector cb;
    curl::handle h;

    static char const agent[];

    typedef match_results<vector<char>::const_iterator> vmatch;

    void dump(const vector<char> &v)
    {
      string p(v.begin(), v.end());
      cerr << "Result: \n" << p << '\n';
    }
    string get_fst_re_group(const string &expr, const string &url)
    {
      cb.clear();
      cerr << "Fetching: " << url << '\n';
      h.get(url);
      cerr << "Matching: |" << expr << "|\n";
      regex re(expr, regex::extended);
      vmatch results;
      vector<char>::const_iterator b = v.begin(), e = v.end();
      if (regex_search(b, e, results, re)) {
        vmatch::const_reference r = results[1];
        string m(r.first, r.second);
        return m;
      } else {
        dump(v);
        throw runtime_error("Could not find playlist link");
      }
      return "";
    }
  public:

    dctp()
      : cb(v), h(g, cb)
    {
      h.set_useragent(agent);
    }

    string get_streaming_server(const string &url)
    {
      return get_fst_re_group("\"server\": \"([a-zA-Z0-9.-]+)\"", url);
    }

    string get_pl_link(const string &sserver, const string &url)
    {
      return "http://" + sserver + get_fst_re_group("http://' \\+ data\\[0\\]\\.server \\+ '([/_.:a-zA-Z0-9]+)", url);
    }

    string get_session_link(const string &url)
    {
      return get_fst_re_group("(http://[a-zA-Z0-9./_:-]+/playlist.m3u8\\?wowzasessionid=[0-9]+)", url);
    }

    void get_part_links(vector<string> &parts, const string &url)
    {
      size_t last = url.rfind('/');
      if (last == string::npos)
        throw runtime_error("Session url error");
      string prefix(url.substr(0, last+1));
      cb.clear();
      cerr << "Fetching: " << url << '\n';
      h.get(url);
      regex re("(media_[0-9]+\\.ts\\?wowzasessionid=[0-9]+)", regex::extended);
      vmatch results;
      vector<char>::const_iterator b = v.begin(), e = v.end();
      while (regex_search(b, e, results, re)) {
        vmatch::const_reference r = results[1];
        string m(r.first, r.second);
        parts.push_back(prefix + m);
        cerr << "Saw part: " << parts.back() << '\n';
        b = r.second;
      }
      if (parts.empty())
        throw runtime_error("Reading parts links failed");
    }

    void fetch_parts(const vector<string> &parts, const string &dest)
    {
      ofstream o(dest.c_str());
      curl::callback::ostream fb(o, size_t(5) * size_t(1024) * size_t(1024) * size_t(1024));
      curl::handle handle(g, fb);
      handle.set_useragent(agent);
      for (const string &f : parts) {
        cerr << "Fetching: " << f << '\n';
        handle.get(f);
        sleep(1);
      }
    }


};

char const dctp::agent[] = "Firefox 24/Linux: Mozilla/5.0 (X11; Linux x86_64; rv:24.0) Gecko/20100101 Firefox/24.0";


int main(int argc, char **argv)
{
  dctp d;

  if (argc == 6) {
    // e.g. ./dctp-dl file://json file://index.html file://playlist.m3u8 file://playlist.m3u8\?wowzasessionid=575989031 test.m4v

    string sserver = d.get_streaming_server(argv[1]);
    cout << sserver << '\n';
    string pl  = d.get_pl_link(sserver, argv[2]);
    cout << pl << '\n';
    //string sl = d.get_session_link(pl);
    string sl = d.get_session_link(argv[3]);
    cout << sl << '\n';
    vector<string> parts;
    //d.get_part_links(parts, sl);
    d.get_part_links(parts, argv[4]);
    d.fetch_parts(parts, argv[5]); 
  } else if (argc == 3) {

    string sserver = d.get_streaming_server("http://www.dctp.tv/elastic_streaming_client/get_streaming_server/");
    cout << sserver << '\n';
    string pl  = d.get_pl_link(sserver, argv[1]);
    cout << pl << '\n';
    string sl = d.get_session_link(pl);
    cout << sl << '\n';

    vector<string> parts;
    d.get_part_links(parts, sl);
    d.fetch_parts(parts, argv[2]); 
  } else {
    cerr << "call: " << argv[0] << "URL OUTFILE\n";
    return 1;
  }
  return 0;
}


