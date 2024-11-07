# MessageBox
MessageBox for Windows command line.

Redistributable x86 packages are required to run the program
https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170

![bench](https://raw.githubusercontent.com/pedromagician/CMD_MessageBox/main/pic/screenshot.png)


@echo|set /p="@set mytmp=">1.bat\
@messagebox -message "yes or no" -button yesno>>1.bat\
@echo errorlevel=%errorlevel%\
@call 1.bat\
@echo output=%mytmp%
