[![Build](https://github.com/AstroAir/ARAS/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/AstroAir/ARAS/actions/workflows/build.yml)
[![CircleCI](https://circleci.com/gh/AstroAir-Develop-Team/ARAS/tree/master.svg?style=svg)](https://circleci.com/gh/AstroAir-Develop-Team/ARAS/tree/master)
[![CodeQL](https://github.com/AstroAir-Develop-Team/ARAS/actions/workflows/codeql-analysis.yml/badge.svg?branch=master)](https://github.com/AstroAir-Develop-Team/ARAS/actions/workflows/codeql-analysis.yml)

## AstroAir Server

<i> A New Generation of Astronomical Photography Tools </i>

A light but powerful tools kit

## Feature
  * Lightweight, can be deployed on smaller clients.
  * Powerful, can control most of the devices.
  * Opensource, you can modify according to your needs
  * Little dependencies, not much other things

## Building
### Install Pre-requisites
### On Debian/Ubuntu:
```
sudo apt-get install cmake git gcc g++ libwebsocketpp-dev libglfw3-dev libgl-dev liblua-dev
```
And you alse need to install all of the SDK.
```
sudo cp -R includes /usr/include
```
### Build
```
git clone https://github.com/AstroAir/ARAS
mkdir build
cmake -S . -B build
cmake --build build/
```
## Problem report
  * Join the official QQ group 710622107
  * Problem report at GitHub
  * Official email<astro_air@126.com>
  * Developer email<qwdmax@qq.com>
