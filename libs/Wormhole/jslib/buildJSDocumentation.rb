#
# This file is used to parse the js files and build the Wormhole documentation
# It's really dumb at the moment
require "fileutils"
require "rubygems"
require "nokogiri"

def error_exit(message)
  puts message
  exit!(1)
end
if(!File.exist?("tempDocs"))
  FileUtils.mkdir("tempDocs")
end

pg_files = Dir["mdDocs/**/**"]

pg_files.each { |fileName|
  if(fileName.include?(".md"))
    FileUtils.cp fileName, "./tempDocs/"
  end
}

puts "running converter"
system "ruby joDocConverter.rb"
FileUtils.cp "template/WHTemplate.html", "tempDocs"
FileUtils.cp "mdDocs/toc.md", "tempDocs"

FileUtils.cd("tempDocs")

if(!system("chmod +x ../../../../tools/ReleasePackageBuild/JoDoc/jodoc"))
  error_exit("failed to change mode of jodoc")
end
if(!system("chmod +x ../../../../tools/ReleasePackageBuild/JoDoc/Markdown.pl"))
  error_exit("failed to change mode of markdown")
end
if(!system("../../../../tools/ReleasePackageBuild/JoDoc/jodoc --output ../html5  --title \"JavaScript API Documentation\" --template WHTemplate.html --toc toc.md --markdown ../../../../tools/ReleasePackageBuild/JoDoc/Markdown.pl *.js *.md"))
  error_exit("failed to run jodoc")
end

FileUtils.cd("../")
style_list = Dir["template/**"]
puts style_list
style_list.each { |file|
  FileUtils.cp_r file, "html5/"
}

FileUtils.cp "html5/index.md.html", "html5/index.html"

html_files = Dir["html5/**/**"]

#read the contents of the index.html file
doc = Nokogiri::HTML(File.read("html5/_content.html"))
content_source = doc.css('#contents_div')[0]
html_files.each do |filename|
  if(filename.include?(".html"))
    dest_doc = Nokogiri::HTML(File.read(filename))
    content_dest = dest_doc.css('#sidebar')[0]
    content_dest.inner_html = content_source.inner_html
    #prettify the code
    dest_doc.css('#contents_div pre').each do |tag|
      tag['class'] = 'prettyprint'
    end

    File.open(filename, 'w') { |file| file.write dest_doc.to_html }

  end
end
FileUtils.rm_rf "tempDocs"
