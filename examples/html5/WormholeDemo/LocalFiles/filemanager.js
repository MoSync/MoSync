var userFilesDir; //DirectoryEntry for the UserFiles directory
var currentFile; //The file that is being edited

/**
 * This function initializes the file manager
 */
function initFileManager()
{
	window.requestFileSystem(
		LocalFileSystem.PERSISTENT,
		0,
		// Success callback.
		function(fileSystem)
		{
			//The "root" is actually the local file storage
			//directory for our app
			fileSystem.root.getDirectory(
				"UserFiles",
				{create: false, exclusive: false},
				function(dirEntry)
				{
					userFilesDir = dirEntry;
				},
				function(error)
				{
					alert(error);
				});
		},
		// Error callback.
		function(error)
		{
			alert("fs error");
		});
}

/**
 * This function refreshes the file list in the file manager panel
 */
function refreshFiles()
{
	var dirReader = userFilesDir.createReader();
	//Clear the file list
	$('#fileList > li').remove();
	dirReader.readEntries(function(entries)
	{
		for(var i = 0; i < entries.length; i++)
		{
			//Create a nested <li><a> structure for each file returned
			var newLi = document.createElement("li");
			var newA = document.createElement("a");
			//Store the file data to the <a> element
			$(newA).data('file', entries[i]);
			//Set the <a> element to call the setFile function with the stored file when clicked
			newA.setAttribute("onclick", "setFile($(this).data('file'))");
			newA.innerHTML = entries[i].name;
			newLi.appendChild(newA);
			$('#fileList').append(newLi);
		}
	},
	function(error)
	{
		alert("error");
	});
}

/**
 * Sets the file editing panel to the selected file
 * @param file FilEntry with the info about the selected file
 */
function setFile(file)
{
	//We programmatically go to the file editing panel
	jQT.goTo('#filePage','pop');
	currentFile = file;

	$('#filePanelTitle').html(currentFile.name);
	$('#fileName').val(currentFile.name);
	var reader = new FileReader();
	reader.onloadend = function(evt){
		//Sets the file contents to the text area
		//We use val() instead of html() or innerHTML because
		//it's consistent between transitions to other files
		$('#fileContents').val(evt.target.result);
	};
	//This call will evoke the onloaded callback above
	reader.readAsText(currentFile);
}

/**
 * Creates a new file in the UserFiles directory
 */
function createFile()
{
	userFilesDir.createReader().readEntries(
		// Success callback.
		function(entries)
		{
			var defaultNewFileName =  "NewFile";
			var count = 1;
			var newFileName;
			var conflict;
			do
			{
				conflict = false;
				newFileName = defaultNewFileName + "_" + count;
				count++;
				for(var i = 0; i < entries.length; i++)
				{
					if(entries[i].name == newFileName)
					{
						conflict = true;
						break;
					}
				}
			}
			while(conflict);

			userFilesDir.getFile(
				newFileName,
				{create: true, exclusive: true},
				function(){
					refreshFiles();
				},
				function()
				{
					alert("error");
				});
		},
		// Error callback.
		function(error)
		{
			alert("error");
		});
}

/**
 * Deletes the file that is currently being edited
 */
function deleteFile()
{
	if(confirm("Delete " + currentFile.name + "?") == true)
	{
		currentFile.remove();
		//Go back to the file list
		jQT.goBack();
	}
}

/**
 * Saves the new contents of the file currently being edited
 */
function saveCurrentFile()
{
	// We are lazy here and do not handle success/error
	// callbacks from write and truncate.
	var text = $('#fileContents').val();
	// Write file.
	currentFile.createWriter(
		function(writer){
			writer.seek(0);
			writer.write(text);
		},
		function(error){
			alert("Cannot save file");
		});
	// Need to truncate to remove old file data
	// in case file is shorter after write.
	currentFile.createWriter(
		function(writer){
			writer.truncate(text.length);
		},
		function(error){
			alert("Cannot save file");
		});
}

/**
 * Renames the file that is currently being edited
 */
function renameCurrentFile()
{
	var newFileName = $('#fileName').val();
	if(newFileName != currentFile.name)
	{
		currentFile.moveTo(userFilesDir, newFileName,
			function(){
				$('#filePanelTitle').html(newFileName);
			},
			function(){
				alert("error");
			});
	}
}