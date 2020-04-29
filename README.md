<p align="center">
<img src="doc/UNIZAP_LOGO_UGLY_.png" align="center" width="350" height="125"/> 
 </p>

# UNIZAP
This program will zip up a directory or list of files and push it to Dropbox.
It uses a config file, where you can specify what files to include or what directory to include.
See below for example and explanations of parameters

## OWL
The config file extension is .owl, which is a scripting language I tried to make a few years ago, but failed miserably!


## Config file 
### UPLOAD
```Java
ACCESS = "<ACCESS TOKEN>"
FILE_NAME = "unizap.zip"  // Can also include full path
DIR_PATH = "..\unizap"   // If you want to zip up a full directory
OUTPUT_PATH = "Versions" // Where to send the zip file, in dropbox

FILES {
  "C:\Users\default\desktop\cat.png"  //Extra files to include in the zip file.
}

FLAGS {
  INCLUDE_DIRECTORY    // In the DIR_PATH, if you want to include the 
                       // parent directory i.e. "unizap\file.txt" in zip file
  DISABLE_CONFIRM_CURL // The program will, by default, give the user file info 
                       // and output path etc. and give a confirm if all is ok, and then upload
  UPLOAD
}

 ```
### DOWNLOAD
```Java
ACCESS = "<ACCESS TOKEN>"
FILE_NAME = "unizap.zip" // Can also include full path
OUTPUT_PATH = "Versions" // Where to send the zip file, in dropbox, 
                         //if null, current path will be used
FLAGS {
  INCLUDE_DIRECTORY    // In the DIR_PATH, if you want to include the 
                       // parent directory i.e. "unizap\file.txt" in zip file
  DISABLE_CONFIRM_CURL // The program will, by default, give the user file info 
                       // and output path etc. and give a confirm if all is ok, and then upload
  DOWNLOAD
}

 ```
