#!/usr/bin/ruby

require 'webrick'
include WEBrick

class EchoServlet < HTTPServlet::AbstractServlet
	def do_POST(req, resp)
		resp.status = 299

		#resp.header = req.header	#can't; resp doesn't have a header member.
		#instead, we do this:
		req.header.each_pair { |key, value|
			#if(key != "Transfer-Encoding" && key != "User-Agent" && key != "Content-Length") then
			if(key.include? "-") then
				puts "Skipping header '#{key}'"
			else
				puts "Reflecting header '#{key}'"
				resp[key] = value
			end
		}

		resp.body = req.body
	end
end

def start_webrick(config = {})
	# always listen on port x
	config.update(:Port => 5004)
	server = HTTPServer.new(config)
	yield server if block_given?
	['INT', 'TERM'].each {|signal|
		trap(signal) {server.shutdown}
	}
	server.start
end

#debug_logger = Log.new($stdout, Log::DEBUG)

start_webrick(
#:Logger => debug_logger, :AccessLog => debug_logger
) {|server| server.mount('/post', EchoServlet) }

