/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file filenmaneger.js
 *
 * Functions used by the File Storage demo.
 */

var userFilesDir; // DirectoryEntry for the UserFiles directory.
var currentFile; // The file that is being edited.

/**
 * This function initializes the file manager.
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
					refreshFiles();
				},
				function(error)
				{
					alert("initFileManager1 error: " + error);
				});
		},
		// Error callback.
		function(error)
		{
			alert("initFileManager2 error: " + error);
		});
}

/**
 * This function refreshes the file list in the file manager panel.
 */
function refreshFiles()
{
	// Clear the file list.
	$('#fileList > li').remove();

	// Read files.
	var dirReader = userFilesDir.createReader();
	dirReader.readEntries(function(entries)
	{
		for(var i = 0; i < entries.length; i++)
		{
			// Create a nested <li><a> structure for each file returned.
			var newListElementTag = document.createElement("li");
			var newActhorTag = document.createElement("a");

			// Store the file data to the <a> element.
			$(newActhorTag).data('file', entries[i]);

			// Set the <a> element to call the setFile function with the
			// stored file when clicked.
			newActhorTag.setAttribute("onclick", "setFile($(this).data('file'))");
			newActhorTag.innerHTML = entries[i].name;
			newListElementTag.appendChild(newActhorTag);
			$('#fileList').append(newListElementTag);
		}
	},
	function(error)
	{
		alert("refreshFiles error: " + error);
	});
}

/**
 * Sets the file editing panel to the selected file.
 * @param file FilEntry with the info about the selected file.
 */
function setFile(file)
{
	// We programmatically go to the file editing panel.
	jQT.goTo('#filePage','pop');
	currentFile = file;

	$('#filePanelTitle').html(currentFile.name);
	$('#fileName').val(currentFile.name);
	var reader = new FileReader();
	reader.onloadend = function(evt){
		// Sets the file contents to the text area.
		// We use val() instead of html() or innerHTML because
		// it's consistent between transitions to other files.
		$('#fileContents').val(evt.target.result);
	};

	// This call will invoke the onloaded callback above.
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
					alert("createFile error: " + error);
				});
		},
		// Error callback.
		function(error)
		{
			alert("createFile error: " + error);
		});
}

/**
 * Deletes the file that is currently being edited.
 */
function deleteFile()
{
	if (confirm("Delete " + currentFile.name + "?") == true)
	{
		currentFile.remove();
		refreshFiles();
		// Go back to the file list.
		jQT.goBack();
	}
}

/**
 * Saves the new contents of the file currently being edited.
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
			alert("saveCurrentFile1 error: " + error);
		});
	// Need to truncate to remove old file data
	// in case file is shorter after write.
	currentFile.createWriter(
		function(writer){
			writer.truncate(text.length);
		},
		function(error){
			alert("saveCurrentFile2 error: " + error);
		});
}

/**
 * Renames the file that is currently being edited.
 */
function renameCurrentFile()
{
	var newFileName = $('#fileName').val();
	if(newFileName != currentFile.name)
	{
		currentFile.moveTo(userFilesDir, newFileName,
			function(){
				$('#filePanelTitle').html(newFileName);
				//Reopening the file, because the move action closes it on some platforms
				userFilesDir.getFile(newFileName,{create: false},
						function(file)
						{
							currentFile = file;
							refreshFiles();
						},
						function(error)
						{
							alert("renameCurrentFile1 error: " + error);
						});
			},
			function(error){
				alert("renameCurrentFile2 error: " + error);
			});
	}
}
