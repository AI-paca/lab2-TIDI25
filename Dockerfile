FROM python:3.11-bookworm

ENV DEBIAN_FRONTEND=noninteractive \
    PIP_DISABLE_PIP_VERSION_CHECK=1 \
    PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1

# Build deps (pybind11) + runtime deps (pygame) + VNC/noVNC for cross-platform GUI in browser
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake g++ make \
    pybind11-dev \
    tigervnc-standalone-server tigervnc-tools \
    novnc websockify \
    xauth \
    fluxbox xterm x11-xserver-utils \
    fonts-dejavu-core \
    libgl1 libglib2.0-0 libsm6 libxrender1 libxext6 \
    libx11-6 libxfixes3 libxi6 libxrandr2 libxcursor1 libxinerama1 libxxf86vm1 \
    libasound2 \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app

RUN python -m pip install --no-cache-dir pygame pybind11

COPY . /app/

# Build the pybind11 module
RUN python /app/build.py

RUN chmod +x /app/entrypoint.sh
RUN chmod +x /app/vnc/xstartup

EXPOSE 6080

ENTRYPOINT ["/app/entrypoint.sh"]
