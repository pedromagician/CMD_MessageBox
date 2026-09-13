@echo off
setlocal EnableDelayedExpansion
chcp 65001 >nul
title MessageBox - manualny test

rem =============================================================
rem  MessageBox.exe - kompletny manualny testovaci beh
rem =============================================================
rem  Ako to funguje:
rem   1. Pre kazdy test sa najprv vypise, co sa ma objavit / stat.
rem   2. Stlacenim ENTER sa test spusti (spusti sa MessageBox.exe
rem      s danymi parametrami).
rem   3. Over dialog / konzolu podla popisu a zavri ho.
rem   4. Potvrd vysledok: Y = spravne, N = chyba, S = preskocit.
rem   5. Vysledky sa priebezne zapisuju do test-results.txt.
rem
rem  Pouzitie:
rem   test-manual.bat                 - najde exe sam (Release/Debug)
rem   test-manual.bat "cesta\k.exe"   - pouzije zadanu cestu k exe
rem =============================================================

set "SCRIPT_DIR=%~dp0"
set "EXE="

if not "%~1"=="" set "EXE=%~1"
if not defined EXE if exist "%SCRIPT_DIR%Release\MessageBox.exe" set "EXE=%SCRIPT_DIR%Release\MessageBox.exe"
if not defined EXE if exist "%SCRIPT_DIR%Debug\MessageBox.exe" set "EXE=%SCRIPT_DIR%Debug\MessageBox.exe"
if not defined EXE if exist "%SCRIPT_DIR%Bin\MessageBox.exe" set "EXE=%SCRIPT_DIR%Bin\MessageBox.exe"
if not defined EXE if exist "%SCRIPT_DIR%MessageBox.exe" set "EXE=%SCRIPT_DIR%MessageBox.exe"

if not defined EXE (
    echo [CHYBA] MessageBox.exe sa nenasiel v Release\, Debug\, Bin\ ani v priecinku skriptu.
    echo Zostav projekt vo Visual Studiu, alebo zadaj cestu k exe ako parameter:
    echo   %~nx0 "C:\cesta\MessageBox.exe"
    exit /b 1
)
for %%F in ("%EXE%") do set "EXE=%%~fF"

set "LOG=%SCRIPT_DIR%test-results.txt"

set /a TOTAL=0
set /a PASS=0
set /a FAIL=0
set /a SKIP=0

echo. >> "%LOG%"
echo ============================================================ >> "%LOG%"
echo Test beh: %DATE% %TIME% >> "%LOG%"
echo Exe: %EXE% >> "%LOG%"
echo ============================================================ >> "%LOG%"

cls
echo ============================================================
echo   MessageBox.exe - KOMPLETNY MANUALNY TEST
echo ============================================================
echo Testovany subor: %EXE%
echo.
echo Postup pre kazdy test:
echo   1. Precitaj si, co sa ma stat / objavit.
echo   2. Stlac ENTER - test sa spusti.
echo   3. Over dialog / konzolu podla popisu, potom ho zavri.
echo   4. Potvrd vysledok: Y = spravne, N = chyba, S = preskocit.
echo.
echo Vysledky sa priebezne zapisuju do: %LOG%
echo Kedykolvek mozes cely beh ukoncit cez CTRL+C.
echo ============================================================
pause

rem =============================================================
rem A. ZAKLADNE PARAMETRE A CHYBOVE STAVY
rem =============================================================
echo.
echo ############################################################
echo # A. ZAKLADNE PARAMETRE A CHYBOVE STAVY
echo ############################################################

set "ID=A1-help"
set "DESC=Vypise sa kompletny zoznam parametrov a priklad pouzitia do konzoly. Nema sa otvorit ziadne okno. Errorlevel ma byt 0."
call :runtest -help

set "ID=A2-basic"
set "DESC=Objavi sa okno s titulkom Test a textom Zakladny dialog, s jednym tlacidlom OK. Po kliknuti na OK ma byt errorlevel 1."
call :runtest -t Test -m "Zakladny dialog"

set "ID=A3-missing-message"
set "DESC=V konzole sa vypise Error - message is empty. Ziadne okno sa neotvori, errorlevel 0."
call :runtest -t BezSpravy -m ""

set "ID=A4-unknown-param"
set "DESC=V konzole sa vypise Unknown parameter a strucna sprava Run with -help for usage information - NIE cela napoveda. Ziadne okno sa neotvori."
call :runtest -neexistujuciParameter hodnota

set "ID=A5-invalid-enum"
set "DESC=V konzole sa vypise Invalid enum value a strucna sprava Run with -help for usage information - NIE cela napoveda. Ziadne okno sa neotvori."
call :runtest -m "test" -button 9

rem =============================================================
rem B. TEXT A UNIKOD
rem =============================================================
echo.
echo ############################################################
echo # B. TEXT A UNIKOD
echo ############################################################

set "ID=B1-multiline"
set "DESC=Text sa zobrazi na troch riadkoch pod sebou, nie na jednom riadku."
call :runtest -m "Riadok jedna\nRiadok dva\nRiadok tri"

set "ID=B2-unicode"
set "DESC=Za slovom Hotovo sa zobrazi emoji palec hore."
call :runtest -m "Hotovo \u{1F44D}"

rem =============================================================
rem C. IKONY
rem =============================================================
echo.
echo ############################################################
echo # C. IKONY
echo ############################################################

set "ID=C1-icon-information"
set "DESC=Zobrazi sa systemova ikona Informacia - modre i."
call :runtest -m "Informacna sprava" -icon Information

