var userFilesDir; //DirectoryEntry for the UserFiles directory
var textChanged = false; //Flag to check if there are changes to be saved
var currentFile; //The file that is being edited

/**
 * This function initializes the file manager
 */
function initFileManager()
{
	window.requestFileSystem(LocalFileSystem.PERSISTENT, 0, function(fileSystem)
	{
		//"root" is actually the local file storage directory for our app
		fileSystem.root.getDirectory("UserFiles",
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
	function(error)
	{
		alert("fs error");
	});

	$('#newfileNameContainer').hide();
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
	$('#filePanelTitle').html(file.name);
	currentFile = file;
	var reader = new FileReader();
	reader.onloadend = function(evt){
		//Sets the file contents to the text area
		//We use val() instead of html() or innerHTML because
		//it's consistent between transitions to other files
		$('#fileContents').val(evt.target.result);
	};
	//This call will evoke the onloaded callback above
	reader.readAsText(file);
}

/**
 * Creates a new file in the UserFiles directory
 */
function createFile()
{
	$('#newFileNameContainer').show();
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
 * Keep track of file text being changed in order to save it later
 */
function textHasChanged()
{
	textChanged = true;
}

/**
 * Checks if there have been changes to the file after the textarea loses focus and saves them if needed
 */
function textLostFocus()
{
	if(textChanged == true)
	{
		textChanged = false;
		currentFile.createWriter(function(writer){
			writer.seek(0);
			text = $('#fileContents').val();
			writer.write(text);
			writer.truncate(text.length);
		},
		function(error){
			alert("error");
		});
	}
}