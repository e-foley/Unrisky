@echo off
:do_the_thing
python pull_new_json.py
..\x64\Release\Unrisky.exe "all.json" 10

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