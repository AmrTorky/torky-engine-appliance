#!/usr/bin/env bash
set -eo pipefail

ARCHIVE="torky_engine_peak_commercial_v1.tar.gz"
IMAGE_TAG="torky-engine-api:v1.0-commercial"
CONTAINER_NAME="torky-commercial-node"
PORT="8080"

echo "================================================================="
echo "   Torky Engine (AT-ACE Profile C) Enterprise Turnkey Installer  "
echo "================================================================="

# 1. التحقق من وجود الحزمة المستقلة
if [[ ! -f "$ARCHIVE" ]]; then
  echo "[-] Error: Archive '$ARCHIVE' not found in current directory."
  exit 1
fi

# 2. فحص متطلبات بيئة Docker
if ! command -v docker &> /dev/null; then
  echo "[-] Error: Docker engine is not installed or not in PATH."
  exit 1
fi

# 3. إيقاف أي نسخ نشطة سابقة
echo "[*] Cleaning up existing containers on port $PORT..."
docker rm -f "$CONTAINER_NAME" 2>/dev/null || true

# 4. استيراد صورة الحاوية التجارية
echo "[*] Ingesting enterprise appliance image ($ARCHIVE)..."
docker load < "$ARCHIVE"

# 5. تشغيل الحاوية بحدود ذاكرة آمنة
echo "[*] Launching container appliance '$CONTAINER_NAME' on port $PORT..."
docker run -d \
  --name "$CONTAINER_NAME" \
  --restart unless-stopped \
  -p "$PORT:8080" \
  --memory=512m \
  --cpus=2.0 \
  "$IMAGE_TAG"

# 6. انتظار اكتمال الجاهزية والتحقق الصحي
echo "[*] Awaiting microservice convergence..."
for i in {1..10}; do
  if curl -s "http://localhost:$PORT/status" | grep -q "HEALTHY_OPERATIONAL"; then
    echo "[+] Engine is fully converged and operational!"
    break
  fi
  sleep 1
done

# 7. اختبار سريع لنقاط النهاية الحيوية
echo -e "\n--- Operational Verification ---"
curl -s "http://localhost:$PORT/status" | grep -E "engine|governed_records|compression_efficiency|status"

echo -e "\n[+] Deployment successful. Web Dashboard accessible at: http://localhost:$PORT/dashboard.html"
echo "================================================================="
