<?php
ini_set("display_errors", "1");
ini_set("log_errors", "1");
error_reporting(-1);

function mylog($message)
{
	$file = 'mylog.txt';
	// Open the file to get existing content
	$current = file_get_contents($file);
	// Append a line to the file
	$current .= "$message\n";
	// Write the contents back to the file
	file_put_contents($file, $current);
}

function uploadPhoto()
{
	$targetPath = "photos/" . basename($_FILES["file"]["name"]);

	if (fileIsOK($_FILES["file"]))
	{
		$success = move_uploaded_file($_FILES["file"]["tmp_name"], $targetPath);
		if ($success)
		{
			echo "The file " . basename($_FILES["file"]["name"]) .
			" has been uploaded";
		}
		else
		{
			echo "There was an error uploading the file";
		}
	}
	else
	{
		echo "Not a valid image file";
	}
}

function fileIsOK($file)
{
	return $file["size"] < 1500000
		&& fileIsImage($file["name"]);
}

function fileIsImage($path)
{
	return endsWith($path, ".jpeg")
		|| endsWith($path, ".jpg")
		|| endsWith($path, ".png");
}

function endsWith($haystack, $needle)
{
	$expectedPosition = strlen($haystack) - strlen($needle);
	return strripos($haystack, $needle, 0) === $expectedPosition;
}

function getPhotoURLs()
{
	/*// For debugging.
	$urls = getLastTenPhotoURLs();
	foreach ($urls as $url)
	{
		echo "<a href='$url'>$url</a><br/>\n";
	}*/

	echo getPhotoURLsAsJSON();
}

function getPhotoURLsAsJSON()
{
	$urls = getLastTenPhotoURLs();

	return json_encode($urls);
}

// Gets last TEN photo urls.
function getLastTenPhotoURLs()
{
	$baseURL = "http://dev.mosync.com/mobilelua/PhotoGallery/photos/";
	$baseDir = "photos/";

	$files = array();

	// Add files to table using last mod time as key.
	if ($handle = opendir($baseDir))
	{
		while (false !== ($file = readdir($handle)))
		{
			if ("." != $file && ".." != $file)
			{
				$files[filemtime($baseDir . $file)] = $file;
			}
		}

		closedir($handle);
	}

	// Sort by mod time.
	ksort($files);

	// Last ones first.
	$files = array_reverse($files);

	// List if URLs.
	$urls = array();

	// Create a list of URLs to the most recent files.
	$fileCounter = 0;
	foreach ($files as $file)
	{
		++$fileCounter;
		// We only get TEN recent files (100 is too many!).
		if ($fileCounter > 10)
		{
			// TODO: Move excessive files to an archive directory?
			break;
		}
		array_push($urls, $baseURL . $file);
	}

	return $urls;
}

// If file data is posted then upload the file,
// else get the list of image urls.
if (isset($_FILES["file"]))
{
	uploadPhoto();
}
else
{
	getPhotoURLs();
}
?>
