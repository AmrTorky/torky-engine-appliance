FROM ubuntu:22.04 AS builder
WORKDIR /build
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    libgomp1 \
    && rm -rf /var/lib/apt/lists/*
COPY torky_engine.cpp .
RUN g++ -O3 -std=c++17 -fopenmp -fPIC -shared torky_engine.cpp -o libtorky_core.so && strip libtorky_core.so

FROM python:3.10-slim
WORKDIR /app
RUN apt-get update && apt-get install -y --no-install-recommends \
    libgomp1 \
    curl \
    && rm -rf /var/lib/apt/lists/*
RUN pip install --no-cache-dir numpy psutil

COPY --from=builder /build/libtorky_core.so /app/libtorky_core.so
COPY torky_engine.py /app/torky_engine.py
COPY torky_tools.py /app/torky_tools.py
COPY torky_impossible_tools.py /app/torky_impossible_tools.py
COPY torky_server.py /app/torky_server.py
COPY torky_cli.py /app/torky_cli.py
COPY dashboard.html /app/dashboard.html
COPY stress_1M.torky /app/stress_1M.torky

EXPOSE 8080
HEALTHCHECK --interval=5s --timeout=3s --retries=3 \
  CMD curl -f http://localhost:8080/status || exit 1

CMD ["python3", "torky_server.py"]
