#
# This file is used to parse the js files and build the Wormhole documentation
# It's really dumb at the moment

%x[../../../tools/ReleasePackageBuild/JoDoc/jodoc --output documentation --toc WormHole_TOC.mdown --markdown ../../../tools/ReleasePackageBuild/JoDoc/Markdown.pl *.js]