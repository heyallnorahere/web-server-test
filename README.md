# web-server-test [![build](https://github.com/yodasoda1219/web-server-test/actions/workflows/build.yml/badge.svg)](https://github.com/yodasoda1219/web-server-test/actions/workflows/build.yml)
This is a little server-client program I'm working on. Current supported platforms:
- MacOS X
- Ubuntu (It's what I'm running on GitHub Actions)

## Dependencies
Dependencies required on all platforms:
- OpenSSL >= 1.1.1
- OpenGL >= 3.3
- CMake >= 3.15.4

Dependencies required on Unix systems:
- libasio
- libssh2

## Building
On supported platforms, there should be a script to generate build files. Once that has been run, simply build the project using the generated files.