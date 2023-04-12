# projectmanager

### Description

A project manager for c++, create project and class easily.

### Software Architecture

1. Linux_x86_64
2. Linux_arm

### Installation

```bash
#!/bin/bash
cd
rm -rf ./projectmanager
git clone git@gitee.com:sential/projectmanager.git
cd projectmanager/build
rm -rf ./*
cmake ..
make
sudo cp ./pm /usr/bin/pm
```

### Usage

Use `pm --[args]` or `pm -[args]`:

| long arg          | arg    | do                         |
| ----------------- | ------ | -------------------------- |
| `--list`          | `-l`   | show the whole inforamtion |
| `--createproject` | `-c`   | create project             |
| `--delproject`    | `-d`   | delete a prject            |
| `--addclass`      | `-a`   | add class                  |
| `--delclass`      | `none` | delete a class             |
| `--build`         | `-b`   | build without run          |
| `--run`           | `-r`   | build and run              |
| `--setproject`    | `-s`   | set the current project    |
| `--setpath`       | `none` | set the project path       |
| `--help`          | `-h`   | show help information      |
