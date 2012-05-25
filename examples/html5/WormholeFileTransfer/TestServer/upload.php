<?php
ini_set("display_errors", "1");
ini_set("log_errors", "1");
error_reporting(-1);

$target_path = "upload/";

$target_path = $target_path . basename( $_FILES["file"]["name"]);

echo "Target file: " . $target_path;
echo "<br/>";

print_r($_FILES);
echo "<br/>";

$success = move_uploaded_file($_FILES["file"]["tmp_name"], $target_path);
if ($success)
{
    echo "The file " . basename( $_FILES["file"]["name"]).
    " has been uploaded";
}
else
{
    echo "There was an error uploading the file, please try again!";
}

/*
if ((($_FILES["file"]["type"] == "image/gif")
|| ($_FILES["file"]["type"] == "image/jpeg")
|| ($_FILES["file"]["type"] == "image/pjpeg"))
&& ($_FILES["file"]["size"] < 20000))
  {
  if ($_FILES["file"]["error"] > 0)
    {
    echo "Return Code: " . $_FILES["file"]["error"] . "<br />";
    }
  else
    {
    echo "Upload: " . $_FILES["file"]["name"] . "<br />";
    echo "Type: " . $_FILES["file"]["type"] . "<br />";
    echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
    echo "Temp file: " . $_FILES["file"]["tmp_name"] . "<br />";

    if (file_exists("upload/" . $_FILES["file"]["name"]))
      {
      echo $_FILES["file"]["name"] . " already exists. ";
      }
    else
      {
      move_uploaded_file($_FILES["file"]["tmp_name"],
      "upload/" . $_FILES["file"]["name"]);
      echo "Stored in: " . "upload/" . $_FILES["file"]["name"];
      }
    }
  }
else
  {
  echo "Invalid file";
  }
 */
?>
