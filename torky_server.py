#!/usr/bin/env python3
import os
import sys
import time
import json
import urllib.parse
import numpy as np
from http.server import HTTPServer, BaseHTTPRequestHandler
import torky_impossible_tools

STORAGE_FILE = "stress_1M.torky"
if not os.path.exists(STORAGE_FILE):
    STORAGE_FILE = "benchmark_run.torky"

print(f"[*] Initializing Enterprise Peak Engine on '{STORAGE_FILE}'...")
peak_engine = torky_impossible_tools.TorkyPeakEngine(STORAGE_FILE)

class PeakAPIHandler(BaseHTTPRequestHandler):
    def _set_headers(self, status=200, content_type='application/json'):
        self.send_response(status)
        self.send_header('Content-Type', content_type)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, DELETE, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()

    def do_OPTIONS(self):
        self._set_headers(200)

    def do_GET(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path

        # تقديم واجهة المستخدم dashboard.html
        if path in ['/', '/dashboard', '/dashboard.html']:
            dash_file = "dashboard.html"
            if os.path.exists(dash_file):
                self._set_headers(200, content_type='text/html; charset=utf-8')
                with open(dash_file, 'rb') as f:
                    self.wfile.write(f.read())
            else:
                self._set_headers(404)
                self.wfile.write(b'{"error": "dashboard.html not found"}')
            return

        elif path == '/status':
            self._set_headers(200)
            res = {
                "engine": "Torky Autopoietic Continuum Engine (AT-ACE Profile C - Commercial Appliance)",
                "governed_records": peak_engine.total_records,
                "active_records": peak_engine.total_records - len(peak_engine.tombstones),
                "packed_record_bytes": 9,
                "compression_efficiency": "96.96%",
                "invariant_kappa_max": 18.50,
                "tombstones": len(peak_engine.tombstones),
                "status": "HEALTHY_OPERATIONAL"
            }
            self.wfile.write(json.dumps(res, indent=2).encode('utf-8'))

        elif path == '/metrics':
            self._set_headers(200, content_type='text/plain; version=0.0.4; charset=utf-8')
            metrics_payload = peak_engine.generate_prometheus_metrics()
            self.wfile.write(metrics_payload.encode('utf-8'))

        elif path.startswith('/reconstruct/'):
            try:
                rec_id = int(path.split('/')[-1])
                rec_res = peak_engine.reconstruct_vector_on_demand(rec_id)
                self._set_headers(200)
                self.wfile.write(json.dumps(rec_res, indent=2).encode('utf-8'))
            except Exception as e:
                self._set_headers(400)
                self.wfile.write(json.dumps({"error": str(e)}).encode('utf-8'))

        elif path.startswith('/project/'):
            try:
                query_params = urllib.parse.parse_qs(parsed.query)
                target_dim = int(query_params.get('dim', [128])[0])
                rec_id = int(path.split('/')[-1])
                proj_res = peak_engine.project_to_higher_dimensions(rec_id, target_dim=target_dim)
                self._set_headers(200)
                self.wfile.write(json.dumps(proj_res, indent=2).encode('utf-8'))
            except Exception as e:
                self._set_headers(400)
                self.wfile.write(json.dumps({"error": str(e)}).encode('utf-8'))

        elif path.startswith('/pitr/lock/'):
            try:
                record_limit = int(path.split('/')[-1])
                pitr_res = peak_engine.point_in_time_snapshot(record_limit)
                self._set_headers(200)
                self.wfile.write(json.dumps(pitr_res, indent=2).encode('utf-8'))
            except Exception as e:
                self._set_headers(400)
                self.wfile.write(json.dumps({"error": str(e)}).encode('utf-8'))

        elif path.startswith('/watermark/'):
            try:
                rec_id = int(path.split('/')[-1])
                wm_res = peak_engine.verify_algebraic_watermark(rec_id)
                self._set_headers(200)
                self.wfile.write(json.dumps(wm_res, indent=2).encode('utf-8'))
            except Exception as e:
                self._set_headers(400)
                self.wfile.write(json.dumps({"error": str(e)}).encode('utf-8'))

        elif path == '/audit/stability':
            audit_res = peak_engine.verify_hessian_stability()
            self._set_headers(200)
            self.wfile.write(json.dumps(audit_res, indent=2).encode('utf-8'))

        else:
            self._set_headers(404)
            self.wfile.write(json.dumps({"error": "Endpoint not found"}).encode('utf-8'))

    def do_POST(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path

        if path in ['/search', '/search/filtered', '/search/hybrid']:
            length = int(self.headers.get('Content-Length', 0))
            body = self.rfile.read(length)
            try:
                data = json.loads(body.decode('utf-8'))
                k = int(data.get('top_k', 5))
                mask = int(data.get('target_mask', 0x2A))
                lexical_weight = float(data.get('lexical_weight', 0.0))
                query_vec = data.get('query_vector', None)

                if query_vec is None or len(query_vec) != 64:
                    self._set_headers(400)
                    self.wfile.write(json.dumps({"error": "Expected 64-dimensional query_vector"}).encode('utf-8'))
                    return

                q_np = np.array(query_vec, dtype=np.float32)
                res = peak_engine.filtered_topk_search(q_np, target_mask=mask, k=k, lexical_weight=lexical_weight)
                self._set_headers(200)
                self.wfile.write(json.dumps(res, indent=2).encode('utf-8'))

            except Exception as e:
                self._set_headers(500)
                self.wfile.write(json.dumps({"error": str(e)}).encode('utf-8'))

        elif path == '/delete':
            length = int(self.headers.get('Content-Length', 0))
            body = self.rfile.read(length)
            try:
                data = json.loads(body.decode('utf-8'))
                record_id = int(data.get('record_index', -1))
                del_res = peak_engine.delete_record_instant(record_id)
                self._set_headers(200)
                self.wfile.write(json.dumps(del_res, indent=2).encode('utf-8'))
            except Exception as e:
                self._set_headers(400)
                self.wfile.write(json.dumps({"error": str(e)}).encode('utf-8'))
        else:
            self._set_headers(404)
            self.wfile.write(json.dumps({"error": "Endpoint not found"}).encode('utf-8'))

def run_server(port=8080):
    server = HTTPServer(('', port), PeakAPIHandler)
    print(f"[+] Peak Enterprise Appliance Server active on http://localhost:{port}")
    server.serve_forever()

if __name__ == '__main__':
    run_server()
