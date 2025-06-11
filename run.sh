#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

BUILD=false
USE_DOCKER=false
CLIENT_PORT=
DISPLAY_PORT=
PROCESS_PORT=
DISPLAY_ADDR="localhost:8765"
ACTION_CLIENT=false
ACTION_DISPLAY=false
ACTION_PROCESS=false

DOCKER_DISPLAY_PORT=8765
DOCKER_PROCESS_PORT=8766

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -b|--build) BUILD=true ;;
        --dockered) USE_DOCKER=true ;;
        --client)
            ACTION_CLIENT=true
            CLIENT_PORT="$2"
            shift
            ;;
        --display)
            ACTION_DISPLAY=true
            DISPLAY_PORT="$2"
            shift
            ;;
        --process)
            ACTION_PROCESS=true
            PROCESS_PORT="$2"
            shift
            ;;
        --display-addr)
            DISPLAY_ADDR="$2"
            shift
            ;;
        --display-port)
            DOCKER_DISPLAY_PORT="$2"
            shift
            ;;
        --process-port)
            DOCKER_PROCESS_PORT="$2"
            shift
            ;;
        -h|--help)
            echo -e "${GREEN}Usage:${NC}"
            echo "run.sh [options]"
            echo ""
            echo "Options:"
            echo "  -b, --build                        Build all targets before running"
            echo "  --dockered                         Run using docker-compose with default ports from .env"
            echo "  --client <ip:port>                 Run client and connect to processing server on ip:port"
            echo "  --display <port>                   Run display server locally on port"
            echo "  --process <port>                   Run processing server locally on port"
            echo "  --display-addr <ip:port>           Set display server address for processing server"
            echo "  --display-port <port>              Set display-server port in docker-compose (default: 8765)"
            echo "  --process-port <port>              Set processing-server port in docker-compose (default: 8766)"
            echo "  -h, --help                         Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown parameter: $1${NC}"
            exit 1
            ;;
    esac
    shift
done

if [ "$BUILD" = true ]; then
    echo -e "${GREEN}Building project...${NC}"
    mkdir -p build && cd build || exit
    cmake ..
    make
    cd .. || exit
fi

if [ "$USE_DOCKER" = true ]; then
    echo -e "${GREEN}Starting services via Docker...${NC}"

    cat > .env << EOF
DOCKER_DISPLAY_PORT=$DOCKER_DISPLAY_PORT
DOCKER_PROCESS_PORT=$DOCKER_PROCESS_PORT
EOF

    docker-compose down --remove-orphans > /dev/null 2>&1
    docker-compose up
    exit 0
fi

if [ "$ACTION_DISPLAY" = true ]; then
    if [ -z "$DISPLAY_PORT" ]; then
        echo -e "${RED}Error: --display requires a port${NC}"
        exit 1
    fi
    echo -e "${GREEN}Starting display server on port $DISPLAY_PORT${NC}"
    ./build/display_server "$DISPLAY_PORT" &
fi

if [ "$ACTION_PROCESS" = true ]; then
    if [ -z "$PROCESS_PORT" ]; then
        echo -e "${RED}Error: --process requires a port${NC}"
        exit 1
    fi
    echo -e "${GREEN}Starting processing server on port $PROCESS_PORT, display at $DISPLAY_ADDR${NC}"
    IP="${DISPLAY_ADDR%%:*}"
    PORT="${DISPLAY_ADDR##*:}"
    ./build/processing_server "$PROCESS_PORT" "$IP" "$PORT" &
fi

if [ "$ACTION_CLIENT" = true ]; then
    if [ -z "$CLIENT_PORT" ]; then
        echo -e "${RED}Error: --client requires an argument in format 'host:port'${NC}"
        exit 1
    fi

    if [[ "$CLIENT_PORT" == *:* ]]; then
        HOST="${CLIENT_PORT%%:*}"
        PORT="${CLIENT_PORT##*:}"
    else
        echo -e "${RED}Error: invalid format for --client. Expected 'host:port'${NC}"
        exit 1
    fi

    echo -e "${GREEN}Starting client connected to $HOST:$PORT${NC}"
    ./build/client "$HOST" "$PORT"
fi

wait
