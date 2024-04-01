# QTastList-Exemple
Exemple usage for QTaskList component repo.
The application connect to a running kistu instance, to display user tasks.

# Screenshots
* [ ] Todo

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
- Create the executable : 
- Via **MSVC**
	- Open `build_windows/QTaskListExemple.sln` with MSVC
	- Find the `INSTALL` project in the solution explorer, **right click** and **build**.. 
- Via **cmd**
	- Run `install.bat` with `install`
- Execute `build_windows/bin/QTaskListExemple/bin/QTaskListExemple.exe`.

`build_windows/bin/QTaskListExemple` is the standalone app folder.

# Todo
* [ ] Specify dependencies versions.
* [ ] Upload executable.
* [ ] Tag versions, starting a first realease.
* [ ] Link all the frameworks zou, qt ect...