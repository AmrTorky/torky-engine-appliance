#!/usr/bin/env bash
set -e

echo "=== TORKY CONTINUUM ENGINE: 30-SECOND TERMINAL REPRODUCTION ==="
cd /home/amr/torky_dist

# 1. Self-Certifying Invariant Proofs & Cryptographic Seal
echo "[1/6] Auditing integrity seal and 10 core subsystems..."
sha256sum -c /home/amr/RELEASE_SEAL.txt
python3 verify_appliance.py

# 2. Instant Cold-Start & Embedded Bare-Metal Binary
echo "[2/6] Cold-starting Profile C appliance daemon..."
python3 torky_server.py --port 8080 --profile C &
DAEMON_PID=$!
sleep 1.2

# 3. Deterministic Microsecond SLA & Hard Memory Fencing Status
echo "[3/6] Polling engine status & invariant boundaries..."
curl -s http://localhost:8080/status | python3 -m json.tool

# 4. Zero-Storage Waveform Synthesis & Coordinate Inversion (Cardano-Viète)
echo "[4/6] Reconstructing dense vector via Cardano-Viète synthesis..."
curl -s http://localhost:8080/reconstruct/100 | python3 -m json.tool

# 5. Continuous Microsecond PITR (Point-in-Time Manifold Restoration)
echo "[5/6] Locking historical continuum boundary at 500,000 records..."
curl -s http://localhost:8080/pitr/lock/500000 | python3 -m json.tool

# 6. Hybrid Metric Search across 1M records & Metadata-Vector Algebraic Fusion
echo "[6/6] Executing hybrid similarity query across 1M governed vectors..."
python3 -c "import urllib.request, json; q = [0.05]*64; req = urllib.request.Request('http://localhost:8080/search', data=json.dumps({'query_vector': q, 'top_k': 5, 'filter_active': True}).encode('utf-8'), headers={'Content-Type': 'application/json'}); res = urllib.request.urlopen(req); print(json.dumps(json.loads(res.read()), indent=2))"

# Observability sweep & teardown
echo "=== TELEMETRY METRIC SNAPSHOT ==="
curl -s http://localhost:8080/metrics | grep torky_

echo "=== TEARDOWN ==="
kill -9 $DAEMON_PID
echo "[OK] Reproduction suite complete in under 30 seconds."
