#!/bin/bash

echo "Starting BAS Docker container..."

Xvfb :99 -screen 0 1024x768x16 &
XVFB_PID=$!

sleep 3

cd /app

echo "Starting Node.js web server..."
node docker-web-server.js &
WEB_SERVER_PID=$!

sleep 2

echo "Web server status check..."
if ps -p $WEB_SERVER_PID > /dev/null; then
    echo "✓ Web server started successfully"
    echo "Web Interface available at:"
    echo "- Main: http://localhost:8080"
    echo "- Toolbox: http://localhost:8080/toolbox"
    echo "- Scenario Editor: http://localhost:8080/scenario" 
    echo "- Settings: http://localhost:8080/central"
    echo "- Scheduler: http://localhost:8080/scheduler"
else
    echo "✗ Web server failed to start"
fi

if [ -f "/app/Studio/BrowserAutomationStudio.exe" ]; then
    echo "Starting BAS Studio..."
    cd /app/Studio
    wine64 BrowserAutomationStudio.exe &
    BAS_STUDIO_PID=$!
    echo "BAS Studio PID: $BAS_STUDIO_PID"
fi

if [ -f "/app/WebInterfaceBrowser/WebInterfaceBrowser.exe" ]; then
    echo "Starting WebInterfaceBrowser..."
    cd /app/WebInterfaceBrowser  
    wine64 WebInterfaceBrowser.exe &
    WEB_INTERFACE_PID=$!
    echo "WebInterfaceBrowser PID: $WEB_INTERFACE_PID"
fi

echo ""
echo "Container startup complete!"
echo "WebSocket server will start on random port 10000-20000 when BAS applications launch."
echo "Check container logs for WebSocket port information."

cleanup() {
    echo "Shutting down services..."
    kill $XVFB_PID $WEB_SERVER_PID 2>/dev/null
    exit 0
}

trap cleanup SIGTERM SIGINT

tail -f /dev/null
