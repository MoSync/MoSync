
require "fileutils"

js_files = {
  "nativeui.md" => "mosync-nativeui.js",
  "bridge.md" => "mosync-bridge.js",
 # "sensormanager.md" => "mosync-sensormanager.js",
  "pushnotifications.md" => "mosync-pushnotifications.js",
  "resource.md" =>"mosync-resource.js"
  }

isInCommentBlock = false
hasStartedParamBlock = false
shouldAddFunctionDefinition = false
functionList = Array.new
parentName = "";
moduleName = "";
ignoreMode = false;
markdown_mode = false
if(!File.exist?("tempDocs"))
  FileUtils.mkdir("tempDocs")
end
js_files.each { |mdFile, fileName|
  sourceFile  = File.new(fileName, "r")
  destFile = File.new("tempDocs/Fixed_#{fileName}", "w")
    while(line = sourceFile.gets)
      if(isInCommentBlock == false)
        if(line.strip.start_with?("/**"))
          destComments = Array.new
          destComments.push("#{line.strip}")
          isInCommentBlock = true;
        elsif(shouldAddFunctionDefinition == true)
          if(line.strip != "")
            newComments = Array.new
            newComments[0] = destComments[0]
            functionName = line.strip.split("=")[0]
            if(functionName.start_with?("var"))
              functionName = functionName.split("var")[1..-1].join("").strip
            elsif(functionName.include?("prototype"))
              splittedFunctionName = functionName.split("prototype.")
              functionName = "#{splittedFunctionName[0]}#{splittedFunctionName[1]}".strip
            end
            if(functionName.start_with?("this"))
              functionName = "#{parentName}.#{functionName.split(".")[1..-1].join(".")}".strip
            else
              parentName = functionName.strip
            end
            if(markdown_mode == false)
              newComments[1] = functionName.strip
              newComments[2] = "=" * newComments[1].length
            end
            markdown_mode = false
            newComments.concat(destComments[1..-1])
            shouldAddFunctionDefinition = false
            if(!ignoreMode)
              functionList.push(functionName.strip)
              destFile.puts(newComments.join("\n"));
            end
            destFile.puts(line);
            ignoreMode = false
          end
        else
          destFile.puts(line);
        end
      else
        if(line.include?("*/"))
          destComments.push("#{line.strip}\n")
          isInCommentBlock = false;
          shouldAddFunctionDefinition = true
          hasStartedParamBlock = false
        else
          if(line.strip.start_with?("*"))
            strippedLine = line.strip[1..-1]
          else
            strippedLine = line.strip
          end
          # Detecting Specific Tags in the comments

          if(strippedLine.strip.start_with?("@private"))
            ignoreMode = true
          elsif(markdown_mode == true)
            destComments.push(strippedLine)
          elsif(strippedLine.strip.start_with?("@param"))
            splittedLine = strippedLine.split(" ")
            if(hasStartedParamBlock == false)
              destComments.push("\n**Parameters:**\n")
              destComments.push(" * **#{splittedLine[1]}** #{splittedLine[2..-1].join(" ")}")
              hasStartedParamBlock = true;
            else
              destComments.push(" * **#{splittedLine[1]}** #{splittedLine[2..-1].join(" ")}")
            end
          elsif(strippedLine.strip.start_with?("@markdown"))
            markdown_mode = true
          elsif(strippedLine.strip.start_with?("@returns"))
            splittedLine = strippedLine.split(" ")
            destComments.push("\n**ReturnValue:** #{splittedLine[1]} #{splittedLine[2..-1].join(" ")}")
          elsif(strippedLine.strip.start_with?("\\code"))
            enteredCodeMode = true
          elsif(strippedLine.strip.start_with?("\\endcode"))
            enteredCodeMode = false
            destComments.push("\n");
          elsif(strippedLine.strip.start_with?("@namespace"))
             destComments.push("This is a class. Please look into the memebers for usage")
          elsif(strippedLine.strip.start_with?("@module"))
              moduleName = strippedLine.strip.split(" ")[1..-1].join(" ")
          else
            #Non Tagged lines
            if(enteredCodeMode == true)
                destComments.push("\t\t#{strippedLine}");
            else
              destComments.push(strippedLine);
            end
          end
          #End of Speficic Tag Detection writing part
        end
      end

    end
    destFile.close
    sourceFile.close

    combinedFile = File.new("tempDocs/#{fileName}", "w")
    combinedFile.puts("/**")
    combinedFile.puts("Contents")
    combinedFile.puts("=" * "Contents".length)
    functionList.each do |functionItem|
      combinedFile.puts("- #{functionItem}")
    end
    combinedFile.puts("*/")

    destFile = File.new("tempDocs/Fixed_#{fileName}", "r")
    while(line = destFile.gets)
      combinedFile.puts(line)
    end
    destFile.close
    combinedFile.close
    puts "modifying #{mdFile}"
    if(File.exist?("tempDocs/#{mdFile}"))
      File.open("tempDocs/#{mdFile}", "a") do |markdownFile|
        functionList.each do |functionItem|
          markdownFile.puts("- #{functionItem}")
        end

      end
    end

    FileUtils.rm_rf("tempDocs/Fixed_#{fileName}")
    functionList = Array.new
    moduleName = ""
#end of itration over input files
  }
