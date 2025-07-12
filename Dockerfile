# Docker configuration for BAS (Browser Automation Studio)
# Using Wine to run Windows application on Linux

FROM ubuntu:22.04 AS base

# Install Wine and dependencies
RUN apt-get update && apt-get install -y \
    wine64 \
    winetricks \
    xvfb \
    wget \
    curl \
    qt5-qmake \
    qtbase5-dev \
    qtbase5-dev-tools \
    build-essential \
    nodejs \
    npm \
    && rm -rf /var/lib/apt/lists/*

# Set up Wine environment
ENV WINEPREFIX=/root/.wine
ENV DISPLAY=:99
ENV WINEARCH=win64

# Initialize Wine
RUN wine64 wineboot --init

FROM base AS builder

# Copy source code
COPY . /app
WORKDIR /app

# Set up BAS environment variables
ENV BAS_PATH=/app
ENV BAS_PATH_WORKER=/app/ChromeWorker

# Install Node.js dependencies for web server
RUN npm install express

FROM base AS runtime

# Copy built applications and source
COPY --from=builder /app /app
WORKDIR /app

# Expose WebSocket port range and web server port
EXPOSE 10000-20000
EXPOSE 8080

# Create startup script
COPY docker-entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