set "ID=C2-icon-question"
set "DESC=Zobrazi sa systemova ikona Otazka."
call :runtest -m "Otazna sprava" -icon Question

set "ID=C3-icon-warning"
set "DESC=Zobrazi sa systemova ikona Varovanie - zlty vykricnik."
call :runtest -m "Varovna sprava" -icon Warning

set "ID=C4-icon-error"
set "DESC=Zobrazi sa systemova ikona Chyba - cerveny krizik."
call :runtest -m "Chybova sprava" -icon Error

rem =============================================================
rem D. TLACIDLA
rem =============================================================
echo.
echo ############################################################
echo # D. TLACIDLA
echo ############################################################

set "ID=D1-button-okcancel"
set "DESC=Zobrazia sa dve tlacidla OK a Storno. Klikni OK - errorlevel ma byt 1."
call :runtest -m "OK / Storno" -button OkCancel

set "ID=D2-button-yesno"
set "DESC=Zobrazia sa dve tlacidla Ano a Nie. Klikni Ano - errorlevel ma byt 1."
call :runtest -m "Ano / Nie" -button YesNo

set "ID=D3-button-yesnocancel"
set "DESC=Zobrazia sa tri tlacidla Ano, Nie, Storno. Klikni Nie - errorlevel ma byt 2."
call :runtest -m "Ano / Nie / Storno" -button YesNoCancel

set "ID=D4-custom-labels"
set "DESC=Tlacidla YesNo maju vlastne texty Suhlasim / Nesuhlasim namiesto Ano/Nie."
call :runtest -m "Vlastne texty tlacidiel" -button YesNo -b1 Suhlasim -b2 Nesuhlasim

set "ID=D5-default-button"
set "DESC=Pri otvoreni je fokusovane druhe tlacidlo - Nie. Stlac ENTER bez klikania mysou - errorlevel ma byt 2."
call :runtest -m "Fokus na druhom tlacidle" -button YesNo -default 2

set "ID=D6-windows-return-code"
set "DESC=Klikni na Ano. V konzole sa vypise cislo vysledku - ma byt 6, teda IDYES, nie 1."
call :runtest -m "Klikni na Ano" -button YesNo -wrc

set "ID=D7-quiet"
set "DESC=Po zatvoreni okna sa do konzoly NEMA vypisat ziadne cislo vysledku - bez -quiet by sa vypisalo."
call :runtest -m "Po kliknuti sa nic nevypise do konzoly" -quiet

rem =============================================================
rem E. POZICOVANIE A MONITORY
rem =============================================================
echo.
echo ############################################################
echo # E. POZICOVANIE A MONITORY
echo ############################################################

set "ID=E1-position-top"
set "DESC=Okno sa zobrazi pri hornom okraji obrazovky, vodorovne v strede."
call :runtest -m "Hore v strede obrazovky" -pos top

set "ID=E2-position-bottom"
set "DESC=Okno sa zobrazi pri spodnom okraji obrazovky, vodorovne v strede."
call :runtest -m "Dole v strede obrazovky" -pos bottom

set "ID=E3-position-xy-offset"
set "DESC=Okno sa zobrazi 100 px od laveho a 100 px od horneho okraja obrazovky."
call :runtest -m "Posunute o 100,100 od laveho horneho rohu" -pos xy -x 100 -y 100

set "ID=E4-monitor-invalid"
set "DESC=Program ma korektne spracovat neplatny monitor - fallback alebo chybova hlaska, nie spadnut."
call :runtest -mon abc -m "Neplatny monitor"

set "ID=E5-monitor-primary"
set "DESC=Okno sa zobrazi v strede primarneho monitora."
call :runtest -mon primary -pos center -m "Na primarnom monitore"

goto :summary

rem =============================================================
rem Pomocna rutina: spusti jeden test podla premennych ID a DESC,
rem argumenty pre MessageBox.exe su prevzate z parametrov call-u.
rem =============================================================
:runtest
set ARGS=%*
set /a TOTAL+=1
cls
echo ============================================================
echo TEST !ID!   (%TOTAL%. v poradi)
echo ------------------------------------------------------------
echo Ocakavane:
echo   !DESC!
echo.
echo Prikaz: "%EXE%" !ARGS!
echo ============================================================
echo Stlac ENTER pre spustenie testu...
pause >nul

"%EXE%" !ARGS!
set "RC=%ERRORLEVEL%"

echo.
echo Dialog sa zatvoril. Navratovy kod (errorlevel): %RC%
choice /c YNS /n /m "Zodpoveda vysledok popisu vyssie? [Y]ano [N]ie [S]kip: "
if errorlevel 3 goto :runtest_skip
if errorlevel 2 goto :runtest_fail

echo [ OK ] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a PASS+=1
goto :runtest_done

:runtest_fail
echo [FAIL] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a FAIL+=1
goto :runtest_done

:runtest_skip
echo [SKIP] !ID! - !DESC! ^| errorlevel=!RC! >> "%LOG%"
set /a SKIP+=1

:runtest_done
exit /b 0

:summary
echo.
echo ============================================================
echo VYSLEDOK: %PASS% OK, %FAIL% chyba, %SKIP% preskocenych, spolu %TOTAL%
echo Podrobny log: %LOG%
echo ============================================================
echo Zhrnutie: %PASS% OK, %FAIL% chyba, %SKIP% preskocenych, spolu %TOTAL% >> "%LOG%"
endlocal
pause
exit /b 0
