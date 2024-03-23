# QTastList-Exemple
Exemple usage for QTaskList component repo.

# Requirements

## Libraries

- CMake 3.16
- Qt 6.2.2

## Platforms

Tested only for **windows 10 x64**.
Build tested only for **msvc community 2022**.
Code only tested with **VC 17**.

## Other

- A Zou instance to query 

# Install

- Create a directory for the project `mkdir QTaskListProject` and `cd QTaskListProject`.
- Clone the modules and its submodule : `git clone https://github.com/Elsombrerobot/QTaskList-Exemple --recurse-submodules`
- `cd QTaskList-Exemple`
- Modify QT_PATH in `make.bat`
- Run `make.bat` with `make`, it will create an out of source build in `build_windows.`
- Open `build_windows/QTaskListExemple.sln` with MSVC
- Find the `INSTALL` project in the solution explorer, **right click** and **build**, it will output the executable in `bin/QTaskListExemple/bin/`. 
- Execute `QTaskListExemple.exe`.

`bin/QTaskListExemple` is the standalone app folder.

# Todo
* [ ] Specify dependencies versions.
* [ ] Link all the frameworks zou, qt ect...