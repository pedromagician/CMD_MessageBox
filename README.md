# MessageBox
MessageBox for Windows command line.

![bench](https://raw.githubusercontent.com/pedromagician/CMD_MessageBox/main/pic/screenshot.png)


@echo|set /p="@set mytmp=">1.bat
@messagebox -message "yes or no" -button yesno>>1.bat
@echo errorlevel=%errorlevel%
@call 1.bat
@echo output=%mytmp%
