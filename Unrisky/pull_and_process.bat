@echo off
:do_the_thing
python pull_new_json.py
..\x64\Release\Unrisky.exe "C:\Users\Ed\Documents\SpiderOak Hive\Creations\Code\C++\Unrisky\Unrisky\all.json"

set resp=""
set /p resp="Check again? (y/N) "
if "%resp%" == "y" (
  echo.
  goto :do_the_thing
)
if "%resp%" == "Y" (
  echo.
  goto :do_the_thing
)