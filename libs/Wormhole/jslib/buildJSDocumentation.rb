#
# This file is used to parse the js files and build the Wormhole documentation
# It's really dumb at the moment
require "fileutils"

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
system("../../../../tools/ReleasePackageBuild/JoDoc/jodoc --output ../html5  --title Wormhole --template WHTemplate.html --toc toc.md --markdown ../../../../tools/ReleasePackageBuild/JoDoc/Markdown.pl *.js *.md")

FileUtils.cd("../")
style_list = Dir["template/**/**"]

style_list.each { |file|
  FileUtils.cp file, "html5/"
}

FileUtils.cp "html5/_content.html", "html5/index.html"
FileUtils.rm_rf "tempDocs"
