@echo off
set /p id="Enter name: "
cd C:\cgame\
@echo on
git add .
git commit -m "%id%"
git push -u origin main
@echo off
echo Press any key to Continue.
pause > nul