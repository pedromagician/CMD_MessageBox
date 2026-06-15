# MessageBox

Command‑line MessageBox utility for Windows.

## 📦 Overview

MessageBox is a lightweight Windows command‑line tool that displays a customizable message box dialog.
Perfect for scripts, automation, installers, batch workflows, or any situation where you need a GUI popup from the command line.

## ✨ Features

- Custom title and message text
- Unicode support (\u{XXXX})
- Customizable icons
- Multiple button layouts
- Custom button labels
- Default button selection
- Monitor targeting
- Window positioning
- Optional Windows‑style return codes
- Quiet mode

## 🚀 Usage

```CMD
MessageBox.exe -t Title -m Message
```

## 🛠️ Command‑line Options

| Switch | Description |
|--------|-------------|
| `-title`, `-t` | Sets the window title. |
| `-message`, ` -m` | Sets the dialog message. Supports \n for new lines and Unicode via \u{XXXX}. |
| `-icon`, `-i` | Available values: noicon, information, question, warning, error |
| `-button`, `-b` | Available values: ok (1), okcancel, yesno (2), yesnocancel (3) |
| `-b1`, `-b2`, `-b3` | Custom Button Labels |
| `-default`, `-d` | Selects which button is focused by default (1–3). |
| `-monitor`, `-mon` | Values: Primary, Mouse, MousePointer, 0, 1, 2, n (monitor index) |
| `-position`, `-pos` | Values: center, top, bottom, left, right, pointer, xy |
| `-x`, `-y` | Offset in pixels |
| `-windowsReturnCode`, `-wrc` | Enables standard Windows MessageBox return codes. |
| `-quiet` | Suppresses console output. |
| `xxxxxxx` | xxxxxxxxxxxxxxxx |

## 📘 Examples

```CMD
MessageBox.exe -t "Hello" -m "This is a message."

MessageBox.exe -t "Info" -m "Line 1\nLine 2\nLine 3"
```

## 🧩 Batch Script Example

Store the selected value into a variable:

```CMD
@echo|set /p="@set mytmp=">tmp.bat
@messagebox -message "yes or no" -button yesno>>tmp.bat
@echo errorlevel=%errorlevel%
@call tmp.bat
@echo output=%mytmp%
```

Or capture the output directly:

```CMD
@set "FILE="
@for /f "usebackq delims=" %%A in (`messagebox -message "yes or no" -button yesno`) do @set "output=%%A"
@echo output: %output%
```

## 📜 License

Free to use. Amiga Rulez!
