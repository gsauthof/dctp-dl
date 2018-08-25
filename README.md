**Warning:** As of 2018-08-25 I highly recommend
[youtube-dl][youtubedl] for all your DCTP downloading needs. In
2012, youtube-dl didn't support DCTP and thus I created dctp-dl.
Nowadays, youtube-dl is also able to download videos from the
DCTP site. Also, the DCTP page has changed enough such that the
old dctp-dl implementation doesn't work anymore. Thus, the code
in this repository is only of historic interest.

## Old Introduction

dctp-dl is a small tool to download episodes from the german [DCTP][1]
television network.

## Example ##

    $ git clone 'https://github.com/gsauthof/dctp-dl.git'
    $ cd dctp-dl
    $ make update
    $ make
    $ ./dctp-dl http://www.dctp.tv/filme/abhoeren-fuer-alle_harald-welte/ harald.m4v
    $ mplayer harald.m4v


## How does it work? ##

It just fetches the [HTTP Live Streaming][2] content from a page and the
retrieved stream parts are concatenated. If you are interested how the
splitting with HTTP Live Streaming works you can look e.g. at [this
segmenter][3].

Since the stream parts are concatenated as-is, the remaining time display does
not work in video players like mplayer. Transforming the stream file a little
bit should not be that complicated, though.

## Contact ##

Georg Sauthoff <mail@georg.so>

## License ##

GPLv3+

## Build Dependencies

On Fedora/RHEL:

    libcurl-devel
    boost-regex
    boost-devel
    make
    gcc-c++ # or any other ISO C++ compiler

The packages should be named quite similiar on other distributions (e.g.
libcurl-dev instead of libcurl-devel).

For example, on Debian wheezy:

    libcurl4-gnutls-dev
    libboost-regex1.49.0
    libboost1.49-dev
    make
    g++

[1]: http://www.dctp.tv
[2]: http://en.wikipedia.org/wiki/HTTP_Live_Streaming
[3]: https://github.com/carsonmcdonald/HTTP-Live-Video-Stream-Segmenter-and-Distributor/blob/master/live_segmenter.c
[youtubedl]: https://github.com/rg3/youtube-dl
