# projectmanager

### Description

A project manager for c++, create project and class easily.

### Software Architecture

1. Linux_x86_64
2. Linux_arm

### Installation

Mannully: 

```bash
#!/bin/bash
cd
rm -rf ./projectmanager
git clone https://github.com/Dionysen/ProjectManager.git
cd projectmanager/build
rm -rf ./*
cmake ..
make
sudo cp ./pm /usr/bin/pm
```

or Use install script:

```shell
git clone https://github.com/Dionysen/ProjectManager.git
cd ProjectManager/tools
chmod +x ./install.sh
./install.sh
```

Tips: You even can download the "install.sh" and just run it.  

### Usage

Use `pm --[args]` or `pm -[args]`:

`[args]` should be a path or a project.

| long arg          | arg    | do                         |
| ----------------- | ------ | -------------------------- |
| `--list`          | `-l`   | show the whole inforamtion |
| `--createproject` | `-c`   | create project             |
| `--delproject`    | `-d`   | delete a prject            |
| `--addclass`      | `-a`   | add class                  |
| `--delclass`      | `-D` | delete a class             |
| `--build`         | `-b`   | build without run          |
|`--make`|`-m`| build the project with makefile|
| `--run`           | `-r`   | build and run              |
| `--setproject`    | `-s`   | set the current project    |
| `--setpath`       | `-S` | set the project path       |
| `--help`          | `-h`   | show help information      |
