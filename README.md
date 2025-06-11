# prereqs:
- cmake
- clang v18.1.3
- docker (if you will start server side in docker)

# usage:
`./run.sh -h` -- show help message

`./run.sh --dockered` -- start server side in docker with default ports

`./run.sh --client <ip>:<port>` -- start client connected to processing server on ip:dest
