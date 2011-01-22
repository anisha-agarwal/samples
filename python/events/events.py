#!/usr/bin/env python

import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web
import urllib2, urllib
import os,sys

from tornado.options import define, options

define("port", default=9090, help="run on the given port", type=int)


class MainHandler(tornado.web.RequestHandler):

    def get(self):
        self.render('index.html', events=None)

    def post(self):
        x = {}
        x['what'] = self.get_argument('query')
        x['where'] = self.get_argument('city')
        req = 'http://www.zvents.com/rest/search?'+urllib.urlencode(x)+'&limit=10&key=XUOUXGRSMPDWVVMDXDNFFCPQEDRVMSOLPMSEKRCETTRYWYFPLSIUSFWUWEWCJIMU&format=json'
        #print req
        f = urllib2.urlopen(req)
        rsp = f.read()
        #print rsp
        rsp_json = tornado.escape.json_decode(rsp)
        events = None
        if rsp_json['rsp']['content'].has_key('events'):
          events = rsp_json['rsp']['content']['events']
        #print events
        self.render('index.html', events=events)


def main():
    tornado.options.parse_command_line()
    application = tornado.web.Application([
        (r"/", MainHandler),
    ],
    template_path=os.path.join(os.path.dirname(__file__), "templates"),
    static_path=os.path.join(os.path.dirname(__file__), "static"),
    )
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()


if __name__ == "__main__":
    main()
